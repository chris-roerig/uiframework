#include "TabContainer.h"
#include "../Theme/ThemeGlobals.h"
#include <SDL2/SDL.h>
#include <iostream>

namespace ui {

TabContainer::TabContainer(int x_, int y_, int w_, int h_)
    : UIElement(x_, y_, w_, h_), currentTab(0)
{
}

TabContainer::~TabContainer() {}

void TabContainer::addTab(const std::string &label, std::shared_ptr<UIElement> panel) {
    tabLabels.push_back(label);
    panels.push_back(panel);
}

void TabContainer::setCurrentTab(int index) {
    if(index >= 0 && index < static_cast<int>(panels.size()))
        currentTab = index;
}

void TabContainer::render(SDL_Renderer* renderer) {
    ThemeableElementColors tc = g_currentTheme->containerColors();
    Color tabBg = tc.containerBackground;
    SDL_SetRenderDrawColor(renderer, tabBg.r, tabBg.g, tabBg.b, tabBg.a);
    SDL_Rect tabBarRect = { x, y, width, tabBarHeight };
    SDL_RenderFillRect(renderer, &tabBarRect);

    // Render tab labels.
    // (For simplicity, we simply draw rectangles here.
    // In a full implementation, you might render text.)
    int numTabs = tabLabels.size();
    int tabWidth = (numTabs > 0) ? width / numTabs : width;
    for (int i = 0; i < numTabs; ++i) {
        SDL_Rect tabRect = { x + i * tabWidth, y, tabWidth, tabBarHeight };
        if (i == currentTab) {
            // Highlight current tab.
            SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
        } else {
            SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255);
        }
        SDL_RenderFillRect(renderer, &tabRect);
        // Here, you would also render the label text.
    }

    // Render the currently active panel within the remaining area.
    if (!panels.empty() && currentTab < static_cast<int>(panels.size())) {
        // Adjust panel position to start below the tab bar.
        panels[currentTab]->x = x;
        panels[currentTab]->y = y + tabBarHeight;
        panels[currentTab]->width = width;
        panels[currentTab]->height = height - tabBarHeight;
        panels[currentTab]->render(renderer);
    }
}

void TabContainer::handleEvent(const SDL_Event &e) {
    // Here you would detect clicks in the tab bar and update currentTab.
    // For now, we simply forward events to the current panel.
    if (!panels.empty() && currentTab < static_cast<int>(panels.size()))
        panels[currentTab]->handleEvent(e);
}

} // namespace ui
