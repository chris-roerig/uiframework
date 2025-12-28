#pragma once

#include "UIElement.h"
#include <SDL2/SDL.h>
#include <string>
#include <memory>

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
    
    void loadFromFile(SDL_Renderer* renderer, const std::string& path);
    void loadFromData(SDL_Renderer* renderer, const unsigned char* data, size_t dataSize);
    void cleanup();

public:
    // Constructor for loading from file path
    Image(int x_, int y_, int w_, int h_, const std::string &path, bool stretch_ = false);
    
    // Constructor for loading from binary data
    Image(int x_, int y_, int w_, int h_, const unsigned char* data, size_t dataSize, bool stretch_ = false);

    virtual ~Image();
    
    // Non-copyable but movable
    Image(const Image&) = delete;
    Image& operator=(const Image&) = delete;
    Image(Image&& other) noexcept;
    Image& operator=(Image&& other) noexcept;

    void render(SDL_Renderer* renderer, TTF_Font* font, std::shared_ptr<class Theme> theme) override;
    bool isInteractive() const override { return false; }
    
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
};

} // namespace ui
