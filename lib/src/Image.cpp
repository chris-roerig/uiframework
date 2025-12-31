#include "uiframework/UIElements/Image.h"
#include "uiframework/Theme/ThemeBase.h"
#include "uiframework/Helpers.h"
#include <SDL2/SDL_image.h>
#include <iostream>
#include <algorithm>

namespace ui {

// Constructor loading from a file
Image::Image(int x_, int y_, int w_, int h_, const std::string &path, bool stretch_)
    : UIElement(x_, y_, w_, h_), filePath(path), stretch(stretch_), isDataImage(false) {
    // Texture will be loaded on first render
}

// Constructor loading from binary data
Image::Image(int x_, int y_, int w_, int h_, const unsigned char* data, size_t dataSize, bool stretch_)
    : UIElement(x_, y_, w_, h_), stretch(stretch_), isDataImage(true) {
    if (data && dataSize > 0) {
        imageData.assign(data, data + dataSize);
    }
    // Texture will be loaded on first render
}

Image::~Image() {
    cleanup();
}

Image::Image(Image&& other) noexcept
    : UIElement(std::move(other)),
      filePath(std::move(other.filePath)),
      imageData(std::move(other.imageData)),
      stretch(other.stretch),
      texture(other.texture),
      naturalWidth(other.naturalWidth),
      naturalHeight(other.naturalHeight),
      isDataImage(other.isDataImage) {
    
    other.texture = nullptr;
    other.naturalWidth = 0;
    other.naturalHeight = 0;
}

Image& Image::operator=(Image&& other) noexcept {
    if (this != &other) {
        cleanup();
        
        UIElement::operator=(std::move(other));
        filePath = std::move(other.filePath);
        imageData = std::move(other.imageData);
        stretch = other.stretch;
        texture = other.texture;
        naturalWidth = other.naturalWidth;
        naturalHeight = other.naturalHeight;
        isDataImage = other.isDataImage;
        
        other.texture = nullptr;
        other.naturalWidth = 0;
        other.naturalHeight = 0;
    }
    return *this;
}

void Image::cleanup() {
    if (texture) {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }
}

void Image::loadFromFile(SDL_Renderer* renderer, const std::string& path) {
    if (!renderer || path.empty()) {
        return;
    }
    
    cleanup();
    
    SDL_Surface* surface = IMG_Load(path.c_str());
    if (!surface) {
        std::cerr << "IMG_Load Error: " << IMG_GetError() << " for file: " << path << std::endl;
        return;
    }
    
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        std::cerr << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
        SDL_FreeSurface(surface);
        return;
    }
    
    naturalWidth = surface->w;
    naturalHeight = surface->h;
    SDL_FreeSurface(surface);
    
    // Auto-size if not stretching
    if (!stretch) {
        width = naturalWidth;
        height = naturalHeight;
    }
}

void Image::loadFromData(SDL_Renderer* renderer, const unsigned char* data, size_t dataSize) {
    if (!renderer || !data || dataSize == 0) {
        return;
    }
    
    cleanup();
    
    SDL_RWops* rw = SDL_RWFromConstMem(data, static_cast<int>(dataSize));
    if (!rw) {
        std::cerr << "SDL_RWFromConstMem error: " << SDL_GetError() << std::endl;
        return;
    }
    
    // IMG_Load_RW will free the SDL_RWops if the second parameter is non-zero
    SDL_Surface* surface = IMG_Load_RW(rw, 1);
    if (!surface) {
        std::cerr << "IMG_Load_RW error: " << IMG_GetError() << std::endl;
        return;
    }
    
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        std::cerr << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
        SDL_FreeSurface(surface);
        return;
    }
    
    naturalWidth = surface->w;
    naturalHeight = surface->h;
    SDL_FreeSurface(surface);
    
    // Auto-size if not stretching
    if (!stretch) {
        width = naturalWidth;
        height = naturalHeight;
    }
}

void Image::renderImpl(const RenderContext& ctx) {
    if (!visible) {
        return;
    }
    
    // Load texture if not already loaded
    if (!texture) {
        if (isDataImage && !imageData.empty()) {
            loadFromData(ctx.renderer, imageData.data(), imageData.size());
        } else if (!isDataImage && !filePath.empty()) {
            loadFromFile(ctx.renderer, filePath);
        }
    }
    
    if (!texture) {
        // Draw placeholder rectangle if image failed to load
        if (ctx.theme) {
            auto colors = ctx.theme->buttonColors();
            SDL_Rect rect = { x, y, width, height };
            SDL_SetRenderDrawColor(ctx.renderer, colors.buttonBackground.r, colors.buttonBackground.g, 
                                 colors.buttonBackground.b, colors.buttonBackground.a);
            SDL_RenderFillRect(ctx.renderer, &rect);
            SDL_SetRenderDrawColor(ctx.renderer, colors.buttonText.r, colors.buttonText.g, 
                                 colors.buttonText.b, colors.buttonText.a);
            SDL_RenderDrawRect(ctx.renderer, &rect);
            
            // Draw X to indicate missing image
            SDL_RenderDrawLine(ctx.renderer, x, y, x + width, y + height);
            SDL_RenderDrawLine(ctx.renderer, x + width, y, x, y + height);
        }
        return;
    }
    
    SDL_Rect destRect;
    if (stretch) {
        // Stretch to fit the specified dimensions
        destRect = { x, y, width, height };
    } else {
        // Use natural dimensions, centered if element is larger
        int imgWidth = std::min(width, naturalWidth);
        int imgHeight = std::min(height, naturalHeight);
        destRect = { 
            x + (width - imgWidth) / 2, 
            y + (height - imgHeight) / 2, 
            imgWidth, 
            imgHeight 
        };
    }
    
    SDL_RenderCopy(ctx.renderer, texture, nullptr, &destRect);
    
    // Draw focus indicator if focused
    if (hasFocus && ctx.theme) {
        auto colors = ctx.theme->focusColors();
        SDL_Rect focusRect = getFocusRect();
        SDL_SetRenderDrawColor(ctx.renderer, colors.focusBorder.r, colors.focusBorder.g, 
                             colors.focusBorder.b, colors.focusBorder.a);
        SDL_RenderDrawRect(ctx.renderer, &focusRect);
    }
}

void Image::reload(SDL_Renderer* renderer) {
    if (isDataImage && !imageData.empty()) {
        loadFromData(renderer, imageData.data(), imageData.size());
    } else if (!isDataImage && !filePath.empty()) {
        loadFromFile(renderer, filePath);
    }
}

void Image::setImagePath(SDL_Renderer* renderer, const std::string& path) {
    filePath = path;
    imageData.clear();
    isDataImage = false;
    loadFromFile(renderer, path);
}

void Image::setImageData(SDL_Renderer* renderer, const unsigned char* data, size_t dataSize) {
    filePath.clear();
    isDataImage = true;
    if (data && dataSize > 0) {
        imageData.assign(data, data + dataSize);
        loadFromData(renderer, data, dataSize);
    } else {
        imageData.clear();
        cleanup();
    }
}

} // namespace ui
