#include "uiframework/Resources/ElementPool.h"
#include "uiframework/UIElements/Label.h"
#include "uiframework/UIElements/Button.h"

namespace ui {

void ElementPool::preAllocate(size_t labelCount, size_t buttonCount) {
    std::lock_guard<std::mutex> lock(poolMutex);
    
    // Pre-allocate labels
    for (size_t i = 0; i < labelCount; ++i) {
        auto label = std::make_shared<Label>(0, 0, 100, 30, "");
        labelPool.push(label);
    }
    
    // Pre-allocate buttons  
    for (size_t i = 0; i < buttonCount; ++i) {
        auto button = std::make_shared<Button>(0, 0, 100, 30, "", [](){});
        buttonPool.push(button);
    }
}

std::shared_ptr<Label> ElementPool::acquireLabel() {
    std::lock_guard<std::mutex> lock(poolMutex);
    
    std::shared_ptr<Label> label;
    if (!labelPool.empty()) {
        label = labelPool.front();
        labelPool.pop();
        
        // Reset to clean state
        label->setText("");
        label->setPosition(0, 0);
        label->hasFocus = false;
    } else {
        // Pool empty, create new
        label = std::make_shared<Label>(0, 0, 100, 30, "");
    }
    
    // Track active element
    activeLabelPool[label->getId()] = label;
    return label;
}

std::shared_ptr<Button> ElementPool::acquireButton() {
    std::lock_guard<std::mutex> lock(poolMutex);
    
    std::shared_ptr<Button> button;
    if (!buttonPool.empty()) {
        button = buttonPool.front();
        buttonPool.pop();
        
        // Reset to clean state
        button->setText("");
        button->setPosition(0, 0);
        button->setCallback([](){});
        button->hasFocus = false;
    } else {
        // Pool empty, create new
        button = std::make_shared<Button>(0, 0, 100, 30, "", [](){});
    }
    
    // Track active element
    activeButtonPool[button->getId()] = button;
    return button;
}

void ElementPool::releaseElement(const std::string& elementId) {
    std::lock_guard<std::mutex> lock(poolMutex);
    
    // Check if it's a pooled label
    auto labelIt = activeLabelPool.find(elementId);
    if (labelIt != activeLabelPool.end()) {
        labelPool.push(labelIt->second);
        activeLabelPool.erase(labelIt);
        return;
    }
    
    // Check if it's a pooled button
    auto buttonIt = activeButtonPool.find(elementId);
    if (buttonIt != activeButtonPool.end()) {
        buttonPool.push(buttonIt->second);
        activeButtonPool.erase(buttonIt);
        return;
    }
}

ElementPool::PoolStats ElementPool::getStats() const {
    std::lock_guard<std::mutex> lock(poolMutex);
    
    return {
        .labelsAvailable = labelPool.size(),
        .buttonsAvailable = buttonPool.size(),
        .labelsInUse = activeLabelPool.size(),
        .buttonsInUse = activeButtonPool.size()
    };
}

void ElementPool::clear() {
    std::lock_guard<std::mutex> lock(poolMutex);
    
    // Clear all pools
    while (!labelPool.empty()) labelPool.pop();
    while (!buttonPool.empty()) buttonPool.pop();
    
    activeLabelPool.clear();
    activeButtonPool.clear();
}

} // namespace ui
