#pragma once

#include "UIElement.h"
#include <SDL2/SDL.h>
#include <string>

namespace ui {

class Sprite : public UIElement {
public:
    int naturalWidth;
    int naturalHeight;
    
    // Constructor: loads the sprite sheet from a file.
    // 'stretch' controls whether the sprite is rendered using the given w/h or its natural dimensions.
    Sprite(int x_, int y_, int w_, int h_, const std::string &path, SDL_Renderer* renderer, bool stretch);

    // Constructor: loads the sprite sheet from embedded binary data.
    // 'data' is the pointer to the binary image data and 'dataSize' its size.
    Sprite(int x_, int y_, int w_, int h_, const unsigned char* data, size_t dataSize, SDL_Renderer* renderer, bool stretch);

    virtual ~Sprite();

    // Renders the sprite by drawing the section defined by srcRect into the destination rectangle.
    virtual void render(SDL_Renderer* renderer) override;

    // Sets the source rectangle (the section of the sprite sheet to render).
    void setSourceRect(const SDL_Rect &rect);

    // Returns the current source rectangle.
    const SDL_Rect& getSourceRect() const;

private:
    std::string filePath;  // For file-based loading (empty for binary loading)
    SDL_Texture* texture;
    SDL_Rect srcRect;      // The portion of the sprite sheet to render.
    bool stretch;          // If false, the natural dimensions are used.
};

} // namespace ui
