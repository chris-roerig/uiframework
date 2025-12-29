#include "uiframework/Resources/FontManager.h"
#include "uiframework/Resources/EmbeddedFont.h"
#include <iostream>
#include <SDL2/SDL_rwops.h>
#include <atomic>

namespace ui {

std::unique_ptr<FontManager> FontManager::instance = nullptr;
std::mutex FontManager::instanceMutex;
static std::atomic<int> ttfRefCount{0};

FontManager& FontManager::getInstance() {
    std::lock_guard<std::mutex> lock(instanceMutex);
    if (!instance) {
        instance = std::unique_ptr<FontManager>(new FontManager());
    }
    return *instance;
}

TTF_Font* FontManager::getFont(const std::string& path, int size) {
    std::lock_guard<std::mutex> lock(cacheMutex);
    
    FontKey key{path, size};
    auto it = fontCache.find(key);
    
    if (it != fontCache.end()) {
        return it->second;
    }
    
    // Ensure TTF is initialized
    if (ttfRefCount.fetch_add(1) == 0) {
        if (TTF_Init() == -1) {
            ttfRefCount.fetch_sub(1);
            std::cerr << "FontManager: TTF_Init failed: " << TTF_GetError() << std::endl;
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
                std::cout << "FontManager: Loaded font: " << defaultFont << " (size " << size << ")" << std::endl;
                break;
            }
        }
    } else {
        font = TTF_OpenFont(path.c_str(), size);
        if (font) {
            std::cout << "FontManager: Loaded font: " << path << " (size " << size << ")" << std::endl;
        }
    }
    
    if (font) {
        fontCache[key] = font;
    } else {
        std::cerr << "FontManager: Failed to load font: " << (path.empty() ? "default" : path) << std::endl;
        ttfRefCount.fetch_sub(1);
    }
    
    return font;
}

TTF_Font* FontManager::loadEmbeddedFont(int size) {
    SDL_RWops* rw = SDL_RWFromConstMem(embedded::console_font_data, embedded::console_font_size);
    if (!rw) {
        std::cerr << "FontManager: Failed to create RWops from embedded font" << std::endl;
        return nullptr;
    }
    
    TTF_Font* font = TTF_OpenFontRW(rw, 1, size); // 1 = free RWops automatically
    if (!font) {
        std::cerr << "FontManager: Failed to load embedded font: " << TTF_GetError() << std::endl;
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
    
    // Clean up TTF if we were the last user
    if (ttfRefCount.exchange(0) > 0) {
        TTF_Quit();
    }
}

FontManager::~FontManager() {
    cleanup();
}

} // namespace ui
