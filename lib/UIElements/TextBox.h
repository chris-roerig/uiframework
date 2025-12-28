#pragma once
#include "UIElement.h"
#include <vector>
#include <string>
#include <functional>

namespace ui {

class TextBox : public UIElement {
private:
    bool textSelected = false;   // true if text is selected
    size_t cursorPosition = 0;   // cursor position in text
    
public:
    std::string content;
    bool autoHighlight;  // auto-highlight on focus; default true
    
    TextBox(int x_, int y_, int w_, int h_, const std::string& defaultText = "", bool autoHighlight_ = true)
      : UIElement(x_, y_, w_, h_), content(defaultText), autoHighlight(autoHighlight_) {
        cursorPosition = content.length();
    }
    
    void render(SDL_Renderer* renderer, TTF_Font* font, std::shared_ptr<class Theme> theme) override;
    void handleEvent(const SDL_Event &e) override;
    bool isInteractive() const override { return true; }
    SDL_Rect getFocusRect() const override;
    void activate() override;
    
    // Focus management
    void onFocusGained() override;
    void onFocusLost() override;
    
    // TextBox-specific methods
    void setText(const std::string& text);
    const std::string& getText() const { return content; }
    void clear() { setText(""); }
    void selectAll();
    void clearSelection();
    bool hasSelection() const { return textSelected; }
};

} // namespace ui
