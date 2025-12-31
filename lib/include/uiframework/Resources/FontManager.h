#pragma once
#include <SDL2/SDL_ttf.h>
#include <string>
#include <unordered_map>
#include <mutex>
#include <memory>
#include <vector>

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

class FontManager {
private:
    static std::unique_ptr<FontManager> instance;
    static std::mutex instanceMutex;
    static std::once_flag initialized;
    
    std::unordered_map<FontKey, TTF_Font*, FontKeyHash> fontCache;
    mutable std::mutex cacheMutex;
    std::unique_ptr<TTFManager> ttfManager;
    
    // Phase 0: Embedded font registry
    std::unordered_map<std::string, std::unordered_map<FontStyle, EmbeddedFontData>> embeddedFonts;
    
    FontManager() = default;
    
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
