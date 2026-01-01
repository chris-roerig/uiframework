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

// Phase 1: Constructor with cache initialization
FontManager::FontManager() 
    : maxFontCacheSize(DEFAULT_MAX_FONT_CACHE_SIZE), totalTexturesCreated(0) {
}

// Legacy method for backward compatibility
TTF_Font* FontManager::getFont(const std::string& path, int size) {
    std::lock_guard<std::mutex> lock(cacheMutex);
    
    FontKey key{path, size, FontStyle::Regular};
    auto it = fontCache.find(key);
    
    if (it != fontCache.end()) {
        // Phase 1: Update LRU on cache hit
        updateLRU(key);
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
            // Phase 1: Check cache size before adding
            if (fontCache.size() >= maxFontCacheSize) {
                evictOldestFont();
            }
            fontCache[key] = font;
            updateLRU(key);
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
        // Phase 1: Check cache size before adding
        if (fontCache.size() >= maxFontCacheSize) {
            evictOldestFont();
        }
        fontCache[key] = font;
        updateLRU(key);
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
        // Phase 1: Update LRU on cache hit
        updateLRU(key);
        return it->second;
    }
    
    // Try to load from embedded fonts first
    TTF_Font* font = loadEmbeddedFont(familyName, size, style);
    
    if (font) {
        // Phase 1: Check cache size before adding
        if (fontCache.size() >= maxFontCacheSize) {
            evictOldestFont();
        }
        fontCache[key] = font;
        updateLRU(key);
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
    
    // Add embedded fonts
    for (const auto& family : embeddedFonts) {
        families.push_back(family.first);
    }
    
    // Phase 4: Add external fonts
    for (const auto& family : externalFonts) {
        // Only add if not already in the list (avoid duplicates)
        if (std::find(families.begin(), families.end(), family.first) == families.end()) {
            families.push_back(family.first);
        }
    }
    
    return families;
}

bool FontManager::isFamilyAvailable(const std::string& familyName) const {
    std::lock_guard<std::mutex> lock(cacheMutex);
    return embeddedFonts.find(familyName) != embeddedFonts.end() ||
           externalFonts.find(familyName) != externalFonts.end();
}

TTF_Font* FontManager::loadEmbeddedFont(int size) {
    // Legacy method - use console font as default
    return loadEmbeddedFont("Console", size, FontStyle::Regular);
}

TTF_Font* FontManager::loadEmbeddedFont(const std::string& familyName, int size, FontStyle style) {
    // First try embedded fonts
    auto familyIt = embeddedFonts.find(familyName);
    if (familyIt != embeddedFonts.end()) {
        auto styleIt = familyIt->second.find(style);
        if (styleIt == familyIt->second.end()) {
            // Fallback to regular style if requested style not available
            styleIt = familyIt->second.find(FontStyle::Regular);
        }
        
        if (styleIt != familyIt->second.end()) {
            const EmbeddedFontData& fontData = styleIt->second;
            
            SDL_RWops* rw = SDL_RWFromConstMem(fontData.data, fontData.size);
            if (rw) {
                TTF_Font* font = TTF_OpenFontRW(rw, 1, size); // 1 = free RWops automatically
                if (font) {
                    return font;
                } else {
                    Logger::log(LogLevel::ERROR, "Failed to load embedded font " + familyName + ": " + std::string(TTF_GetError()));
                }
            } else {
                Logger::log(LogLevel::ERROR, "Failed to create RWops from embedded font: " + familyName);
            }
        }
    }
    
    // Phase 4: Try external fonts if embedded font not found
    auto externalFamilyIt = externalFonts.find(familyName);
    if (externalFamilyIt != externalFonts.end()) {
        auto externalStyleIt = externalFamilyIt->second.find(style);
        if (externalStyleIt == externalFamilyIt->second.end()) {
            // Fallback to regular style if requested style not available
            externalStyleIt = externalFamilyIt->second.find(FontStyle::Regular);
        }
        
        if (externalStyleIt != externalFamilyIt->second.end()) {
            const std::string& filePath = externalStyleIt->second;
            TTF_Font* font = TTF_OpenFont(filePath.c_str(), size);
            if (font) {
                return font;
            } else {
                Logger::log(LogLevel::ERROR, "Failed to load external font " + familyName + " from " + filePath + ": " + std::string(TTF_GetError()));
            }
        }
    }
    
    return nullptr;
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

// Phase 1: Cache management methods
void FontManager::evictOldestFont() {
    if (fontLRU.empty()) {
        return;
    }
    
    // Remove the least recently used font
    FontKey oldestKey = fontLRU.back();
    fontLRU.pop_back();
    
    auto it = fontCache.find(oldestKey);
    if (it != fontCache.end()) {
        TTF_CloseFont(it->second);
        fontCache.erase(it);
        Logger::log(LogLevel::DEBUG, "Evicted font from cache: " + oldestKey.familyName + 
                   " (size " + std::to_string(oldestKey.size) + ")");
    }
}

void FontManager::updateLRU(const FontKey& key) {
    // Remove key from current position in LRU list
    auto it = std::find(fontLRU.begin(), fontLRU.end(), key);
    if (it != fontLRU.end()) {
        fontLRU.erase(it);
    }
    
    // Add key to front (most recently used)
    fontLRU.push_front(key);
}

size_t FontManager::estimateFontMemoryUsage(TTF_Font* font) const {
    if (!font) {
        return 0;
    }
    
    // Rough estimate: base font structure + glyph cache
    // TTF_Font structure is typically ~1KB + glyph cache varies by usage
    // We'll estimate 4KB per font as a reasonable baseline
    return 4096;
}

void FontManager::setMaxCacheSize(size_t maxFonts) {
    std::lock_guard<std::mutex> lock(cacheMutex);
    maxFontCacheSize = maxFonts;
    
    // Evict fonts if current cache exceeds new limit
    while (fontCache.size() > maxFontCacheSize && !fontLRU.empty()) {
        evictOldestFont();
    }
}

size_t FontManager::getCacheSize() const {
    std::lock_guard<std::mutex> lock(cacheMutex);
    return fontCache.size();
}

size_t FontManager::getMaxCacheSize() const {
    std::lock_guard<std::mutex> lock(cacheMutex);
    return maxFontCacheSize;
}

FontMemoryStats FontManager::getMemoryStats() const {
    std::lock_guard<std::mutex> lock(cacheMutex);
    
    FontMemoryStats stats;
    stats.totalFontsLoaded = fontCache.size();
    stats.totalTexturesCreated = totalTexturesCreated;
    stats.maxFontCacheSize = maxFontCacheSize;
    stats.currentFontCacheSize = fontCache.size();
    
    // Estimate memory usage
    stats.estimatedMemoryUsage = 0;
    for (const auto& pair : fontCache) {
        stats.estimatedMemoryUsage += estimateFontMemoryUsage(pair.second);
    }
    
    return stats;
}

void FontManager::logMemoryUsage() const {
    FontMemoryStats stats = getMemoryStats();
    
    Logger::log(LogLevel::INFO, "Font Memory Usage:");
    Logger::log(LogLevel::INFO, "  Fonts loaded: " + std::to_string(stats.totalFontsLoaded) + 
               "/" + std::to_string(stats.maxFontCacheSize));
    Logger::log(LogLevel::INFO, "  Estimated memory: " + std::to_string(stats.estimatedMemoryUsage / 1024) + " KB");
    Logger::log(LogLevel::INFO, "  Textures created: " + std::to_string(stats.totalTexturesCreated));
}

// Phase 3: Font metrics and text measurement methods
FontMetrics FontManager::getFontMetrics(const std::string& familyName, int size, FontStyle style) {
    TTF_Font* font = getFont(familyName, size, style);
    if (!font) {
        return FontMetrics(); // Return empty metrics if font not found
    }
    
    int ascent = TTF_FontAscent(font);
    int descent = TTF_FontDescent(font);
    int lineSkip = TTF_FontLineSkip(font);
    int height = TTF_FontHeight(font);
    
    return FontMetrics(ascent, descent, lineSkip, height);
}

int FontManager::getTextWidth(const std::string& text, const std::string& familyName, int size, FontStyle style) {
    TTF_Font* font = getFont(familyName, size, style);
    if (!font || text.empty()) {
        return 0;
    }
    
    int width = 0;
    TTF_SizeText(font, text.c_str(), &width, nullptr);
    return width;
}

int FontManager::getTextHeight(const std::string& text, const std::string& familyName, int size, FontStyle style) {
    TTF_Font* font = getFont(familyName, size, style);
    if (!font || text.empty()) {
        return 0;
    }
    
    int height = 0;
    TTF_SizeText(font, text.c_str(), nullptr, &height);
    return height;
}

std::pair<int, int> FontManager::getTextSize(const std::string& text, const std::string& familyName, int size, FontStyle style) {
    TTF_Font* font = getFont(familyName, size, style);
    if (!font || text.empty()) {
        return {0, 0};
    }
    
    int width = 0, height = 0;
    TTF_SizeText(font, text.c_str(), &width, &height);
    return {width, height};
}

// Phase 4: Runtime font registration methods
bool FontManager::registerFontFromMemory(const std::string& familyName, FontStyle style,
                                        const unsigned char* data, size_t dataSize) {
    if (!data || dataSize == 0 || familyName.empty()) {
        return false;
    }
    
    std::lock_guard<std::mutex> lock(cacheMutex);
    
    // Register as embedded font (reuse existing embedded font system)
    EmbeddedFontData fontData;
    fontData.data = data;
    fontData.size = dataSize;
    
    embeddedFonts[familyName][style] = fontData;
    
    return true;
}

bool FontManager::registerFontFromFile(const std::string& familyName, FontStyle style,
                                      const std::string& filePath) {
    if (familyName.empty() || filePath.empty()) {
        return false;
    }
    
    // Test if file can be loaded
    TTF_Font* testFont = TTF_OpenFont(filePath.c_str(), 12);
    if (!testFont) {
        std::cerr << "FontManager: Cannot load font file: " << filePath << " - " << TTF_GetError() << std::endl;
        return false;
    }
    TTF_CloseFont(testFont);
    
    std::lock_guard<std::mutex> lock(cacheMutex);
    
    // Register external font path
    externalFonts[familyName][style] = filePath;
    
    return true;
}

void FontManager::unregisterFontFamily(const std::string& familyName) {
    if (familyName.empty()) {
        return;
    }
    
    std::lock_guard<std::mutex> lock(cacheMutex);
    
    // Remove from embedded fonts
    embeddedFonts.erase(familyName);
    
    // Remove from external fonts
    externalFonts.erase(familyName);
    
    // Remove cached fonts for this family
    auto it = fontCache.begin();
    while (it != fontCache.end()) {
        if (it->first.familyName == familyName) {
            TTF_CloseFont(it->second);
            
            // Remove from LRU list
            fontLRU.remove(it->first);
            
            it = fontCache.erase(it);
        } else {
            ++it;
        }
    }
}

} // namespace ui
