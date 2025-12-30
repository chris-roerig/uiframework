#include "uiframework/UIElements/TabbedPanel.h"
#include "uiframework/Theme/ThemeBase.h"
#include "uiframework/UICore.h"
#include "uiframework/Helpers.h"
#include <algorithm>

namespace ui {

TabbedPanel::TabbedPanel(int x, int y, int w, int h)
    : UIElement(x, y, w, h) {
}

void TabbedPanel::render(SDL_Renderer* renderer, TTF_Font* font, std::shared_ptr<Theme> theme) {
    if (!visible) return;
    
    // Draw panel background
    auto colors = theme->buttonColors();
    SDL_SetRenderDrawColor(renderer, colors.buttonBackground.r, colors.buttonBackground.g, 
                          colors.buttonBackground.b, colors.buttonBackground.a);
    SDL_Rect panelRect = {x, y + tabHeight, width, height - tabHeight};
    SDL_RenderFillRect(renderer, &panelRect);
    
    // Draw panel border
    SDL_SetRenderDrawColor(renderer, colors.buttonBorderDark.r, colors.buttonBorderDark.g, 
                          colors.buttonBorderDark.b, colors.buttonBorderDark.a);
    SDL_RenderDrawRect(renderer, &panelRect);
    
    renderTabs(renderer, font, theme);
    renderTabContent(renderer, font, theme);
    
    // Draw focus border if focused
    if (hasFocus) {
        auto focusColors = theme->focusColors();
        SDL_SetRenderDrawColor(renderer, focusColors.focusBorder.r, focusColors.focusBorder.g, 
                              focusColors.focusBorder.b, focusColors.focusBorder.a);
        SDL_Rect focusRect = {x - 1, y - 1, width + 2, height + 2};
        SDL_RenderDrawRect(renderer, &focusRect);
    }
}

void TabbedPanel::renderTabs(SDL_Renderer* renderer, TTF_Font* font, std::shared_ptr<Theme> theme) {
    if (tabs.empty()) return;
    
    auto colors = theme->buttonColors();
    int tabWidth = width / tabs.size();
    
    for (size_t i = 0; i < tabs.size(); ++i) {
        int tabX = x + (i * tabWidth);
        bool isActive = (i == activeTabIndex);
        
        // Draw tab background
        if (isActive) {
            SDL_SetRenderDrawColor(renderer, colors.buttonForeground.r, colors.buttonForeground.g, 
                                  colors.buttonForeground.b, colors.buttonForeground.a);
        } else {
            SDL_SetRenderDrawColor(renderer, colors.buttonBackground.r, colors.buttonBackground.g, 
                                  colors.buttonBackground.b, colors.buttonBackground.a);
        }
        
        SDL_Rect tabRect = {tabX, y, tabWidth, tabHeight};
        SDL_RenderFillRect(renderer, &tabRect);
        
        // Draw tab border
        SDL_SetRenderDrawColor(renderer, colors.buttonBorderDark.r, colors.buttonBorderDark.g, 
                              colors.buttonBorderDark.b, colors.buttonBorderDark.a);
        SDL_RenderDrawRect(renderer, &tabRect);
        
        // Draw tab text
        SDL_Color textColor = isActive ? 
            SDL_Color{colors.buttonBackground.r, colors.buttonBackground.g, colors.buttonBackground.b, colors.buttonBackground.a} :
            SDL_Color{colors.buttonText.r, colors.buttonText.g, colors.buttonText.b, colors.buttonText.a};
            
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

void TabbedPanel::handleEvent(const SDL_Event &e) {
    if (!visible) return;
    
    if (e.type == SDL_MOUSEBUTTONDOWN) {
        if (e.button.button == SDL_BUTTON_LEFT) {
            int tabIndex = getTabAtPosition(e.button.x, e.button.y);
            if (tabIndex >= 0) {
                setActiveTab(tabIndex);
                if (coreRef) {
                    coreRef->setFocus(getId());
                }
            }
        }
    }
}

int TabbedPanel::addTab(const std::string& title) {
    Tab newTab;
    newTab.title = title;
    newTab.active = tabs.empty(); // First tab is active by default
    tabs.push_back(newTab);
    
    if (tabs.size() == 1) {
        activeTabIndex = 0;
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
        
        if (onTabChange) {
            onTabChange(activeTabIndex);
        }
    }
}

void TabbedPanel::updateChildVisibility() {
    for (size_t i = 0; i < tabs.size(); ++i) {
        bool shouldBeVisible = (i == activeTabIndex);
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
