#pragma once

#include "../Helpers.h"
#include "../UIElements/UIElement.h"
#include <SDL2/SDL.h>
#include <string>
#include <vector>
#include <memory>

namespace ui {

class TabContainer : public UIElement {
public:
    // Constructs a TabContainer at (x,y) with width w and height h.
    // Tab bar height is fixed (e.g. 30 pixels).
    TabContainer(int x_, int y_, int w_, int h_);
    virtual ~TabContainer();

    // Adds a new tab with the given label and panel content.
    void addTab(const std::string &label, std::shared_ptr<UIElement> panel);

    // Set the current tab by index.
    void setCurrentTab(int index);

    // Renders the tab bar and the currently active panel.
    virtual void render(SDL_Renderer* renderer) override;
    virtual void handleEvent(const SDL_Event &e) override;

private:
    std::vector<std::string> tabLabels;
    std::vector<std::shared_ptr<UIElement>> panels;
    int currentTab;
    const int tabBarHeight = 30;
};

} // namespace ui
