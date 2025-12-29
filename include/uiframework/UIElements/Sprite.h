#pragma once

#include "UIElement.h"
#include <SDL2/SDL.h>
#include <string>
#include <vector>

namespace ui {

class Sprite : public UIElement {
private:
    std::string filePath;
    std::vector<unsigned char> imageData;
    bool stretch;
    SDL_Texture* texture = nullptr;
    SDL_Rect srcRect = {0, 0, 0, 0}; // Source rectangle on sprite sheet
    bool isDataImage = false;
    
    void loadFromFile(SDL_Renderer* renderer, const std::string& path);
    void loadFromData(SDL_Renderer* renderer, const unsigned char* data, size_t dataSize);
    void cleanup();

public:
    int naturalWidth = 0;
    int naturalHeight = 0;
    
    // Constructor: loads the sprite sheet from a file
    Sprite(int x_, int y_, int w_, int h_, const std::string &path, bool stretch);

    // Constructor: loads the sprite sheet from embedded binary data
    Sprite(int x_, int y_, int w_, int h_, const unsigned char* data, size_t dataSize, bool stretch);

    virtual ~Sprite();
    
    // Non-copyable but movable
    Sprite(const Sprite&) = delete;
    Sprite& operator=(const Sprite&) = delete;
    Sprite(Sprite&& other) noexcept;
    Sprite& operator=(Sprite&& other) noexcept;

    void render(SDL_Renderer* renderer, TTF_Font* font, std::shared_ptr<class Theme> theme) override;
    bool isInteractive() const override { return false; }

    // Sprite-specific methods
    void setSourceRect(const SDL_Rect &rect);
    SDL_Rect getSourceRect() const { return srcRect; }
    void setSourceRect(int x, int y, int w, int h) { setSourceRect({x, y, w, h}); }
    
    void setStretch(bool shouldStretch) { stretch = shouldStretch; }
    bool isStretched() const { return stretch; }
    bool isLoaded() const { return texture != nullptr; }
    
    // Reload the sprite (useful if file changed)
    void reload(SDL_Renderer* renderer);
    
    // Set new sprite source
    void setSpritePath(SDL_Renderer* renderer, const std::string& path);
    void setSpriteData(SDL_Renderer* renderer, const unsigned char* data, size_t dataSize);
    
    // Utility methods for sprite sheets
    void setFrame(int frameIndex, int frameWidth, int frameHeight, int framesPerRow = 0);
    void setFrameGrid(int col, int row, int frameWidth, int frameHeight);
};

} // namespace ui
