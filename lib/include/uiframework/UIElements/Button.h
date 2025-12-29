#pragma once
#include "UIElement.h"
#include <vector>
#include <string>
#include <functional>

namespace ui {

class Button : public UIElement {
private:
    bool pressed = false; // tracks pressed state
    
public:
    std::string text;
    std::function<void()> onClick;
    
    Button(int x_, int y_, int w_, int h_, const std::string &text_, std::function<void()> callback)
      : UIElement(x_, y_, w_, h_), text(text_), onClick(callback) {}
    
    void render(SDL_Renderer* renderer, TTF_Font* font, std::shared_ptr<class Theme> theme) override;
    void handleEvent(const SDL_Event &e) override;
    bool isInteractive() const override { return true; }
    void activate() override;
    
    // Button-specific methods
    void setText(const std::string& newText) { 
        if (text != newText) {
            text = newText; 
            invalidateTextCache();
        }
    }
    const std::string& getText() const { return text; }
    void setCallback(std::function<void()> callback) { onClick = callback; }
    bool isPressed() const { return pressed; }
};

} // namespace ui
