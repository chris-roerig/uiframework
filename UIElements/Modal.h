#pragma once
#include "../UICore.h"
#include <string>
#include <functional>
#include <vector>

namespace ui {

class Modal : public UIElement {
public:
    std::string message;
    // Each button has a label and a callback.
    std::vector<std::string> buttonLabels;
    std::vector<std::function<void()>> buttonCallbacks;
    // Index of the button that currently has focus.
    int buttonFocusIndex;
    
    std::function<void()> onDismiss;
    bool dismissed;  // True when the modal should be removed.
    
    // Constructor: x, y, width, height of the modal box; message to display.
    Modal(int x_, int y_, int w_, int h_, const std::string &msg);
    
    void render(SDL_Renderer* renderer) override;
    void handleEvent(const SDL_Event &e) override;
    SDL_Rect getFocusRect() const override;
    bool isInteractive() const override { return true; }
};

} // namespace ui
