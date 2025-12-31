#pragma once
#include "InteractiveElement.h"
#include <vector>
#include <string>
#include <memory>
#include <functional>

namespace ui {

struct Tab {
    std::string title;
    std::vector<std::shared_ptr<UIElement>> children;
    bool active = false;
};

class TabbedPanel : public InteractiveElement {
private:
    std::vector<Tab> tabs;
    int activeTabIndex = -1;  // No active tab initially
    int tabHeight = 25;
    std::function<void(int)> onTabChange;
    
public:
    TabbedPanel(int x, int y, int w, int h);
    
protected:
    void renderImpl(const RenderContext& ctx) override;
    void onMouseDown(int x, int y) override;
    void onKeyDown(const SDL_Keycode& key) override;

public:
    void activate() override;
    
    // Tab management
    int addTab(const std::string& title);
    void addChildToTab(int tabIndex, std::shared_ptr<UIElement> child);
    void setActiveTab(int tabIndex);
    int getActiveTab() const { return activeTabIndex; }
    void setOnTabChange(std::function<void(int)> callback) { onTabChange = callback; }
    
    // Child element visibility management
    void updateChildVisibility();
    
private:
    void renderTabs(SDL_Renderer* renderer, TTF_Font* font, std::shared_ptr<Theme> theme);
    void renderTabContent(SDL_Renderer* renderer, TTF_Font* font, std::shared_ptr<Theme> theme);
    int getTabAtPosition(int mouseX, int mouseY);
};

} // namespace ui
