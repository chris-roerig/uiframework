#pragma once
#include <SDL2/SDL_ttf.h>
#include <string>
#include <unordered_map>
#include <mutex>
#include <memory>
#include <vector>
#include <list>

namespace ui {

// Forward declarations
class TTFManager;

enum class FontStyle {
    Regular = 0,
    Bold = 1,
    Italic = 2,
    BoldItalic = 3
};

enum class FontType {
    Primary,    // Main UI font
    Monospace,  // Code/console font
    UI          // Interface elements
};

struct FontKey {
    std::string familyName;  // Changed from path to familyName for Phase 0
    int size;
    FontStyle style;
    
    bool operator==(const FontKey& other) const;
};

struct FontKeyHash {
    std::size_t operator()(const FontKey& key) const;
};

// Embedded font data structure
struct EmbeddedFontData {
    const unsigned char* data;
    size_t size;
    
    EmbeddedFontData() : data(nullptr), size(0) {}
    EmbeddedFontData(const unsigned char* fontData, size_t fontSize) 
        : data(fontData), size(fontSize) {}
};

// Phase 1: Memory usage statistics
struct FontMemoryStats {
    size_t totalFontsLoaded;
    size_t totalTexturesCreated;
    size_t estimatedMemoryUsage;
    size_t maxFontCacheSize;
    size_t currentFontCacheSize;
};

// Phase 3: Font metrics structure
struct FontMetrics {
    int ascent;      // Distance from baseline to top
    int descent;     // Distance from baseline to bottom (negative)
    int lineSkip;    // Recommended line spacing
    int height;      // Total font height (ascent - descent)
    
    FontMetrics() : ascent(0), descent(0), lineSkip(0), height(0) {}
    FontMetrics(int a, int d, int ls, int h) : ascent(a), descent(d), lineSkip(ls), height(h) {}
};

class FontManager {
private:
    static std::unique_ptr<FontManager> instance;
    static std::mutex instanceMutex;
    static std::once_flag initialized;
    
    // Phase 1: Cache size limits
    static constexpr size_t MAX_FONT_CACHE_SIZE = 50;
    static constexpr size_t DEFAULT_MAX_FONT_CACHE_SIZE = 50;
    
    std::unordered_map<FontKey, TTF_Font*, FontKeyHash> fontCache;
    std::list<FontKey> fontLRU; // Phase 1: LRU tracking for cache eviction
    mutable std::mutex cacheMutex;
    std::unique_ptr<TTFManager> ttfManager;
    
    // Phase 0: Embedded font registry
    std::unordered_map<std::string, std::unordered_map<FontStyle, EmbeddedFontData>> embeddedFonts;
    
    // Phase 4: External font registry
    std::unordered_map<std::string, std::unordered_map<FontStyle, std::string>> externalFonts;
    
    // Phase 1: Cache management
    size_t maxFontCacheSize;
    size_t totalTexturesCreated; // Track texture creation for memory stats
    
    FontManager();
    
    // Phase 1: Cache management methods
    void evictOldestFont();
    void updateLRU(const FontKey& key);
    size_t estimateFontMemoryUsage(TTF_Font* font) const;
    
public:
    static FontManager& getInstance();
    
    // Legacy method for backward compatibility
    TTF_Font* getFont(const std::string& path, int size);
    
    // Phase 0: New multi-font methods
    TTF_Font* getFont(const std::string& familyName, int size, FontStyle style);
    TTF_Font* getThemeFont(FontType type, int size, FontStyle style = FontStyle::Regular);
    
    // Phase 0: Embedded font registration
    void registerEmbeddedFont(const std::string& familyName, FontStyle style,
                             const unsigned char* data, size_t dataSize);
    
    // Phase 0: Font family management
    std::vector<std::string> getAvailableFamilies() const;
    bool isFamilyAvailable(const std::string& familyName) const;
    
    // Phase 1: Cache management and monitoring
    void setMaxCacheSize(size_t maxFonts);
    size_t getCacheSize() const;
    size_t getMaxCacheSize() const;
    FontMemoryStats getMemoryStats() const;
    void logMemoryUsage() const;
    
    // Phase 3: Font metrics and text measurement
    FontMetrics getFontMetrics(const std::string& familyName, int size, FontStyle style = FontStyle::Regular);
    int getTextWidth(const std::string& text, const std::string& familyName, int size, FontStyle style = FontStyle::Regular);
    int getTextHeight(const std::string& text, const std::string& familyName, int size, FontStyle style = FontStyle::Regular);
    std::pair<int, int> getTextSize(const std::string& text, const std::string& familyName, int size, FontStyle style = FontStyle::Regular);
    
    // Phase 4: Runtime font registration
    bool registerFontFromMemory(const std::string& familyName, FontStyle style,
                               const unsigned char* data, size_t dataSize);
    bool registerFontFromFile(const std::string& familyName, FontStyle style,
                             const std::string& filePath);
    void unregisterFontFamily(const std::string& familyName);
    
    TTF_Font* loadEmbeddedFont(int size);
    TTF_Font* loadEmbeddedFont(const std::string& familyName, int size, FontStyle style);
    void cleanup();
    
    ~FontManager();
    
    // Non-copyable, non-movable
    FontManager(const FontManager&) = delete;
    FontManager& operator=(const FontManager&) = delete;
    FontManager(FontManager&&) = delete;
    FontManager& operator=(FontManager&&) = delete;
};

} // namespace ui
