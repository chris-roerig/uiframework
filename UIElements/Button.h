#pragma once
#include "../UICore.h"
#include <vector>
#include <string>
#include <functional>

namespace ui {

class Button : public UIElement {
public:
    std::string text;
    std::function<void()> onClick;
    bool pressed; // tracks pressed state.
    
    Button(int x_, int y_, int w_, int h_, const std::string &text_, std::function<void()> callback)
      : UIElement(x_, y_, w_, h_), text(text_), onClick(callback), pressed(false) {}
    void render(SDL_Renderer* renderer) override;
    void handleEvent(const SDL_Event &e) override;
    bool isInteractive() const override { return true; }
    virtual void activate() override;
};

} // namespace ui
