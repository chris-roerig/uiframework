#pragma once
#include <atomic>
#include <memory>
#include <functional>
#include <string>

namespace ui {

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
     * @brief Get approximate queue size (for monitoring)
     */
    size_t size() const;
};

} // namespace ui
