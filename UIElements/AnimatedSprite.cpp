#include "AnimatedSprite.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>

namespace ui {

AnimatedSprite::AnimatedSprite(int x_, int y_, int w_, int h_, const std::string &path, SDL_Renderer* renderer,
                               int frameCount_, Uint32 frameDelay_, bool stretch)
    : Sprite(x_, y_, w_, h_, path, renderer, stretch),
      frameCount(frameCount_), frameDelay(frameDelay_), currentFrame(0), lastUpdate(SDL_GetTicks())
{
    // Default calculation: evenly divide the natural width.
    // (This may not be what you want if the frame size is fixed.)
    int defaultFrameW = naturalWidth / frameCount;
    int defaultFrameH = naturalHeight;
    frames.clear();
    for (int i = 0; i < frameCount; ++i) {
        SDL_Rect rect = { i * defaultFrameW, 0, defaultFrameW, defaultFrameH };
        frames.push_back(rect);
    }
    // Set initial frame.
    setSourceRect(frames[0]);
}

AnimatedSprite::AnimatedSprite(int x_, int y_, int w_, int h_, const unsigned char* data, size_t dataSize, SDL_Renderer* renderer,
                               int frameCount_, Uint32 frameDelay_, bool stretch)
    : Sprite(x_, y_, w_, h_, data, dataSize, renderer, stretch),
      frameCount(frameCount_), frameDelay(frameDelay_), currentFrame(0), lastUpdate(SDL_GetTicks())
{
    int defaultFrameW = naturalWidth / frameCount;
    int defaultFrameH = naturalHeight;
    frames.clear();
    for (int i = 0; i < frameCount; ++i) {
        SDL_Rect rect = { i * defaultFrameW, 0, defaultFrameW, defaultFrameH };
        frames.push_back(rect);
    }
    setSourceRect(frames[0]);
}

AnimatedSprite::~AnimatedSprite() {
    // Nothing extra to do here.
}

void AnimatedSprite::setFrameDimensions(int frameW, int frameH, int numFrames) {
    frames.clear();
    for (int i = 0; i < numFrames; ++i) {
        SDL_Rect rect = { i * frameW, 0, frameW, frameH };
        frames.push_back(rect);
    }
    // Use these frames instead of the default.
    frameCount = numFrames;
    currentFrame = 0;
    setSourceRect(frames[0]);
}

const std::vector<SDL_Rect>& AnimatedSprite::getFrames() const {
    return frames;
}

void AnimatedSprite::update() {
    Uint32 currentTime = SDL_GetTicks();
    if (currentTime - lastUpdate >= frameDelay) {
        currentFrame = (currentFrame + 1) % frameCount;
        lastUpdate = currentTime;
        setSourceRect(frames[currentFrame]);
    }
}

void AnimatedSprite::render(SDL_Renderer* renderer) {
    update(); // Update animation frame.
    Sprite::render(renderer);
}

} // namespace ui
