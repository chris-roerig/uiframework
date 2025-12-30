#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <mutex>
#include <queue>
#include <functional>

namespace ui {

// Forward declarations
class UIElement;

/**
 * @class FocusManager
 * @brief Centralized focus management for UI elements
 * 
 * Handles focus tracking, tab order, keyboard navigation, and focus events
 * in a thread-safe manner. Extracted from UICore for better separation of concerns.
 */
class FocusManager {
private:
    // Current focused element
    std::string focusedElementId;
    
    // Registry of all focusable elements (weak references to avoid circular dependencies)
    std::unordered_map<std::string, std::weak_ptr<UIElement>> elementRegistry;
    
    // Custom focus order (empty = use default registration order)
    std::vector<std::string> focusOrder;
    
    // Thread safety
    mutable std::mutex focusMutex;
    
    // Pending focus changes for thread-safe operations
    std::queue<std::string> pendingFocusChanges;
    
    // Pending callbacks to avoid deadlocks
    std::vector<std::function<void()>> pendingCallbacks;
    
public:
    FocusManager() = default;
    ~FocusManager() = default;
    
    // Non-copyable, non-movable
    FocusManager(const FocusManager&) = delete;
    FocusManager& operator=(const FocusManager&) = delete;
    FocusManager(FocusManager&&) = delete;
    FocusManager& operator=(FocusManager&&) = delete;
    
    // Element registration
    void registerElement(const std::string& elementId, std::weak_ptr<UIElement> element);
    void unregisterElement(const std::string& elementId);
    
    // Focus management
    void setFocus(const std::string& elementId);
    std::string getFocusedElementId() const;
    void clearFocus();
    
    // Tab order management (Phase 2)
    void setFocusOrder(const std::vector<std::string>& elementIds);
    std::vector<std::string> getFocusOrder() const;
    void focusNext();
    void focusPrevious();
    
    // Callback management
    void queueCallback(std::function<void()> callback);
    
    // Thread-safe focus change processing
    void processPendingFocusChanges();
    void processPendingCallbacks();
    
    // Utility methods
    bool hasFocusedElement() const;
    bool isElementFocusable(const std::string& elementId) const;
    std::vector<std::string> getFocusableElementIds() const;
    
private:
    // Internal helper methods
    void setFocusInternal(const std::string& elementId);
    int findElementInFocusOrder(const std::string& elementId) const;
    std::string getNextFocusableElement(const std::string& currentId) const;
    std::string getPreviousFocusableElement(const std::string& currentId) const;
};

} // namespace ui
