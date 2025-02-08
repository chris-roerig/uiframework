#include "Image.h"
#include <SDL2/SDL_image.h>
#include <iostream>

namespace ui {

// Constructor loading from a file.
Image::Image(int x_, int y_, int w_, int h_, const std::string &path, SDL_Renderer* renderer, bool stretch_)
    : UIElement(x_, y_, w_, h_), filePath(path), stretch(stretch_), texture(nullptr), naturalWidth(0), naturalHeight(0)
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

    if (!stretch) {
        width = naturalWidth;
        height = naturalHeight;
    }
}

// Constructor loading from binary data (embedded image).
Image::Image(int x_, int y_, int w_, int h_, const unsigned char* data, size_t dataSize, SDL_Renderer* renderer, bool stretch_)
    : UIElement(x_, y_, w_, h_), stretch(stretch_), texture(nullptr), naturalWidth(0), naturalHeight(0)
{
    SDL_RWops* rw = SDL_RWFromConstMem(data, static_cast<int>(dataSize));
    if (!rw) {
        std::cerr << "SDL_RWFromConstMem error: " << SDL_GetError() << std::endl;
        return;
    }
    // IMG_Load_RW will free the SDL_RWops if the second parameter is non-zero.
    SDL_Surface* surface = IMG_Load_RW(rw, 1);
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
}

Image::~Image() {
    if (texture) {
        SDL_DestroyTexture(texture);
    }
}

void Image::render(SDL_Renderer* renderer) {
    if (!texture) return;
    
    SDL_Rect destRect = { x, y, stretch ? width : naturalWidth, stretch ? height : naturalHeight };
    SDL_RenderCopy(renderer, texture, nullptr, &destRect);
    
    if (hasFocus) {
        SDL_Rect focusRect = getFocusRect();
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderDrawRect(renderer, &focusRect);
    }
}

bool Image::isInteractive() const {
    return false;
}

} // namespace ui
