#pragma once
#include "UIElement.h"
#include <vector>
#include <string>
#include <functional>

namespace ui {

// A single top-level menu item.
struct MenuItem {
    std::string label;
    std::vector<std::string> subItemLabels;
    std::vector<std::function<void()>> subCallbacks;
};

// Helper structures for building context menus.
struct SubMenuItem {
    std::string label;
    std::function<void()> callback;
};

struct TopMenuItem {
    std::string label;
    std::vector<SubMenuItem> subItems;
};

// ContextMenu: A full-width menu bar that spans the application.
// Top-level items span only the width of their text (plus horizontal padding);
// the remaining space is empty. When an item is active, its background changes.
// Sub-menu items are shown below the active item when expanded.
class ContextMenu : public UIElement {
public:
    std::vector<MenuItem> items;
    int activeItemIndex;       // Active top-level item.
    int subMenuSelectedIndex;  // Active sub-item when expanded.
    bool expanded;             // Whether the sub-menu is open.
    
    // Instead of a fixed item width, we use the measured width for each item.
    // totalWidth represents the full width of the bar.
    int totalWidth;
    int itemHeight;  // Height of the top-level bar.
    
    ContextMenu(int x_, int y_, int totalWidth_, int itemHeight_);
    void setItems(const std::vector<MenuItem>& newItems);
    
    void render(SDL_Renderer* renderer) override;
    void handleEvent(const SDL_Event &e) override;
    
    // We do not draw a focus border for the entire context menu.
    SDL_Rect getFocusRect() const override;
    bool isInteractive() const override { return true; }
};

} // namespace ui
