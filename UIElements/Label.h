#pragma once
#include "UIElement.h"
#include <vector>
#include <string>
#include <functional>
#include <iostream>

namespace ui {

class Label : public UIElement {
public:
    std::string text;
    Label(int x_, int y_, const std::string &text_)
      : UIElement(x_, y_, 0, 0), text(text_) {
          initFont();
          if (globalFont) {
              TTF_SizeText(globalFont, text.c_str(), &width, &height);
          }
      }
    void render(SDL_Renderer* renderer) override;
    void setText(const std::string &text);
};

} // namespace ui