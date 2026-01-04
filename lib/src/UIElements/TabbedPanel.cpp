#include "uiframework/UIElements/TabbedPanel.h"
#include "uiframework/Theme/ThemeBase.h"
#include "uiframework/UICore.h"
#include "uiframework/Helpers.h"
#include <algorithm>

namespace ui {

TabbedPanel::TabbedPanel(int x, int y, int w, int h)
    : InteractiveElement(x, y, w, h) {
}

void TabbedPanel::renderImpl(const RenderContext& ctx) {
    if (!visible) return;
    
    // Draw panel background
    auto colors = ctx.tabbedPanelColors();
    SDL_SetRenderDrawColor(ctx.renderer, colors.tabbedPanelBackground.r, colors.tabbedPanelBackground.g, 
                          colors.tabbedPanelBackground.b, colors.tabbedPanelBackground.a);
    SDL_Rect panelRect = {x, y + tabHeight, width, height - tabHeight};
    SDL_RenderFillRect(ctx.renderer, &panelRect);
    
    // Draw panel border
    SDL_SetRenderDrawColor(ctx.renderer, colors.tabbedPanelBorder.r, colors.tabbedPanelBorder.g, 
                          colors.tabbedPanelBorder.b, colors.tabbedPanelBorder.a);
    SDL_RenderDrawRect(ctx.renderer, &panelRect);
    
    renderTabs(ctx.renderer, ctx.font, ctx.theme);
    renderTabContent(ctx.renderer, ctx.font, ctx.theme);
    
    // Draw focus border if focused
    if (hasFocus) {
        auto focusColors = ctx.theme->focusColors();
        SDL_SetRenderDrawColor(ctx.renderer, focusColors.focusBorder.r, focusColors.focusBorder.g, 
                              focusColors.focusBorder.b, focusColors.focusBorder.a);
        SDL_Rect focusRect = {x - 1, y - 1, width + 2, height + 2};
        SDL_RenderDrawRect(ctx.renderer, &focusRect);
    }
}

void TabbedPanel::renderTabs(SDL_Renderer* renderer, TTF_Font* font, std::shared_ptr<Theme> theme) {
    if (tabs.empty()) return;
    
    auto colors = theme->tabbedPanelColors();
    int tabWidth = width / tabs.size();
    
    for (size_t i = 0; i < tabs.size(); ++i) {
        int tabX = x + (i * tabWidth);
        bool isActive = (activeTabIndex >= 0 && i == static_cast<size_t>(activeTabIndex));
        
        // Draw tab background
        if (isActive) {
            SDL_SetRenderDrawColor(renderer, colors.tabbedPanelTabActive.r, colors.tabbedPanelTabActive.g, 
                                  colors.tabbedPanelTabActive.b, colors.tabbedPanelTabActive.a);
        } else {
            SDL_SetRenderDrawColor(renderer, colors.tabbedPanelTabInactive.r, colors.tabbedPanelTabInactive.g, 
                                  colors.tabbedPanelTabInactive.b, colors.tabbedPanelTabInactive.a);
        }
        
        SDL_Rect tabRect = {tabX, y, tabWidth, tabHeight};
        SDL_RenderFillRect(renderer, &tabRect);
        
        // Draw tab border
        SDL_SetRenderDrawColor(renderer, colors.tabbedPanelBorder.r, colors.tabbedPanelBorder.g, 
                              colors.tabbedPanelBorder.b, colors.tabbedPanelBorder.a);
        SDL_RenderDrawRect(renderer, &tabRect);
        
        // Draw tab text
        SDL_Color textColor = {colors.tabbedPanelTabText.r, colors.tabbedPanelTabText.g, colors.tabbedPanelTabText.b, colors.tabbedPanelTabText.a};
            
        auto* textEntry = getCachedText("tab_" + std::to_string(i), tabs[i].title, textColor, renderer, font);
        if (textEntry && textEntry->texture) {
            int textX = tabX + (tabWidth - textEntry->width) / 2;
            int textY = y + (tabHeight - textEntry->height) / 2;
            SDL_Rect textRect = {textX, textY, textEntry->width, textEntry->height};
            SDL_RenderCopy(renderer, textEntry->texture, nullptr, &textRect);
        }
    }
}

void TabbedPanel::renderTabContent(SDL_Renderer* renderer, TTF_Font* font, std::shared_ptr<Theme> theme) {
    // Child elements are rendered by the main UI system based on visibility
    // We just need to ensure visibility is set correctly
    updateChildVisibility();
}

void TabbedPanel::onMouseDown(int x, int y) {
    int tabIndex = getTabAtPosition(x, y);
    if (tabIndex >= 0) {
        setActiveTab(tabIndex);
    }
}

void TabbedPanel::onKeyDown(const SDL_Keycode& key) {
    switch (key) {
        case SDLK_LEFT:
            if (activeTabIndex > 0) {
                setActiveTab(activeTabIndex - 1);
            }
            break;
        case SDLK_RIGHT:
            if (activeTabIndex < static_cast<int>(tabs.size()) - 1) {
                setActiveTab(activeTabIndex + 1);
            }
            break;
    }
}

void TabbedPanel::activate() {
    // TabbedPanel activates by cycling to next tab
    if (activeTabIndex < static_cast<int>(tabs.size()) - 1) {
        setActiveTab(activeTabIndex + 1);
    } else {
        setActiveTab(0);
    }
}

int TabbedPanel::addTab(const std::string& title) {
    Tab newTab;
    newTab.title = title;
    newTab.active = false;
    tabs.push_back(newTab);
    
    if (tabs.size() == 1) {
        activeTabIndex = 0;  // First tab becomes active
        tabs[0].active = true;
        updateChildVisibility();
    }
    
    return tabs.size() - 1;
}

void TabbedPanel::addChildToTab(int tabIndex, std::shared_ptr<UIElement> child) {
    if (tabIndex >= 0 && tabIndex < static_cast<int>(tabs.size())) {
        tabs[tabIndex].children.push_back(child);
        updateChildVisibility();
    }
}

void TabbedPanel::setActiveTab(int tabIndex) {
    if (tabIndex >= 0 && tabIndex < static_cast<int>(tabs.size())) {
        activeTabIndex = tabIndex;
        
        // Update tab active states
        for (size_t i = 0; i < tabs.size(); ++i) {
            tabs[i].active = (i == activeTabIndex);
        }
        
        updateChildVisibility();
        invalidateTextCache();
        
        if (onTabChange && coreRef) {
            int currentTabIndex = activeTabIndex;
            coreRef->queueCallback([this, currentTabIndex]() {
                if (onTabChange) {
                    try {
                        onTabChange(currentTabIndex);
                    } catch (const std::exception& e) {
                        std::cerr << "Error in tabbed panel callback: " << e.what() << std::endl;
                    }
                }
            });
        }
    }
}

void TabbedPanel::updateChildVisibility() {
    for (size_t i = 0; i < tabs.size(); ++i) {
        bool shouldBeVisible = (activeTabIndex >= 0 && i == static_cast<size_t>(activeTabIndex));
        for (auto& child : tabs[i].children) {
            child->setVisible(shouldBeVisible);
        }
    }
}

int TabbedPanel::getTabAtPosition(int mouseX, int mouseY) {
    if (tabs.empty()) return -1;
    
    // Check if click is in tab area
    if (mouseY < y || mouseY >= y + tabHeight || mouseX < x || mouseX >= x + width) {
        return -1;
    }
    
    int tabWidth = width / tabs.size();
    int tabIndex = (mouseX - x) / tabWidth;
    
    if (tabIndex >= 0 && tabIndex < static_cast<int>(tabs.size())) {
        return tabIndex;
    }
    
    return -1;
}

} // namespace ui
