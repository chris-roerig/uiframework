#include "uiframework/UIElements/AnimatedSprite.h"
#include <iostream>
#include <algorithm>

namespace ui {

AnimatedSprite::AnimatedSprite(int x_, int y_, int w_, int h_, const std::string &path,
                               int frameCount, Uint32 frameDelay_, bool stretch)
    : Sprite(x_, y_, w_, h_, path, stretch), frameDelay(frameDelay_), lastFrameTime(SDL_GetTicks()) {
    // Frames will be calculated when texture is loaded
}

AnimatedSprite::AnimatedSprite(int x_, int y_, int w_, int h_, const unsigned char* data, size_t dataSize,
                               int frameCount, Uint32 frameDelay_, bool stretch)
    : Sprite(x_, y_, w_, h_, data, dataSize, stretch), frameDelay(frameDelay_), lastFrameTime(SDL_GetTicks()) {
    // Frames will be calculated when texture is loaded
}

void AnimatedSprite::calculateFrames(int frameCount, int frameWidth, int frameHeight) {
    frames.clear();
    
    if (frameCount <= 0 || frameWidth <= 0 || frameHeight <= 0) {
        return;
    }
    
    int framesPerRow = naturalWidth / frameWidth;
    if (framesPerRow <= 0) {
        return;
    }
    
    for (int i = 0; i < frameCount; i++) {
        int col = i % framesPerRow;
        int row = i / framesPerRow;
        
        SDL_Rect frameRect = {
            col * frameWidth,
            row * frameHeight,
            frameWidth,
            frameHeight
        };
        
        // Make sure frame is within texture bounds
        if (frameRect.x + frameRect.w <= naturalWidth && 
            frameRect.y + frameRect.h <= naturalHeight) {
            frames.push_back(frameRect);
        }
    }
    
    if (!frames.empty()) {
        setSourceRect(frames[0]);
    }
}

void AnimatedSprite::render(SDL_Renderer* renderer, TTF_Font* font, std::shared_ptr<Theme> theme) {
    if (!renderer || !theme) return;
    
    // Update animation before rendering
    update();
    
    // Call parent render method
    Sprite::render(renderer, font, theme);
}

void AnimatedSprite::update() {
    if (!isPlaying || frames.empty()) {
        return;
    }
    
    Uint32 currentTime = SDL_GetTicks();
    if (currentTime - lastFrameTime >= frameDelay) {
        currentFrame++;
        
        if (currentFrame >= static_cast<int>(frames.size())) {
            if (looping) {
                currentFrame = 0;
            } else {
                currentFrame = static_cast<int>(frames.size()) - 1;
                isPlaying = false;
            }
        }
        
        if (currentFrame < static_cast<int>(frames.size())) {
            setSourceRect(frames[currentFrame]);
        }
        
        lastFrameTime = currentTime;
    }
}

void AnimatedSprite::setFrames(const std::vector<SDL_Rect>& frameRects) {
    frames = frameRects;
    currentFrame = 0;
    if (!frames.empty()) {
        setSourceRect(frames[0]);
    }
}

void AnimatedSprite::addFrame(const SDL_Rect& frameRect) {
    frames.push_back(frameRect);
    if (frames.size() == 1) {
        setSourceRect(frameRect);
    }
}

void AnimatedSprite::clearFrames() {
    frames.clear();
    currentFrame = 0;
}

void AnimatedSprite::setCurrentFrame(int frame) {
    if (frame >= 0 && frame < static_cast<int>(frames.size())) {
        currentFrame = frame;
        setSourceRect(frames[currentFrame]);
        lastFrameTime = SDL_GetTicks();
    }
}

void AnimatedSprite::setupFrameGrid(int frameWidth, int frameHeight, int framesPerRow, int totalFrames) {
    if (!isLoaded()) {
        std::cerr << "AnimatedSprite: Cannot setup frames - texture not loaded" << std::endl;
        return;
    }
    
    if (framesPerRow <= 0) {
        // framesPerRow = naturalWidth / frameWidth; // Calculated but not used
    }
    
    if (totalFrames <= 0) {
        int maxFramesPerRow = naturalWidth / frameWidth;
        int maxRows = naturalHeight / frameHeight;
        totalFrames = maxFramesPerRow * maxRows;
    }
    
    calculateFrames(totalFrames, frameWidth, frameHeight);
}

void AnimatedSprite::setupHorizontalStrip(int frameWidth, int frameCount) {
    if (!isLoaded()) {
        std::cerr << "AnimatedSprite: Cannot setup frames - texture not loaded" << std::endl;
        return;
    }
    
    frames.clear();
    
    for (int i = 0; i < frameCount; i++) {
        SDL_Rect frameRect = {
            i * frameWidth,
            0,
            frameWidth,
            naturalHeight
        };
        
        if (frameRect.x + frameRect.w <= naturalWidth) {
            frames.push_back(frameRect);
        }
    }
    
    if (!frames.empty()) {
        currentFrame = 0;
        setSourceRect(frames[0]);
    }
}

void AnimatedSprite::setupVerticalStrip(int frameHeight, int frameCount) {
    if (!isLoaded()) {
        std::cerr << "AnimatedSprite: Cannot setup frames - texture not loaded" << std::endl;
        return;
    }
    
    frames.clear();
    
    for (int i = 0; i < frameCount; i++) {
        SDL_Rect frameRect = {
            0,
            i * frameHeight,
            naturalWidth,
            frameHeight
        };
        
        if (frameRect.y + frameRect.h <= naturalHeight) {
            frames.push_back(frameRect);
        }
    }
    
    if (!frames.empty()) {
        currentFrame = 0;
        setSourceRect(frames[0]);
    }
}

} // namespace ui
