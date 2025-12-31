#pragma once
#include "InteractiveElement.h"
#include <vector>
#include <string>
#include <functional>

namespace ui {

// Helper structures for building context menus
struct SubMenuItem {
    std::string label;
    std::function<void()> callback;
};

struct TopMenuItem {
    std::string label;
    std::vector<SubMenuItem> subItems;
};

// Internal menu item structure
struct MenuItem {
    std::string label;
    std::vector<std::string> subItemLabels;
    std::vector<std::function<void()>> subCallbacks;
    int width = 0; // Calculated width for this menu item
};

// ContextMenu: A full-width menu bar that spans the application
class ContextMenu : public InteractiveElement {
private:
    int hoveredItemIndex = -1;
    int hoveredSubIndex = -1;
    
    // Render helper methods
    void renderMenuBar(SDL_Renderer* renderer, TTF_Font* font, std::shared_ptr<class Theme> theme);
    void renderMenuItems(SDL_Renderer* renderer, TTF_Font* font, std::shared_ptr<class Theme> theme);
    void renderSubmenu(SDL_Renderer* renderer, TTF_Font* font, std::shared_ptr<class Theme> theme);
    void renderSubmenuItems(SDL_Renderer* renderer, TTF_Font* font, std::shared_ptr<class Theme> theme, 
                           const MenuItem& activeItem, const SDL_Rect& subMenuRect);
    
public:
    std::vector<MenuItem> items;
    int activeItemIndex = -1;       // Active top-level item
    int subMenuSelectedIndex = -1;  // Active sub-item when expanded
    bool expanded = false;          // Whether the sub-menu is open
    
    ContextMenu(int x_, int y_, int w_, int h_, const std::vector<TopMenuItem>& menus);
    
    SDL_Rect getFocusRect() const override;
    void activate() override;

protected:
    void onMouseDown(int x, int y) override;
    void onKeyDown(const SDL_Keycode& key) override;
    void renderImpl(const RenderContext& ctx) override;

public:
    
public:
    // Menu management
    void setMenuItems(const std::vector<TopMenuItem>& menus);
    void addMenuItem(const TopMenuItem& menu);
    void clearMenuItems();
    
    // Navigation
    void setActiveItem(int index);
    void expandMenu();
    void collapseMenu();
    void selectSubItem(int index);
    
    // Utility methods
    int getItemAt(int mouseX, int mouseY) const;
    int getSubItemAt(int mouseX, int mouseY) const;
    SDL_Rect getItemRect(int itemIndex) const;
    SDL_Rect getSubItemRect(int subItemIndex) const;
    void calculateItemWidths(TTF_Font* font);
};

} // namespace ui
