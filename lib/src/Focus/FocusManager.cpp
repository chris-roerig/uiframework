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
    for (const auto& pair : elementRegistry) {
        auto element = pair.second.lock();
        if (element && element->isInteractive() && element->isVisible()) {
            focusableIds.push_back(pair.first);
        }
    }
    return focusableIds;
}

int FocusManager::findElementInFocusOrder(const std::string& elementId) const {
    auto it = std::find(focusOrder.begin(), focusOrder.end(), elementId);
    return (it != focusOrder.end()) ? static_cast<int>(it - focusOrder.begin()) : -1;
}

std::string FocusManager::getNextFocusableElement(const std::string& currentId) const {
    if (!focusOrder.empty()) {
        // Use custom focus order
        int currentIndex = findElementInFocusOrder(currentId);
        if (currentIndex >= 0) {
            // Find next focusable element in order
            for (size_t i = 1; i < focusOrder.size(); ++i) {
                int nextIndex = (currentIndex + i) % focusOrder.size();
                const std::string& nextId = focusOrder[nextIndex];
                if (isElementFocusable(nextId)) {
                    return nextId;
                }
            }
        } else {
            // Current element not in focus order, start from beginning
            for (const std::string& elementId : focusOrder) {
                if (isElementFocusable(elementId)) {
                    return elementId;
                }
            }
        }
    } else {
        // Use registration order (default behavior)
        auto focusableIds = getFocusableElementIds();
        if (focusableIds.empty()) return "";
        
        if (currentId.empty()) {
            return focusableIds[0];
        }
        
        auto it = std::find(focusableIds.begin(), focusableIds.end(), currentId);
        if (it != focusableIds.end()) {
            ++it;
            if (it != focusableIds.end()) {
                return *it;
            }
        }
        // Wrap around to first element
        return focusableIds[0];
    }
    
    return "";
}

std::string FocusManager::getPreviousFocusableElement(const std::string& currentId) const {
    if (!focusOrder.empty()) {
        // Use custom focus order
        int currentIndex = findElementInFocusOrder(currentId);
        if (currentIndex >= 0) {
            // Find previous focusable element in order
            for (size_t i = 1; i < focusOrder.size(); ++i) {
                int prevIndex = (currentIndex - i + focusOrder.size()) % focusOrder.size();
                const std::string& prevId = focusOrder[prevIndex];
                if (isElementFocusable(prevId)) {
                    return prevId;
                }
            }
        } else {
            // Current element not in focus order, start from end
            for (auto it = focusOrder.rbegin(); it != focusOrder.rend(); ++it) {
                if (isElementFocusable(*it)) {
                    return *it;
                }
            }
        }
    } else {
        // Use registration order (default behavior)
        auto focusableIds = getFocusableElementIds();
        if (focusableIds.empty()) return "";
        
        if (currentId.empty()) {
            return focusableIds.back();
        }
        
        auto it = std::find(focusableIds.begin(), focusableIds.end(), currentId);
        if (it != focusableIds.end() && it != focusableIds.begin()) {
            --it;
            return *it;
        }
        // Wrap around to last element
        return focusableIds.back();
    }
    
    return "";
}

} // namespace ui
