#pragma once
#include "UICore.h"
#include <vector>
#include <string>
#include <functional>

namespace ui {

class ListView : public UIElement {
public:
    // The list of items to display.
    std::vector<std::string> items;
    // Currently selected index.
    int selectedIndex;
    // Fixed height for each item.
    int itemHeight;
    // Callback when an item is selected (via ENTER).
    std::function<void(int)> onSelect;

    // Constructor: x, y position; w, h are the overall bounds; itemHeight is per-item height.
    ListView(int x_, int y_, int w_, int h_, int itemHeight_ = 30);

    void render(SDL_Renderer* renderer) override;
    void handleEvent(const SDL_Event &e) override;
    SDL_Rect getFocusRect() const override;
    bool isInteractive() const override { return true; }
};

} // namespace ui