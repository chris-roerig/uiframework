#pragma once
#include "../UICore.h"
#include <vector>
#include <string>
#include <functional>

namespace ui {

class TextBox : public UIElement {
public:
    std::string content;
    bool textSelected;   // true if text is selected.
    bool autoHighlight;  // auto-highlight on focus; default true.
    TextBox(int x_, int y_, int w_, int h_, bool autoHighlight_ = true)
      : UIElement(x_, y_, w_, h_), content(""), textSelected(false), autoHighlight(autoHighlight_) {}
    void render(SDL_Renderer* renderer) override;
    void handleEvent(const SDL_Event &e) override;
    bool isInteractive() const override { return true; }
    SDL_Rect getFocusRect() const override;
    
    // Focus management.
    void onFocusGained();
    void onFocusLost();
};
} // namespace ui
