#pragma once
#include <atomic>
#include <memory>
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>
#include <array>
#include <string_view>
#include <chrono>
#include <queue>

namespace ui {

// Forward declarations
class UIElement;

/**
 * @brief High-resolution timestamp type for precise timing
 */
using HighResTimePoint = std::chrono::high_resolution_clock::time_point;

/**
 * @brief Memory pool for predictable string allocations
 */
class StringPool {
private:
    static constexpr size_t POOL_SIZE = 16384; // 16KB string pool
    static constexpr size_t MAX_STRINGS = 512;
    
    char pool[POOL_SIZE];
    size_t poolOffset = 0;
    std::array<std::string_view, MAX_STRINGS> strings;
    size_t stringCount = 0;
    
public:
    /**
     * @brief Allocate string from pool (real-time safe)
     */
    std::string_view allocateString(const std::string& str);
    
    /**
     * @brief Reset pool for next frame
     */
    void reset();
    
    /**
     * @brief Check if pool has capacity
     */
    bool hasCapacity(size_t length) const;
};

/**
 * @brief Pre-allocated update structure for deterministic memory usage
 */
struct PredictableUpdate {
    enum Type {
        SET_TEXT,
        SET_POSITION,
        SET_SIZE,
        SET_VALUE,
        SET_VISIBILITY,
        CUSTOM_CALLBACK
    };
    
    Type type;
    std::string_view elementId; // Points to string pool
    
    // Union for different update data
    union {
        struct { int x, y; } position;
        struct { int width, height; } size;
        struct { float value; } floatValue;
        struct { bool visible; } visibility;
    } data;
    
    std::string_view textValue; // Points to string pool
    std::function<void()> callback;
    
    PredictableUpdate() = default;
    PredictableUpdate(Type t, std::string_view id) : type(t), elementId(id) {}
};

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
 * @brief Memory-predictable batched update state
 */
struct PredictableBatch {
    std::string_view elementId; // Points to string pool
    
    // Track which updates are pending
    bool hasText = false;
    bool hasPosition = false;
    bool hasSize = false;
    bool hasValue = false;
    bool hasVisibility = false;
    
    // Latest values (only most recent matters)
    std::string_view textValue; // Points to string pool
    struct { int x, y; } position;
    struct { int width, height; } size;
    float value;
    bool visible;
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
    HighResTimePoint timestamp; // High-resolution timestamp
    
    // Union for different update data
    union {
        struct { int x, y; } position;
        struct { int width, height; } size;
        struct { float value; } floatValue;
        struct { bool visible; } visibility;
    } data;
    
    std::string textValue;
    std::function<void()> callback;
    
    UIUpdate() : timestamp(std::chrono::high_resolution_clock::now()) {}
    UIUpdate(Type t, const std::string& id) : type(t), elementId(id), 
        timestamp(std::chrono::high_resolution_clock::now()) {}
    UIUpdate(Type t, const std::string& id, HighResTimePoint time) : type(t), elementId(id), 
        timestamp(time) {}
};

/**
 * @brief Scheduled update for precise timing control
 */
struct ScheduledUpdate {
    HighResTimePoint scheduledTime;
    UIUpdate update;
    
    ScheduledUpdate(HighResTimePoint time, UIUpdate upd) 
        : scheduledTime(time), update(std::move(upd)) {}
    
    // Priority queue ordering (earliest time first)
    bool operator>(const ScheduledUpdate& other) const {
        return scheduledTime > other.scheduledTime;
    }
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
    
    // Memory-predictable processing structures
    StringPool stringPool;
    std::array<PredictableBatch, 128> predictableBatches; // Pre-allocated batches
    size_t activeBatches = 0;
    
    // High-resolution timing structures
    std::priority_queue<ScheduledUpdate, std::vector<ScheduledUpdate>, 
                       std::greater<ScheduledUpdate>> scheduledQueue;
    std::atomic<bool> hasScheduledUpdates{false};
    
public:
    /**
     * @brief Schedule update for future execution (real-time safe)
     * @param update Update to schedule
     * @param scheduledTime When to execute the update
     * @return true if scheduled, false if queue full
     */
    bool tryScheduleUpdate(const UIUpdate& update, HighResTimePoint scheduledTime);
    
    /**
     * @brief Process scheduled updates that are ready
     * @param currentTime Current high-resolution time
     * @return Number of scheduled updates processed
     */
    size_t processScheduledUpdates(HighResTimePoint currentTime = std::chrono::high_resolution_clock::now());
    
    /**
     * @brief Bulk update operations for SIMD optimization
     */
    
    /**
     * @brief Bulk set text values (SIMD optimized)
     * @param elementIds Vector of element IDs
     * @param textValues Vector of text values (must match elementIds size)
     * @return Number of updates successfully queued
     */
    size_t tryBulkSetText(const std::vector<std::string>& elementIds, 
                         const std::vector<std::string>& textValues);
    
    /**
     * @brief Bulk set positions (SIMD optimized)
     * @param elementIds Vector of element IDs
     * @param positions Vector of {x, y} positions
     * @return Number of updates successfully queued
     */
    size_t tryBulkSetPosition(const std::vector<std::string>& elementIds,
                             const std::vector<std::pair<int, int>>& positions);
    
    /**
     * @brief Bulk set float values (SIMD optimized)
     * @param elementIds Vector of element IDs
     * @param values Vector of float values
     * @return Number of updates successfully queued
     */
    size_t tryBulkSetValue(const std::vector<std::string>& elementIds,
                          const std::vector<float>& values);
    
    /**
     * @brief Bulk set visibility (SIMD optimized)
     * @param elementIds Vector of element IDs
     * @param visibility Vector of visibility states
     * @return Number of updates successfully queued
     */
    size_t tryBulkSetVisibility(const std::vector<std::string>& elementIds,
                               const std::vector<bool>& visibility);
    
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
     * @brief Process updates with memory-predictable operations
     * @param elementCache Cache for non-blocking element access
     * @return Number of updates processed
     */
    size_t processPredictable(std::vector<ElementCache>& elementCache);
    
    /**
     * @brief Get predictable batches for processing
     */
    const std::array<PredictableBatch, 128>& getPredictableBatches() const { return predictableBatches; }
    
    /**
     * @brief Get number of active batches
     */
    size_t getActiveBatches() const { return activeBatches; }
    
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
