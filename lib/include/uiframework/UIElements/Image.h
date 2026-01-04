#pragma once

#include "UIElement.h"
#include <SDL2/SDL.h>
#include <string>
#include <memory>
#include <mutex>

namespace ui {

class Image : public UIElement {
private:
    std::string filePath;
    std::vector<unsigned char> imageData; // For embedded images
    bool stretch;
    SDL_Texture* texture = nullptr;
    int naturalWidth = 0;
    int naturalHeight = 0;
    bool isDataImage = false; // true if loaded from data, false if from file
    mutable std::mutex textureMutex; // Protects texture operations
    
    // Real-time queued loading
    std::string queuedImagePath;
    std::atomic<bool> hasQueuedPath{false};
    
    void loadFromFile(SDL_Renderer* renderer, const std::string& path);
    void loadFromData(SDL_Renderer* renderer, const unsigned char* data, size_t dataSize);
    void cleanup();

public:
    // Constructor for loading from file path
    Image(int x_, int y_, int w_, int h_, const std::string &path, bool stretch_ = false);
    
    // Constructor for loading from binary data
    Image(int x_, int y_, int w_, int h_, const unsigned char* data, size_t dataSize, bool stretch_ = false);

    virtual ~Image() override;
    
    // Non-copyable but movable
    Image(const Image&) = delete;
    Image& operator=(const Image&) = delete;
    Image(Image&& other) noexcept;
    Image& operator=(Image&& other) noexcept;

protected:
    void renderImpl(const RenderContext& ctx) override;
    
public:
    // Image-specific methods
    void setStretch(bool shouldStretch) { stretch = shouldStretch; }
    bool isStretched() const { return stretch; }
    int getNaturalWidth() const { return naturalWidth; }
    int getNaturalHeight() const { return naturalHeight; }
    bool isLoaded() const { return texture != nullptr; }
    
    // Reload the image (useful if file changed)
    void reload(SDL_Renderer* renderer);
    
    // Set new image source
    void setImagePath(SDL_Renderer* renderer, const std::string& path);
    void setImageData(SDL_Renderer* renderer, const unsigned char* data, size_t dataSize);
    
    // Real-time safe methods (lock-free, audio thread safe)
    void realtimeSetImagePath(const std::string& path);
};

} // namespace ui
