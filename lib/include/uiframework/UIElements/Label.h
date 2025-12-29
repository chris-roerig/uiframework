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
    
    Label(int x_, int y_, int w_, int h_, const std::string &text_)
      : UIElement(x_, y_, w_, h_), text(text_) {}
    
    void render(SDL_Renderer* renderer, TTF_Font* font, std::shared_ptr<class Theme> theme) override;
    void setText(const std::string &text);
    const std::string& getText() const { return text; }
    
    // Auto-size the label based on text content
    void autoSize(TTF_Font* font);
};

} // namespace ui
