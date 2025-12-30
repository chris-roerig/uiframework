#include "uiframework/Focus/FocusManager.h"
#include "uiframework/UIElements/UIElement.h"
#include <algorithm>
#include <iostream>

namespace ui {

void FocusManager::registerElement(const std::string& elementId, std::weak_ptr<UIElement> element) {
    std::lock_guard<std::mutex> lock(focusMutex);
    elementRegistry[elementId] = element;
}

void FocusManager::unregisterElement(const std::string& elementId) {
    std::lock_guard<std::mutex> lock(focusMutex);
    
    // Clear focus from element if it was focused
    if (focusedElementId == elementId) {
        auto it = elementRegistry.find(elementId);
        if (it != elementRegistry.end()) {
            auto element = it->second.lock();
            if (element) {
                element->hasFocus = false;
                element->onFocusLost();
            }
        }
        focusedElementId.clear();
    }
    
    // Remove from registry
    elementRegistry.erase(elementId);
    
    // Remove from focus order if present
    auto it = std::find(focusOrder.begin(), focusOrder.end(), elementId);
    if (it != focusOrder.end()) {
        focusOrder.erase(it);
    }
}

void FocusManager::setFocus(const std::string& elementId) {
    // If we can't get the lock immediately, defer the focus change
    if (!focusMutex.try_lock()) {
        std::lock_guard<std::mutex> lock(focusMutex);
        pendingFocusChanges.push(elementId);
        return;
    }
    
    // We got the lock, proceed with focus change
    std::lock_guard<std::mutex> lock(focusMutex, std::adopt_lock);
    setFocusInternal(elementId);
}

void FocusManager::setFocusInternal(const std::string& elementId) {
    // Clear focus from current element
    if (!focusedElementId.empty()) {
        auto it = elementRegistry.find(focusedElementId);
        if (it != elementRegistry.end()) {
            auto currentFocused = it->second.lock();
            if (currentFocused) {
                currentFocused->hasFocus = false;
                currentFocused->onFocusLost();
            }
        }
    }
    
    // Set focus to new element
    auto it = elementRegistry.find(elementId);
    if (it != elementRegistry.end()) {
        auto newFocused = it->second.lock();
        if (newFocused && newFocused->isInteractive() && newFocused->isVisible()) {
            focusedElementId = elementId;
            newFocused->hasFocus = true;
            newFocused->onFocusGained();
        } else {
            focusedElementId.clear();
        }
    } else {
        focusedElementId.clear();
    }
}

std::string FocusManager::getFocusedElementId() const {
    std::lock_guard<std::mutex> lock(focusMutex);
    return focusedElementId;
}

void FocusManager::clearFocus() {
    std::lock_guard<std::mutex> lock(focusMutex);
    
    if (!focusedElementId.empty()) {
        auto it = elementRegistry.find(focusedElementId);
        if (it != elementRegistry.end()) {
            auto currentFocused = it->second.lock();
            if (currentFocused) {
                currentFocused->hasFocus = false;
                currentFocused->onFocusLost();
            }
        }
        focusedElementId.clear();
    }
}

void FocusManager::setFocusOrder(const std::vector<std::string>& elementIds) {
    std::lock_guard<std::mutex> lock(focusMutex);
    focusOrder = elementIds;
}

std::vector<std::string> FocusManager::getFocusOrder() const {
    std::lock_guard<std::mutex> lock(focusMutex);
    return focusOrder;
}

void FocusManager::focusNext() {
    std::lock_guard<std::mutex> lock(focusMutex);
    
    std::string nextElement = getNextFocusableElement(focusedElementId);
    if (!nextElement.empty()) {
        setFocusInternal(nextElement);
    }
}

void FocusManager::focusPrevious() {
    std::lock_guard<std::mutex> lock(focusMutex);
    
    std::string prevElement = getPreviousFocusableElement(focusedElementId);
    if (!prevElement.empty()) {
        setFocusInternal(prevElement);
    }
}

void FocusManager::queueCallback(std::function<void()> callback) {
    std::lock_guard<std::mutex> lock(focusMutex);
    pendingCallbacks.push_back(callback);
}

void FocusManager::processPendingFocusChanges() {
    std::vector<std::string> focusChanges;
    {
        std::lock_guard<std::mutex> lock(focusMutex);
        while (!pendingFocusChanges.empty()) {
            focusChanges.push_back(pendingFocusChanges.front());
            pendingFocusChanges.pop();
        }
    }
    
    // Process focus changes without holding the lock
    for (const auto& elementId : focusChanges) {
        setFocus(elementId);
    }
}

void FocusManager::processPendingCallbacks() {
    std::vector<std::function<void()>> callbacks;
    {
        std::lock_guard<std::mutex> lock(focusMutex);
        callbacks = std::move(pendingCallbacks);
        pendingCallbacks.clear();
    }
    
    // Process callbacks without holding the lock
    for (const auto& callback : callbacks) {
        if (callback) {
            try {
                callback();
            } catch (const std::exception& e) {
                std::cerr << "Error in pending callback: " << e.what() << std::endl;
            }
        }
    }
}

bool FocusManager::hasFocusedElement() const {
    std::lock_guard<std::mutex> lock(focusMutex);
    return !focusedElementId.empty();
}

bool FocusManager::isElementFocusable(const std::string& elementId) const {
    // Note: This method assumes the mutex is already held by the caller
    auto it = elementRegistry.find(elementId);
    if (it != elementRegistry.end()) {
        auto element = it->second.lock();
        return element && element->isInteractive() && element->isVisible();
    }
    return false;
}

std::vector<std::string> FocusManager::getFocusableElementIds() const {
    // Note: This method assumes the mutex is already held by the caller
    std::vector<std::string> focusableIds;
    std::vector<std::string> expiredIds;
    
    for (const auto& pair : elementRegistry) {
        auto element = pair.second.lock();
        if (element) {
            if (element->isInteractive() && element->isVisible()) {
                focusableIds.push_back(pair.first);
            }
        } else {
            expiredIds.push_back(pair.first);
        }
    }
    
    // Clean up expired entries (const_cast needed for cleanup in const method)
    if (!expiredIds.empty()) {
        auto& mutableRegistry = const_cast<std::unordered_map<std::string, std::weak_ptr<UIElement>>&>(elementRegistry);
        for (const auto& expiredId : expiredIds) {
            mutableRegistry.erase(expiredId);
        }
    }
    
    return focusableIds;
}

int FocusManager::findElementInFocusOrder(const std::string& elementId) const {
    auto it = std::find(focusOrder.begin(), focusOrder.end(), elementId);
    return (it != focusOrder.end()) ? static_cast<int>(it - focusOrder.begin()) : -1;
}

std::string FocusManager::getNextFocusableElement(const std::string& currentId) const {
    // Get elements to cycle through (group or all)
    auto elementsToSearch = getActiveGroupElements();
    
    if (elementsToSearch.empty()) return "";
    
    if (currentId.empty()) {
        return elementsToSearch[0];
    }
    
    auto it = std::find(elementsToSearch.begin(), elementsToSearch.end(), currentId);
    if (it != elementsToSearch.end()) {
        ++it;
        if (it != elementsToSearch.end()) {
            return *it;
        }
    }
    // Wrap around to first element
    return elementsToSearch[0];
}

std::string FocusManager::getPreviousFocusableElement(const std::string& currentId) const {
    // Get elements to cycle through (group or all)
    auto elementsToSearch = getActiveGroupElements();
    
    if (elementsToSearch.empty()) return "";
    
    if (currentId.empty()) {
        return elementsToSearch.back();
    }
    
    auto it = std::find(elementsToSearch.begin(), elementsToSearch.end(), currentId);
    if (it != elementsToSearch.end() && it != elementsToSearch.begin()) {
        --it;
        return *it;
    }
    // Wrap around to last element
    return elementsToSearch.back();
}

// Focus Groups and Trapping (Phase 4)
void FocusManager::createFocusGroup(const std::string& groupName, const std::vector<std::string>& elementIds) {
    std::lock_guard<std::mutex> lock(focusMutex);
    focusGroups[groupName] = elementIds;
}

void FocusManager::setActiveFocusGroup(const std::string& groupName) {
    std::lock_guard<std::mutex> lock(focusMutex);
    if (focusGroups.find(groupName) != focusGroups.end()) {
        activeFocusGroup = groupName;
    }
}

void FocusManager::clearActiveFocusGroup() {
    std::lock_guard<std::mutex> lock(focusMutex);
    activeFocusGroup.clear();
}

void FocusManager::trapFocus(const std::string& groupName) {
    std::lock_guard<std::mutex> lock(focusMutex);
    if (focusGroups.find(groupName) != focusGroups.end()) {
        activeFocusGroup = groupName;
        focusTrapActive = true;
    }
}

void FocusManager::releaseFocusTrap() {
    std::lock_guard<std::mutex> lock(focusMutex);
    focusTrapActive = false;
    activeFocusGroup.clear();
}

std::vector<std::string> FocusManager::getActiveGroupElements() const {
    if (!activeFocusGroup.empty()) {
        auto it = focusGroups.find(activeFocusGroup);
        if (it != focusGroups.end()) {
            // Filter for focusable elements in group
            std::vector<std::string> focusableInGroup;
            for (const std::string& elementId : it->second) {
                if (isElementFocusable(elementId)) {
                    focusableInGroup.push_back(elementId);
                }
            }
            return focusableInGroup;
        }
    }
    
    // No active group, use normal focus order or registration order
    if (!focusOrder.empty()) {
        std::vector<std::string> focusableInOrder;
        for (const std::string& elementId : focusOrder) {
            if (isElementFocusable(elementId)) {
                focusableInOrder.push_back(elementId);
            }
        }
        return focusableInOrder;
    }
    
    return getFocusableElementIds();
}

bool FocusManager::isElementInActiveGroup(const std::string& elementId) const {
    if (activeFocusGroup.empty()) return true;
    
    auto it = focusGroups.find(activeFocusGroup);
    if (it != focusGroups.end()) {
        return std::find(it->second.begin(), it->second.end(), elementId) != it->second.end();
    }
    
    return true;
}

void FocusManager::cleanupExpiredElements() {
    std::lock_guard<std::mutex> lock(focusMutex);
    
    auto it = elementRegistry.begin();
    while (it != elementRegistry.end()) {
        if (it->second.expired()) {
            it = elementRegistry.erase(it);
        } else {
            ++it;
        }
    }
}

} // namespace ui
