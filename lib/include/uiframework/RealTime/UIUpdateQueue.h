#pragma once
#include <atomic>
#include <memory>
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

namespace ui {

// Forward declarations
class UIElement;

/**
 * @brief Non-blocking element cache for real-time access
 */
struct ElementCache {
    std::string elementId;
    std::shared_ptr<UIElement> element;
    bool valid = true;
    
    ElementCache() = default;
    ElementCache(const std::string& id, std::shared_ptr<UIElement> elem) 
        : elementId(id), element(std::move(elem)) {}
    
    // Move constructor and assignment
    ElementCache(ElementCache&& other) noexcept 
        : elementId(std::move(other.elementId)), element(std::move(other.element)), valid(other.valid) {}
    
    ElementCache& operator=(ElementCache&& other) noexcept {
        if (this != &other) {
            elementId = std::move(other.elementId);
            element = std::move(other.element);
            valid = other.valid;
        }
        return *this;
    }
    
    // Delete copy operations for performance
    ElementCache(const ElementCache&) = delete;
    ElementCache& operator=(const ElementCache&) = delete;
};

/**
 * @brief Batched update state for efficient processing
 */
struct BatchedUpdate {
    std::string elementId;
    
    // Track which updates are pending
    bool hasText = false;
    bool hasPosition = false;
    bool hasSize = false;
    bool hasValue = false;
    bool hasVisibility = false;
    
    // Latest values (only most recent matters)
    std::string textValue;
    struct { int x, y; } position;
    struct { int width, height; } size;
    float value;
    bool visible;
    
    // Callbacks must be processed in order
    std::vector<std::function<void()>> callbacks;
};

/**
 * @brief Lock-free update operation for audio thread safety
 */
struct UIUpdate {
    enum Type {
        SET_TEXT,
        SET_POSITION,
        SET_SIZE,
        SET_VALUE,
        SET_VISIBILITY,
        CUSTOM_CALLBACK
    };
    
    Type type;
    std::string elementId;
    
    // Union for different update data
    union {
        struct { int x, y; } position;
        struct { int width, height; } size;
        struct { float value; } floatValue;
        struct { bool visible; } visibility;
    } data;
    
    std::string textValue;
    std::function<void()> callback;
    
    UIUpdate() = default;
    UIUpdate(Type t, const std::string& id) : type(t), elementId(id) {}
};

/**
 * @brief Lock-free SPSC queue for UI updates from audio threads
 * 
 * Single Producer, Single Consumer queue optimized for real-time audio.
 * Producer (audio thread) writes updates, Consumer (UI thread) processes them.
 */
class UIUpdateQueue {
private:
    static constexpr size_t QUEUE_SIZE = 1024; // Power of 2 for fast modulo
    static constexpr size_t QUEUE_MASK = QUEUE_SIZE - 1;
    
    alignas(64) std::atomic<size_t> writeIndex{0};
    alignas(64) std::atomic<size_t> readIndex{0};
    alignas(64) UIUpdate queue[QUEUE_SIZE];
    
public:
    /**
     * @brief Try to enqueue update (non-blocking, audio thread safe)
     * @return true if enqueued, false if queue full
     */
    bool tryEnqueue(const UIUpdate& update);
    
    /**
     * @brief Try to dequeue update (non-blocking, UI thread only)
     * @return true if dequeued, false if queue empty
     */
    bool tryDequeue(UIUpdate& update);
    
    /**
     * @brief Check if queue is empty
     */
    bool empty() const;
    
    /**
     * @brief Process all queued updates with batching and element caching
     * @param batchMap Output map of batched updates by element ID
     * @param elementCache Cache for non-blocking element access
     * @return Number of updates processed
     */
    size_t processBatchWithCache(std::unordered_map<std::string, BatchedUpdate>& batchMap,
                                std::vector<ElementCache>& elementCache);
    
    /**
     * @brief Process all queued updates with batching optimization
     * @param batchMap Output map of batched updates by element ID
     * @return Number of updates processed
     */
    size_t processBatch(std::unordered_map<std::string, BatchedUpdate>& batchMap);
    
    /**
     * @brief Get approximate queue size (for monitoring)
     */
    size_t size() const;
};

} // namespace ui
