#pragma once

#include "Sprite.h"
#include <vector>
#include <SDL2/SDL.h>

namespace ui {

class AnimatedSprite : public Sprite {
private:
    std::vector<SDL_Rect> frames;
    int currentFrame = 0;
    Uint32 frameDelay = 100; // milliseconds per frame
    Uint32 lastFrameTime = 0;
    bool isPlaying = true;
    bool looping = true;
    
    void calculateFrames(int frameCount, int frameWidth, int frameHeight);

public:
    // Constructor for file-based animated sprite
    AnimatedSprite(int x_, int y_, int w_, int h_, const std::string &path,
                   int frameCount, Uint32 frameDelay, bool stretch = false);

    // Constructor for embedded binary animated sprite
    AnimatedSprite(int x_, int y_, int w_, int h_, const unsigned char* data, size_t dataSize,
                   int frameCount, Uint32 frameDelay, bool stretch = false);

    virtual ~AnimatedSprite() = default;

    void render(SDL_Renderer* renderer, TTF_Font* font, std::shared_ptr<class Theme> theme) override;

    // Animation control
    void update();
    void play() { isPlaying = true; }
    void pause() { isPlaying = false; }
    void stop() { isPlaying = false; currentFrame = 0; }
    void reset() { currentFrame = 0; lastFrameTime = SDL_GetTicks(); }
    
    // Frame management
    void setFrames(const std::vector<SDL_Rect>& frameRects);
    void addFrame(const SDL_Rect& frameRect);
    void clearFrames();
    void setCurrentFrame(int frame);
    int getCurrentFrame() const { return currentFrame; }
    int getFrameCount() const { return static_cast<int>(frames.size()); }
    
    // Animation properties
    void setFrameDelay(Uint32 delay) { frameDelay = delay; }
    Uint32 getFrameDelay() const { return frameDelay; }
    void setLooping(bool loop) { looping = loop; }
    bool isLooping() const { return looping; }
    bool getIsPlaying() const { return isPlaying; }
    
    // Utility methods for sprite sheet setup
    void setupFrameGrid(int frameWidth, int frameHeight, int framesPerRow = 0, int totalFrames = 0);
    void setupHorizontalStrip(int frameWidth, int frameCount);
    void setupVerticalStrip(int frameHeight, int frameCount);
};

} // namespace ui
