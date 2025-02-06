#pragma once
#include "../Helpers.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <vector>
#include <functional>
#include <memory>

namespace ui {

// Base class for all UI elements.
class UIElement {
public:
    int x, y, width, height;
    bool hasFocus = false;
    UIElement(int x_, int y_, int w_, int h_)
      : x(x_), y(y_), width(w_), height(h_) {}
    virtual ~UIElement() {}
    virtual void render(SDL_Renderer* renderer) = 0;
    virtual void handleEvent(const SDL_Event &e) {}
    virtual bool isInteractive() const { return false; }
    virtual SDL_Rect getFocusRect() const { return SDL_Rect{ x - 2, y - 2, width + 4, height + 4 }; }
    // virtual method for hot key activation.
    virtual void activate() { /* default does nothing */ }
};

} // namespace ui