#pragma once
#include <atomic>
#include <mutex>

namespace ui {

/**
 * @brief RAII wrapper for TTF initialization
 * 
 * Manages TTF_Init/TTF_Quit lifecycle with reference counting
 * to ensure proper cleanup even with multiple instances.
 */
class TTFManager {
public:
    TTFManager();
    ~TTFManager();
    
    // Non-copyable, non-movable for safety
    TTFManager(const TTFManager&) = delete;
    TTFManager& operator=(const TTFManager&) = delete;
    TTFManager(TTFManager&&) = delete;
    TTFManager& operator=(TTFManager&&) = delete;
    
    bool isInitialized() const { return initialized; }

private:
    bool initialized = false;
    static std::atomic<int> refCount;
    static std::mutex initMutex;
};

} // namespace ui
