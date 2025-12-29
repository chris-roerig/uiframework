#include "uiframework/Resources/TTFManager.h"
#include "uiframework/Logger.h"
#include <SDL2/SDL_ttf.h>

namespace ui {

std::atomic<int> TTFManager::refCount{0};
std::mutex TTFManager::initMutex;

TTFManager::TTFManager() {
    try {
        std::lock_guard<std::mutex> lock(initMutex);
        
        if (refCount.fetch_add(1) == 0) {
            if (TTF_Init() == -1) {
                refCount.fetch_sub(1);
                Logger::log(LogLevel::ERROR, "TTF_Init failed: " + std::string(TTF_GetError()));
                initialized = false;
                return;
            }
            Logger::log(LogLevel::DEBUG, "TTF initialized");
        }
        
        initialized = true;
    } catch (...) {
        initialized = false;
    }
}

TTFManager::~TTFManager() {
    if (initialized) {
        try {
            std::lock_guard<std::mutex> lock(initMutex);
            
            if (refCount.fetch_sub(1) == 1) {
                TTF_Quit();
                Logger::log(LogLevel::DEBUG, "TTF shutdown");
            }
        } catch (...) {
            // Ignore errors during shutdown
        }
        initialized = false;
    }
}

} // namespace ui
