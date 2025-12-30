#pragma once
#include <queue>
#include <memory>
#include <mutex>
#include <unordered_map>

namespace ui {

// Forward declarations
class Label;
class Button;

/**
 * @brief Element pool for high-frequency UI scenarios like audio instruments
 * 
 * Provides pre-allocated UI elements to eliminate malloc/free overhead
 * in real-time audio applications where allocation can cause dropouts.
 */
class ElementPool {
private:
    std::queue<std::shared_ptr<Label>> labelPool;
    std::queue<std::shared_ptr<Button>> buttonPool;
    mutable std::mutex poolMutex;
    
    // Track active pooled elements for cleanup
    std::unordered_map<std::string, std::shared_ptr<Label>> activeLabelPool;
    std::unordered_map<std::string, std::shared_ptr<Button>> activeButtonPool;
    
public:
    /**
     * @brief Pre-allocate elements for the pool
     * @param labelCount Number of labels to pre-allocate
     * @param buttonCount Number of buttons to pre-allocate
     */
    void preAllocate(size_t labelCount = 50, size_t buttonCount = 20);
    
    /**
     * @brief Acquire a label from the pool (creates new if pool empty)
     * @return Shared pointer to label ready for use
     */
    std::shared_ptr<Label> acquireLabel();
    
    /**
     * @brief Acquire a button from the pool (creates new if pool empty)
     * @return Shared pointer to button ready for use
     */
    std::shared_ptr<Button> acquireButton();
    
    /**
     * @brief Return element to pool for reuse
     * @param elementId ID of element to return to pool
     */
    void releaseElement(const std::string& elementId);
    
    /**
     * @brief Get pool statistics for monitoring
     */
    struct PoolStats {
        size_t labelsAvailable;
        size_t buttonsAvailable;
        size_t labelsInUse;
        size_t buttonsInUse;
    };
    PoolStats getStats() const;
    
    /**
     * @brief Clear all pools (call during shutdown)
     */
    void clear();
};

} // namespace ui
