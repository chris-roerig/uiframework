#include "Sprite.h"
#include "../Theme/ThemeGlobals.h"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>

namespace ui {

// Constructor: load sprite sheet from a file.
Sprite::Sprite(int x_, int y_, int w_, int h_, const std::string &path, SDL_Renderer* renderer, bool stretch_)
    : UIElement(x_, y_, w_, h_), filePath(path), texture(nullptr), naturalWidth(0), naturalHeight(0), stretch(stretch_)
{
    SDL_Surface* surface = IMG_Load(filePath.c_str());
    if (!surface) {
        std::cerr << "IMG_Load Error: " << IMG_GetError() << " for file: " << filePath << std::endl;
        return;
    }
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        std::cerr << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
    }
    naturalWidth = surface->w;
    naturalHeight = surface->h;
    SDL_FreeSurface(surface);

    // If not stretching, use the natural dimensions.
    if (!stretch) {
        width = naturalWidth;
        height = naturalHeight;
    }

    // Default source rectangle covers the entire image.
    srcRect.x = 0;
    srcRect.y = 0;
    srcRect.w = naturalWidth;
    srcRect.h = naturalHeight;
}

// Constructor: load sprite sheet from embedded binary data.
Sprite::Sprite(int x_, int y_, int w_, int h_, const unsigned char* data, size_t dataSize, SDL_Renderer* renderer, bool stretch_)
    : UIElement(x_, y_, w_, h_), filePath(""), texture(nullptr), naturalWidth(0), naturalHeight(0), stretch(stretch_)
{
    SDL_RWops* rw = SDL_RWFromConstMem(data, static_cast<int>(dataSize));
    if (!rw) {
        std::cerr << "SDL_RWFromConstMem error: " << SDL_GetError() << std::endl;
        return;
    }
    SDL_Surface* surface = IMG_Load_RW(rw, 1); // '1' indicates that SDL will free rw.
    if (!surface) {
        std::cerr << "IMG_Load_RW Error: " << IMG_GetError() << std::endl;
        return;
    }
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        std::cerr << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
    }
    naturalWidth = surface->w;
    naturalHeight = surface->h;
    SDL_FreeSurface(surface);

    if (!stretch) {
        width = naturalWidth;
        height = naturalHeight;
    }

    // Default source rectangle covers the entire image.
    srcRect.x = 0;
    srcRect.y = 0;
    srcRect.w = naturalWidth;
    srcRect.h = naturalHeight;
}

Sprite::~Sprite() {
    if (texture) {
        SDL_DestroyTexture(texture);
    }
}

void Sprite::render(SDL_Renderer* renderer) {
    if (!texture) return;
    
    // Determine destination rectangle:
    // If stretch is true, use the UIElement's width and height,
    // otherwise use the natural dimensions.
    SDL_Rect destRect = { x, y, stretch ? width : naturalWidth, stretch ? height : naturalHeight };
    SDL_RenderCopy(renderer, texture, &srcRect, &destRect);
    
    // Draw a focus rectangle if this element has focus.
    if (hasFocus) {
        SDL_Rect focusRect = getFocusRect();
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red outline for focus.
        SDL_RenderDrawRect(renderer, &focusRect);
    }
}

void Sprite::setSourceRect(const SDL_Rect &rect) {
    srcRect = rect;
}

const SDL_Rect& Sprite::getSourceRect() const {
    return srcRect;
}

} // namespace ui
