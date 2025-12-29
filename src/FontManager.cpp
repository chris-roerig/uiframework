#include "uiframework/Resources/FontManager.h"
#include "uiframework/Resources/EmbeddedFont.h"
#include "uiframework/Resources/TTFManager.h"
#include "uiframework/Logger.h"
#include <SDL2/SDL_rwops.h>

namespace ui {

std::unique_ptr<FontManager> FontManager::instance = nullptr;
std::mutex FontManager::instanceMutex;
std::once_flag FontManager::initialized;

FontManager& FontManager::getInstance() {
    std::call_once(initialized, []() {
        std::lock_guard<std::mutex> lock(instanceMutex);
        if (!instance) {
            instance = std::unique_ptr<FontManager>(new FontManager());
        }
    });
    return *instance;
}

TTF_Font* FontManager::getFont(const std::string& path, int size) {
    std::lock_guard<std::mutex> lock(cacheMutex);
    
    FontKey key{path, size};
    auto it = fontCache.find(key);
    
    if (it != fontCache.end()) {
        return it->second;
    }
    
    // Ensure TTF is initialized using RAII
    if (!ttfManager) {
        ttfManager = std::make_unique<TTFManager>();
        if (!ttfManager->isInitialized()) {
            return nullptr;
        }
    }
    
    TTF_Font* font = nullptr;
    
    if (path.empty()) {
        // Use embedded font as default
        font = loadEmbeddedFont(size);
        if (font) {
            fontCache[key] = font;
            return font;
        }
        
        // Fallback to system fonts if embedded fails
        const char* defaultFonts[] = {
            "assets/default_font.ttf",
            "Assets/default_font.ttf",
            "../Assets/default_font.ttf",
            "/System/Library/Fonts/Helvetica.ttc",
            "/System/Library/Fonts/Arial.ttf",
            "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",
            "/usr/share/fonts/TTF/DejaVuSans.ttf",
            "C:\\Windows\\Fonts\\arial.ttf"
        };
        
        for (const char* defaultFont : defaultFonts) {
            font = TTF_OpenFont(defaultFont, size);
            if (font) {
                Logger::log(LogLevel::INFO, "Loaded font: " + std::string(defaultFont) + " (size " + std::to_string(size) + ")");
                break;
            }
        }
    } else {
        font = TTF_OpenFont(path.c_str(), size);
        if (font) {
            Logger::log(LogLevel::INFO, "Loaded font: " + path + " (size " + std::to_string(size) + ")");
        }
    }
    
    if (font) {
        fontCache[key] = font;
    } else {
        Logger::log(LogLevel::ERROR, "Failed to load font: " + (path.empty() ? "default" : path));
    }
    
    return font;
}

TTF_Font* FontManager::loadEmbeddedFont(int size) {
    SDL_RWops* rw = SDL_RWFromConstMem(embedded::console_font_data, embedded::console_font_size);
    if (!rw) {
        Logger::log(LogLevel::ERROR, "Failed to create RWops from embedded font");
        return nullptr;
    }
    
    TTF_Font* font = TTF_OpenFontRW(rw, 1, size); // 1 = free RWops automatically
    if (!font) {
        Logger::log(LogLevel::ERROR, "Failed to load embedded font: " + std::string(TTF_GetError()));
        return nullptr;
    }
    
    return font;
}

void FontManager::cleanup() {
    std::lock_guard<std::mutex> lock(cacheMutex);
    for (auto& pair : fontCache) {
        if (pair.second) {
            TTF_CloseFont(pair.second);
        }
    }
    fontCache.clear();
    
    // TTF cleanup is handled by TTFManager destructor
}

FontManager::~FontManager() {
    cleanup();
}

} // namespace ui
