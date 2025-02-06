#pragma once
#include "UIElement.h"
#include <vector>
#include <string>
#include <functional>

namespace ui {

class Label : public UIElement {
public:
    std::string text;
    Label(int x_, int y_, const std::string &text_)
      : UIElement(x_, y_, 0, 0), text(text_) {}
    void render(SDL_Renderer* renderer) override;
};

} // namespace ui