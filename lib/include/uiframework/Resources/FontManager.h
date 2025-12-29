#pragma once
#include <SDL2/SDL_ttf.h>
#include <string>
#include <unordered_map>
#include <mutex>
#include <memory>

namespace ui {

// Forward declaration
class TTFManager;

struct FontKey {
    std::string path;
    int size;
    
    bool operator==(const FontKey& other) const {
        return path == other.path && size == other.size;
    }
};

struct FontKeyHash {
    std::size_t operator()(const FontKey& key) const {
        return std::hash<std::string>{}(key.path) ^ (std::hash<int>{}(key.size) << 1);
    }
};

class FontManager {
private:
    static std::unique_ptr<FontManager> instance;
    static std::mutex instanceMutex;
    static std::once_flag initialized;
    
    std::unordered_map<FontKey, TTF_Font*, FontKeyHash> fontCache;
    mutable std::mutex cacheMutex;
    std::unique_ptr<TTFManager> ttfManager;
    
    FontManager() = default;
    
public:
    static FontManager& getInstance();
    
    TTF_Font* getFont(const std::string& path, int size);
    TTF_Font* loadEmbeddedFont(int size);
    void cleanup();
    
    ~FontManager();
    
    // Non-copyable, non-movable
    FontManager(const FontManager&) = delete;
    FontManager& operator=(const FontManager&) = delete;
    FontManager(FontManager&&) = delete;
    FontManager& operator=(FontManager&&) = delete;
};

} // namespace ui
