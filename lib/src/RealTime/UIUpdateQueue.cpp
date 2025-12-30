#include "uiframework/RealTime/UIUpdateQueue.h"
#include <cstring>
#include <vector>
#include <algorithm>

namespace ui {

std::string_view StringPool::allocateString(const std::string& str) {
    if (!hasCapacity(str.length() + 1) || stringCount >= MAX_STRINGS) {
        return {}; // Pool exhausted
    }
    
    // Copy string to pool
    std::memcpy(pool + poolOffset, str.c_str(), str.length());
    pool[poolOffset + str.length()] = '\0';
    
    // Create string_view
    std::string_view result(pool + poolOffset, str.length());
    strings[stringCount++] = result;
    
    poolOffset += str.length() + 1;
    return result;
}

void StringPool::reset() {
    poolOffset = 0;
    stringCount = 0;
}

bool StringPool::hasCapacity(size_t length) const {
    return (poolOffset + length) < POOL_SIZE;
}

bool UIUpdateQueue::tryEnqueue(const UIUpdate& update) {
    const size_t currentWrite = writeIndex.load(std::memory_order_relaxed);
    const size_t nextWrite = (currentWrite + 1) & QUEUE_MASK;
    
    // Check if queue is full
    if (nextWrite == readIndex.load(std::memory_order_acquire)) {
        return false; // Queue full
    }
    
    // Copy update data
    queue[currentWrite] = update;
    
    // Publish the write
    writeIndex.store(nextWrite, std::memory_order_release);
    return true;
}

bool UIUpdateQueue::tryDequeue(UIUpdate& update) {
    const size_t currentRead = readIndex.load(std::memory_order_relaxed);
    
    // Check if queue is empty
    if (currentRead == writeIndex.load(std::memory_order_acquire)) {
        return false; // Queue empty
    }
    
    // Copy update data
    update = queue[currentRead];
    
    // Publish the read
    const size_t nextRead = (currentRead + 1) & QUEUE_MASK;
    readIndex.store(nextRead, std::memory_order_release);
    return true;
}

bool UIUpdateQueue::empty() const {
    return readIndex.load(std::memory_order_acquire) == 
           writeIndex.load(std::memory_order_acquire);
}

size_t UIUpdateQueue::size() const {
    const size_t write = writeIndex.load(std::memory_order_acquire);
    const size_t read = readIndex.load(std::memory_order_acquire);
    return (write - read) & QUEUE_MASK;
}

size_t UIUpdateQueue::processBatch(std::unordered_map<std::string, BatchedUpdate>& batchMap) {
    UIUpdate update;
    size_t processedCount = 0;
    
    // Process all queued updates into batches
    while (tryDequeue(update)) {
        processedCount++;
        
        if (update.type == UIUpdate::CUSTOM_CALLBACK) {
            // Callbacks must be processed immediately in order
            if (update.callback) {
                update.callback();
            }
            continue;
        }
        
        // Get or create batch for this element
        BatchedUpdate& batch = batchMap[update.elementId];
        batch.elementId = update.elementId;
        
        // Coalesce updates (latest value wins)
        switch (update.type) {
            case UIUpdate::SET_TEXT:
                batch.hasText = true;
                batch.textValue = std::move(update.textValue);
                break;
                
            case UIUpdate::SET_POSITION:
                batch.hasPosition = true;
                batch.position.x = update.data.position.x;
                batch.position.y = update.data.position.y;
                break;
                
            case UIUpdate::SET_SIZE:
                batch.hasSize = true;
                batch.size.width = update.data.size.width;
                batch.size.height = update.data.size.height;
                break;
                
            case UIUpdate::SET_VALUE:
                batch.hasValue = true;
                batch.value = update.data.floatValue.value;
                break;
                
            case UIUpdate::SET_VISIBILITY:
                batch.hasVisibility = true;
                batch.visible = update.data.visibility.visible;
                break;
                
            default:
                break;
        }
    }
    
    return processedCount;
}

size_t UIUpdateQueue::processBatchWithCache(std::unordered_map<std::string, BatchedUpdate>& batchMap,
                                           std::vector<ElementCache>& elementCache) {
    UIUpdate update;
    size_t processedCount = 0;
    
    // Process all queued updates into batches
    while (tryDequeue(update)) {
        processedCount++;
        
        if (update.type == UIUpdate::CUSTOM_CALLBACK) {
            // Callbacks must be processed immediately in order
            if (update.callback) {
                update.callback();
            }
            continue;
        }
        
        // Get or create batch for this element
        BatchedUpdate& batch = batchMap[update.elementId];
        batch.elementId = update.elementId;
        
        // Coalesce updates (latest value wins)
        switch (update.type) {
            case UIUpdate::SET_TEXT:
                batch.hasText = true;
                batch.textValue = std::move(update.textValue);
                break;
                
            case UIUpdate::SET_POSITION:
                batch.hasPosition = true;
                batch.position.x = update.data.position.x;
                batch.position.y = update.data.position.y;
                break;
                
            case UIUpdate::SET_SIZE:
                batch.hasSize = true;
                batch.size.width = update.data.size.width;
                batch.size.height = update.data.size.height;
                break;
                
            case UIUpdate::SET_VALUE:
                batch.hasValue = true;
                batch.value = update.data.floatValue.value;
                break;
                
            case UIUpdate::SET_VISIBILITY:
                batch.hasVisibility = true;
                batch.visible = update.data.visibility.visible;
                break;
                
            default:
                break;
        }
    }
    
    // Pre-populate element cache for non-blocking access
    elementCache.clear();
    elementCache.reserve(batchMap.size());
    
    for (const auto& [elementId, batch] : batchMap) {
        // Cache will be populated by UI thread with actual elements
        elementCache.emplace_back(elementId, nullptr);
    }
    
    return processedCount;
}

size_t UIUpdateQueue::processPredictable(std::vector<ElementCache>& elementCache) {
    // Reset string pool for this frame
    stringPool.reset();
    activeBatches = 0;
    
    UIUpdate update;
    size_t processedCount = 0;
    
    // Process all queued updates into predictable batches
    while (tryDequeue(update) && activeBatches < predictableBatches.size()) {
        processedCount++;
        
        if (update.type == UIUpdate::CUSTOM_CALLBACK) {
            // Callbacks must be processed immediately in order
            if (update.callback) {
                update.callback();
            }
            continue;
        }
        
        // Allocate element ID in string pool
        std::string_view elementId = stringPool.allocateString(update.elementId);
        if (elementId.empty()) {
            // Pool exhausted, fall back to regular processing
            break;
        }
        
        // Find or create predictable batch
        PredictableBatch* batch = nullptr;
        for (size_t i = 0; i < activeBatches; ++i) {
            if (predictableBatches[i].elementId == elementId) {
                batch = &predictableBatches[i];
                break;
            }
        }
        
        if (!batch && activeBatches < predictableBatches.size()) {
            batch = &predictableBatches[activeBatches++];
            batch->elementId = elementId;
            // Reset batch state
            batch->hasText = batch->hasPosition = batch->hasSize = 
            batch->hasValue = batch->hasVisibility = false;
        }
        
        if (!batch) {
            // No more batch slots available
            break;
        }
        
        // Coalesce updates (latest value wins)
        switch (update.type) {
            case UIUpdate::SET_TEXT:
                batch->hasText = true;
                batch->textValue = stringPool.allocateString(update.textValue);
                break;
                
            case UIUpdate::SET_POSITION:
                batch->hasPosition = true;
                batch->position.x = update.data.position.x;
                batch->position.y = update.data.position.y;
                break;
                
            case UIUpdate::SET_SIZE:
                batch->hasSize = true;
                batch->size.width = update.data.size.width;
                batch->size.height = update.data.size.height;
                break;
                
            case UIUpdate::SET_VALUE:
                batch->hasValue = true;
                batch->value = update.data.floatValue.value;
                break;
                
            case UIUpdate::SET_VISIBILITY:
                batch->hasVisibility = true;
                batch->visible = update.data.visibility.visible;
                break;
                
            default:
                break;
        }
    }
    
    // Pre-populate element cache for non-blocking access
    elementCache.clear();
    elementCache.reserve(activeBatches);
    
    for (size_t i = 0; i < activeBatches; ++i) {
        const auto& batch = predictableBatches[i];
        // Convert string_view back to string for element lookup
        std::string elementIdStr(batch.elementId);
        elementCache.emplace_back(elementIdStr, nullptr);
    }
    
    return processedCount;
}

bool UIUpdateQueue::tryScheduleUpdate(const UIUpdate& update, HighResTimePoint scheduledTime) {
    // For simplicity, add to regular queue with timestamp
    // In production, this could use a separate lock-free scheduled queue
    UIUpdate timedUpdate = update;
    timedUpdate.timestamp = scheduledTime;
    
    bool result = tryEnqueue(timedUpdate);
    if (result) {
        hasScheduledUpdates.store(true, std::memory_order_release);
    }
    return result;
}

size_t UIUpdateQueue::processScheduledUpdates(HighResTimePoint currentTime) {
    if (!hasScheduledUpdates.load(std::memory_order_acquire)) {
        return 0;
    }
    
    size_t processedCount = 0;
    UIUpdate update;
    
    // Process updates that are ready (timestamp <= currentTime)
    while (tryDequeue(update)) {
        if (update.timestamp <= currentTime) {
            // Re-enqueue for immediate processing
            if (tryEnqueue(update)) {
                processedCount++;
            }
        } else {
            // Update not ready yet, put it back
            // In production, this would use a proper scheduled queue
            break;
        }
    }
    
    return processedCount;
}

// SIMD helper functions for bulk operations
namespace {
    /**
     * @brief Check if SIMD is available (compile-time detection)
     */
    constexpr bool hasSIMD() {
#if defined(__SSE2__) || defined(__AVX2__) || defined(__ARM_NEON)
        return true;
#else
        return false;
#endif
    }
    
    /**
     * @brief Vectorized bulk processing for compatible data types
     */
    template<typename T>
    void processBulkData(const std::vector<T>& data, size_t count) {
        // SIMD optimization placeholder - process in chunks of 4/8
        constexpr size_t SIMD_WIDTH = hasSIMD() ? 4 : 1;
        
        size_t vectorized = (count / SIMD_WIDTH) * SIMD_WIDTH;
        
        // Process vectorized portion
        for (size_t i = 0; i < vectorized; i += SIMD_WIDTH) {
            // SIMD operations would go here
            // For now, process sequentially
            for (size_t j = 0; j < SIMD_WIDTH && (i + j) < count; ++j) {
                // Individual processing
            }
        }
        
        // Process remaining elements
        for (size_t i = vectorized; i < count; ++i) {
            // Individual processing
        }
    }
}

size_t UIUpdateQueue::tryBulkSetText(const std::vector<std::string>& elementIds, 
                                    const std::vector<std::string>& textValues) {
    if (elementIds.size() != textValues.size()) {
        return 0; // Size mismatch
    }
    
    size_t count = elementIds.size();
    size_t successCount = 0;
    
    // Pre-check queue capacity
    size_t availableSpace = QUEUE_SIZE - size();
    if (count > availableSpace) {
        count = availableSpace; // Limit to available space
    }
    
    // Vectorized processing hint
    processBulkData(textValues, count);
    
    // Batch enqueue operations
    for (size_t i = 0; i < count; ++i) {
        UIUpdate update(UIUpdate::SET_TEXT, elementIds[i]);
        update.textValue = textValues[i];
        
        if (tryEnqueue(update)) {
            successCount++;
        } else {
            break; // Queue full
        }
    }
    
    return successCount;
}

size_t UIUpdateQueue::tryBulkSetPosition(const std::vector<std::string>& elementIds,
                                        const std::vector<std::pair<int, int>>& positions) {
    if (elementIds.size() != positions.size()) {
        return 0; // Size mismatch
    }
    
    size_t count = elementIds.size();
    size_t successCount = 0;
    
    // Pre-check queue capacity
    size_t availableSpace = QUEUE_SIZE - size();
    if (count > availableSpace) {
        count = availableSpace;
    }
    
    // Vectorized processing hint
    processBulkData(positions, count);
    
    // Batch enqueue operations
    for (size_t i = 0; i < count; ++i) {
        UIUpdate update(UIUpdate::SET_POSITION, elementIds[i]);
        update.data.position.x = positions[i].first;
        update.data.position.y = positions[i].second;
        
        if (tryEnqueue(update)) {
            successCount++;
        } else {
            break; // Queue full
        }
    }
    
    return successCount;
}

size_t UIUpdateQueue::tryBulkSetValue(const std::vector<std::string>& elementIds,
                                     const std::vector<float>& values) {
    if (elementIds.size() != values.size()) {
        return 0; // Size mismatch
    }
    
    size_t count = elementIds.size();
    size_t successCount = 0;
    
    // Pre-check queue capacity
    size_t availableSpace = QUEUE_SIZE - size();
    if (count > availableSpace) {
        count = availableSpace;
    }
    
    // Vectorized processing hint for float arrays
    processBulkData(values, count);
    
    // Batch enqueue operations
    for (size_t i = 0; i < count; ++i) {
        UIUpdate update(UIUpdate::SET_VALUE, elementIds[i]);
        update.data.floatValue.value = values[i];
        
        if (tryEnqueue(update)) {
            successCount++;
        } else {
            break; // Queue full
        }
    }
    
    return successCount;
}

size_t UIUpdateQueue::tryBulkSetVisibility(const std::vector<std::string>& elementIds,
                                          const std::vector<bool>& visibility) {
    if (elementIds.size() != visibility.size()) {
        return 0; // Size mismatch
    }
    
    size_t count = elementIds.size();
    size_t successCount = 0;
    
    // Pre-check queue capacity
    size_t availableSpace = QUEUE_SIZE - size();
    if (count > availableSpace) {
        count = availableSpace;
    }
    
    // Vectorized processing hint for boolean arrays
    processBulkData(visibility, count);
    
    // Batch enqueue operations
    for (size_t i = 0; i < count; ++i) {
        UIUpdate update(UIUpdate::SET_VISIBILITY, elementIds[i]);
        update.data.visibility.visible = visibility[i];
        
        if (tryEnqueue(update)) {
            successCount++;
        } else {
            break; // Queue full
        }
    }
    
    return successCount;
}

} // namespace ui
