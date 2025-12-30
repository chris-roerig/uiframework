#include "uiframework/RealTime/UIUpdateQueue.h"
#include <cstring>
#include <vector>

namespace ui {

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

} // namespace ui
