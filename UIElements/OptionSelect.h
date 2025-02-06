#pragma once
#include "UIElement.h"
#include <vector>
#include <string>
#include <functional>

namespace ui {

class OptionSelect : public UIElement {
public:
    std::vector<std::string> options;
    int selectedIndex; // used when expanded.
    int activeIndex;   // currently chosen value.
    bool expanded;     // whether drop-down is open.
    std::function<void(int)> onSelect;
    
    OptionSelect(int x_, int y_, int w_, int h_, const std::vector<std::string>& opts,
                 int initial = 0, int active = -1, std::function<void(int)> callback = nullptr)
      : UIElement(x_, y_, w_, h_), options(opts), selectedIndex(initial),
        activeIndex(active == -1 ? initial : active), expanded(false), onSelect(callback) {}
    void render(SDL_Renderer* renderer) override;
    void handleEvent(const SDL_Event &e) override;
    bool isInteractive() const override { return true; }
    SDL_Rect getFocusRect() const override;
    virtual void activate() override;
};

} // namespace ui