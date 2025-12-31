#include "uiframework/Resources/FontManager.h"
#include "uiframework/Resources/EmbeddedFont.h"
#include "uiframework/Resources/EmbeddedFontRegistry.h"
#include "uiframework/Resources/FontTheme.h"
#include "uiframework/Resources/TTFManager.h"
#include "uiframework/Logger.h"
#include <SDL2/SDL_rwops.h>
#include <algorithm>

namespace ui {

std::unique_ptr<FontManager> FontManager::instance = nullptr;
std::mutex FontManager::instanceMutex;
std::once_flag FontManager::initialized;

// Current theme - defaults to framework theme
static std::unique_ptr<FontTheme> currentTheme = std::make_unique<DefaultFontTheme>();

// FontKey implementation for Phase 0
bool FontKey::operator==(const FontKey& other) const {
    return familyName == other.familyName && size == other.size && style == other.style;
}

std::size_t FontKeyHash::operator()(const FontKey& key) const {
    return std::hash<std::string>{}(key.familyName) ^ 
           (std::hash<int>{}(key.size) << 1) ^
           (std::hash<int>{}(static_cast<int>(key.style)) << 2);
}

FontManager& FontManager::getInstance() {
    std::call_once(initialized, []() {
        std::lock_guard<std::mutex> lock(instanceMutex);
        if (!instance) {
            instance = std::unique_ptr<FontManager>(new FontManager());
        }
    });
    return *instance;
}

// Legacy method for backward compatibility
TTF_Font* FontManager::getFont(const std::string& path, int size) {
    std::lock_guard<std::mutex> lock(cacheMutex);
    
    FontKey key{path, size, FontStyle::Regular};
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

// Phase 0: New multi-font method
TTF_Font* FontManager::getFont(const std::string& familyName, int size, FontStyle style) {
    std::lock_guard<std::mutex> lock(cacheMutex);
    
    FontKey key{familyName, size, style};
    auto it = fontCache.find(key);
    
    if (it != fontCache.end()) {
        return it->second;
    }
    
    // Try to load from embedded fonts first
    TTF_Font* font = loadEmbeddedFont(familyName, size, style);
    
    if (font) {
        fontCache[key] = font;
        Logger::log(LogLevel::INFO, "Loaded embedded font: " + familyName + " (size " + std::to_string(size) + ")");
    } else {
        Logger::log(LogLevel::WARNING, "Failed to load font family: " + familyName + " (size " + std::to_string(size) + ")");
    }
    
    return font;
}

// Phase 0: Theme-based font selection
TTF_Font* FontManager::getThemeFont(FontType type, int size, FontStyle style) {
    std::string familyName;
    
    switch (type) {
        case FontType::Primary:
            familyName = currentTheme->getPrimaryFontFamily();
            break;
        case FontType::Monospace:
            familyName = currentTheme->getMonospaceFontFamily();
            break;
        case FontType::UI:
            familyName = currentTheme->getUIFontFamily();
            break;
    }
    
    return getFont(familyName, size, style);
}

// Phase 0: Embedded font registration
void FontManager::registerEmbeddedFont(const std::string& familyName, FontStyle style,
                                     const unsigned char* data, size_t dataSize) {
    std::lock_guard<std::mutex> lock(cacheMutex);
    embeddedFonts[familyName][style] = EmbeddedFontData(data, dataSize);
}

// Phase 0: Font family management
std::vector<std::string> FontManager::getAvailableFamilies() const {
    std::lock_guard<std::mutex> lock(cacheMutex);
    std::vector<std::string> families;
    
    for (const auto& family : embeddedFonts) {
        families.push_back(family.first);
    }
    
    return families;
}

bool FontManager::isFamilyAvailable(const std::string& familyName) const {
    std::lock_guard<std::mutex> lock(cacheMutex);
    return embeddedFonts.find(familyName) != embeddedFonts.end();
}

TTF_Font* FontManager::loadEmbeddedFont(int size) {
    // Legacy method - use console font as default
    return loadEmbeddedFont("Console", size, FontStyle::Regular);
}

TTF_Font* FontManager::loadEmbeddedFont(const std::string& familyName, int size, FontStyle style) {
    // Find the embedded font data
    auto familyIt = embeddedFonts.find(familyName);
    if (familyIt == embeddedFonts.end()) {
        return nullptr;
    }
    
    auto styleIt = familyIt->second.find(style);
    if (styleIt == familyIt->second.end()) {
        // Fallback to regular style if requested style not available
        styleIt = familyIt->second.find(FontStyle::Regular);
        if (styleIt == familyIt->second.end()) {
            return nullptr;
        }
    }
    
    const EmbeddedFontData& fontData = styleIt->second;
    
    SDL_RWops* rw = SDL_RWFromConstMem(fontData.data, fontData.size);
    if (!rw) {
        Logger::log(LogLevel::ERROR, "Failed to create RWops from embedded font: " + familyName);
        return nullptr;
    }
    
    TTF_Font* font = TTF_OpenFontRW(rw, 1, size); // 1 = free RWops automatically
    if (!font) {
        Logger::log(LogLevel::ERROR, "Failed to load embedded font " + familyName + ": " + std::string(TTF_GetError()));
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
