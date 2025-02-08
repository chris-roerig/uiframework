#pragma once

#include "Sprite.h"
#include <vector>
#include <SDL2/SDL.h>

namespace ui {

class AnimatedSprite : public Sprite {
public:
    // Constructor for file-based animated sprite.
    // 'frameCount' is the total number of frames (for default calculation).
    // 'frameDelay' is the delay per frame in milliseconds.
    // 'stretch' controls if the rendered destination uses the provided width/height.
    AnimatedSprite(int x_, int y_, int w_, int h_, const std::string &path, SDL_Renderer* renderer,
                   int frameCount, Uint32 frameDelay, bool stretch);

    // Constructor for embedded binary animated sprite.
    AnimatedSprite(int x_, int y_, int w_, int h_, const unsigned char* data, size_t dataSize, SDL_Renderer* renderer,
                   int frameCount, Uint32 frameDelay, bool stretch);

    virtual ~AnimatedSprite();

    // Call update() in your main loop if you are not using render() to update animation.
    void update();

    // Override render to update animation frame automatically.
    virtual void render(SDL_Renderer* renderer) override;

    // If your sprite sheet does not evenly divide into frames,
    // call this to set the actual frame dimensions.
    // For example, for 9 frames of 85x85 starting at (0,0):
    //    setFrameDimensions(85, 85, 9);
    void setFrameDimensions(int frameW, int frameH, int numFrames);

    // Returns the vector of source rectangles (for debugging or further control).
    const std::vector<SDL_Rect>& getFrames() const;

private:
    int frameCount;       // Total number of frames.
    Uint32 frameDelay;    // Delay per frame (ms).
    int currentFrame;     // Current frame index.
    Uint32 lastUpdate;    // Last timestamp update.
    std::vector<SDL_Rect> frames; // Source rectangles for each frame.
};

} // namespace ui
