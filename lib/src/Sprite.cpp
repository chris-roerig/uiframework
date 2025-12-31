#include "uiframework/UIElements/Sprite.h"
#include "uiframework/Theme/ThemeBase.h"
#include "uiframework/Helpers.h"
#include <SDL2/SDL_image.h>
#include <iostream>
#include <algorithm>

namespace ui {

// Constructor loading from a file
Sprite::Sprite(int x_, int y_, int w_, int h_, const std::string &path, bool stretch_)
    : UIElement(x_, y_, w_, h_), filePath(path), stretch(stretch_), isDataImage(false) {
    // Texture will be loaded on first render
}

// Constructor loading from binary data
Sprite::Sprite(int x_, int y_, int w_, int h_, const unsigned char* data, size_t dataSize, bool stretch_)
    : UIElement(x_, y_, w_, h_), stretch(stretch_), isDataImage(true) {
    if (data && dataSize > 0) {
        imageData.assign(data, data + dataSize);
    }
    // Texture will be loaded on first render
}

Sprite::~Sprite() {
    cleanup();
}

Sprite::Sprite(Sprite&& other) noexcept
    : UIElement(std::move(other)),
      filePath(std::move(other.filePath)),
      imageData(std::move(other.imageData)),
      stretch(other.stretch),
      texture(other.texture),
      srcRect(other.srcRect),
      isDataImage(other.isDataImage),
      naturalWidth(other.naturalWidth),
      naturalHeight(other.naturalHeight) {
    
    other.texture = nullptr;
    other.naturalWidth = 0;
    other.naturalHeight = 0;
}

Sprite& Sprite::operator=(Sprite&& other) noexcept {
    if (this != &other) {
        cleanup();
        
        UIElement::operator=(std::move(other));
        filePath = std::move(other.filePath);
        imageData = std::move(other.imageData);
        stretch = other.stretch;
        texture = other.texture;
        srcRect = other.srcRect;
        isDataImage = other.isDataImage;
        naturalWidth = other.naturalWidth;
        naturalHeight = other.naturalHeight;
        
        other.texture = nullptr;
        other.naturalWidth = 0;
        other.naturalHeight = 0;
    }
    return *this;
}

void Sprite::cleanup() {
    if (texture) {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }
}

void Sprite::loadFromFile(SDL_Renderer* ctx.renderer, const std::string& path) {
    if (!ctx.renderer || path.empty()) {
        return;
    }
    
    cleanup();
    
    SDL_Surface* surface = IMG_Load(path.c_str());
    if (!surface) {
        std::cerr << "IMG_Load Error: " << IMG_GetError() << " for file: " << path << std::endl;
        return;
    }
    
    texture = SDL_CreateTextureFromSurface(ctx.renderer, surface);
    if (!texture) {
        std::cerr << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
        SDL_FreeSurface(surface);
        return;
    }
    
    naturalWidth = surface->w;
    naturalHeight = surface->h;
    SDL_FreeSurface(surface);
    
    // Initialize source rect to full image if not set
    if (srcRect.w == 0 || srcRect.h == 0) {
        srcRect = {0, 0, naturalWidth, naturalHeight};
    }
    
    // Auto-size if not stretching
    if (!stretch) {
        width = srcRect.w;
        height = srcRect.h;
    }
}

void Sprite::loadFromData(SDL_Renderer* ctx.renderer, const unsigned char* data, size_t dataSize) {
    if (!ctx.renderer || !data || dataSize == 0) {
        return;
    }
    
    cleanup();
    
    SDL_RWops* rw = SDL_RWFromConstMem(data, static_cast<int>(dataSize));
    if (!rw) {
        std::cerr << "SDL_RWFromConstMem error: " << SDL_GetError() << std::endl;
        return;
    }
    
    SDL_Surface* surface = IMG_Load_RW(rw, 1);
    if (!surface) {
        std::cerr << "IMG_Load_RW error: " << IMG_GetError() << std::endl;
        return;
    }
    
    texture = SDL_CreateTextureFromSurface(ctx.renderer, surface);
    if (!texture) {
        std::cerr << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
        SDL_FreeSurface(surface);
        return;
    }
    
    naturalWidth = surface->w;
    naturalHeight = surface->h;
    SDL_FreeSurface(surface);
    
    // Initialize source rect to full image if not set
    if (srcRect.w == 0 || srcRect.h == 0) {
        srcRect = {0, 0, naturalWidth, naturalHeight};
    }
    
    // Auto-size if not stretching
    if (!stretch) {
        width = srcRect.w;
        height = srcRect.h;
    }
}

void Sprite::renderImpl(const RenderContext& ctx) {
    if (!visible) {
        return;
    }
    
    // Load texture if not already loaded
    if (!texture) {
        if (isDataImage && !imageData.empty()) {
            loadFromData(ctx.ctx.renderer, imageData.data(), imageData.size());
        } else if (!isDataImage && !filePath.empty()) {
            loadFromFile(ctx.ctx.renderer, filePath);
        }
    }
    
    if (!texture) {
        // Draw placeholder rectangle if sprite failed to load
        if (theme) {
            auto colors = theme->buttonColors();
            SDL_Rect rect = { x, y, width, height };
            SDL_SetRenderDrawColor(ctx.renderer, colors.buttonBackground.r, colors.buttonBackground.g, 
                                 colors.buttonBackground.b, colors.buttonBackground.a);
            SDL_RenderFillRect(ctx.renderer, &rect);
            SDL_SetRenderDrawColor(ctx.renderer, colors.buttonText.r, colors.buttonText.g, 
                                 colors.buttonText.b, colors.buttonText.a);
            SDL_RenderDrawRect(ctx.renderer, &rect);
            
            // Draw X to indicate missing sprite
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
        // Use source rectangle dimensions, centered if element is larger
        int spriteWidth = std::min(width, srcRect.w);
        int spriteHeight = std::min(height, srcRect.h);
        destRect = { 
            x + (width - spriteWidth) / 2, 
            y + (height - spriteHeight) / 2, 
            spriteWidth, 
            spriteHeight 
        };
    }
    
    SDL_RenderCopy(ctx.renderer, texture, &srcRect, &destRect);
    
    // Draw focus indicator if focused
    if (hasFocus && theme) {
        auto colors = theme->focusColors();
        SDL_Rect focusRect = getFocusRect();
        SDL_SetRenderDrawColor(ctx.renderer, colors.focusBorder.r, colors.focusBorder.g, 
                             colors.focusBorder.b, colors.focusBorder.a);
        SDL_RenderDrawRect(ctx.renderer, &focusRect);
    }
}

void Sprite::setSourceRect(const SDL_Rect &rect) {
    srcRect = rect;
    
    // Auto-size if not stretching
    if (!stretch && texture) {
        width = srcRect.w;
        height = srcRect.h;
    }
}

void Sprite::reload(SDL_Renderer* ctx.renderer) {
    if (isDataImage && !imageData.empty()) {
        loadFromData(ctx.renderer, imageData.data(), imageData.size());
    } else if (!isDataImage && !filePath.empty()) {
        loadFromFile(ctx.renderer, filePath);
    }
}

void Sprite::setSpritePath(SDL_Renderer* ctx.renderer, const std::string& path) {
    filePath = path;
    imageData.clear();
    isDataImage = false;
    loadFromFile(ctx.renderer, path);
}

void Sprite::setSpriteData(SDL_Renderer* ctx.renderer, const unsigned char* data, size_t dataSize) {
    filePath.clear();
    isDataImage = true;
    if (data && dataSize > 0) {
        imageData.assign(data, data + dataSize);
        loadFromData(ctx.renderer, data, dataSize);
    } else {
        imageData.clear();
        cleanup();
    }
}

void Sprite::setFrame(int frameIndex, int frameWidth, int frameHeight, int framesPerRow) {
    if (frameIndex < 0 || frameWidth <= 0 || frameHeight <= 0) {
        return;
    }
    
    if (framesPerRow <= 0) {
        // Calculate frames per row based on texture width
        framesPerRow = naturalWidth / frameWidth;
    }
    
    if (framesPerRow <= 0) {
        return;
    }
    
    int col = frameIndex % framesPerRow;
    int row = frameIndex / framesPerRow;
    
    setSourceRect(col * frameWidth, row * frameHeight, frameWidth, frameHeight);
}

void Sprite::setFrameGrid(int col, int row, int frameWidth, int frameHeight) {
    if (col < 0 || row < 0 || frameWidth <= 0 || frameHeight <= 0) {
        return;
    }
    
    setSourceRect(col * frameWidth, row * frameHeight, frameWidth, frameHeight);
}

} // namespace ui
