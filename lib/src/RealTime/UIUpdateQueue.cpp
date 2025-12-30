#include "uiframework/RealTime/UIUpdateQueue.h"
#include <cstring>

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

} // namespace ui
