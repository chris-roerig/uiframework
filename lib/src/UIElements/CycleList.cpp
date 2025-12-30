#include "uiframework/UIElements/CycleList.h"
#include "uiframework/Theme/ThemeBase.h"
#include "uiframework/UICore.h"
#include "uiframework/Helpers.h"
#include <algorithm>

namespace ui {

CycleList::CycleList(int x, int y, int w, int h, const std::vector<std::string>& items_)
    : UIElement(x, y, w, h), items(items_) {
    if (items.empty()) {
        items.push_back(""); // Ensure at least one empty item
    }
}

void CycleList::render(SDL_Renderer* renderer, TTF_Font* font, std::shared_ptr<Theme> theme) {
    if (!visible) return;
    
    auto colors = theme->cycleListColors();
    
    // Draw main background
    SDL_SetRenderDrawColor(renderer, colors.cycleListBackground.r, colors.cycleListBackground.g, 
                          colors.cycleListBackground.b, colors.cycleListBackground.a);
    SDL_Rect bgRect = {x, y, width, height};
    SDL_RenderFillRect(renderer, &bgRect);
    
    // Draw borders (top, left, right normal thickness)
    SDL_SetRenderDrawColor(renderer, colors.cycleListBorder.r, colors.cycleListBorder.g, 
                          colors.cycleListBorder.b, colors.cycleListBorder.a);
    
    // Top border
    SDL_RenderDrawLine(renderer, x, y, x + width - 1, y);
    // Left border  
    SDL_RenderDrawLine(renderer, x, y, x, y + height - 1);
    // Right border
    SDL_RenderDrawLine(renderer, x + width - 1, y, x + width - 1, y + height - 1);
    
    // Draw thicker bottom border (2px)
    SDL_RenderDrawLine(renderer, x, y + height - 1, x + width - 1, y + height - 1);
    SDL_RenderDrawLine(renderer, x, y + height - 2, x + width - 1, y + height - 2);
    
    // Render current value text
    if (!items.empty()) {
        // Calculate indicator area for text color inversion
        int segmentWidth = items.size() > 1 ? width / items.size() : 0;
        int indicatorX = items.size() > 1 ? x + (selectedIndex * segmentWidth) : 0;
        int indicatorY = y + height - 6;
        int indicatorHeight = 4;
        
        auto* textEntry = getCachedText("current_value", items[selectedIndex], 
                                       {colors.cycleListText.r, colors.cycleListText.g, colors.cycleListText.b, colors.cycleListText.a}, 
                                       renderer, font);
        if (textEntry && textEntry->texture) {
            int textX = x + 5; // Small padding
            int textY = y + (height - textEntry->height) / 2;
            SDL_Rect textRect = {textX, textY, textEntry->width, textEntry->height};
            SDL_RenderCopy(renderer, textEntry->texture, nullptr, &textRect);
            
            // Render inverted text over indicator area if text overlaps
            if (items.size() > 1 && textX < indicatorX + segmentWidth && textX + textEntry->width > indicatorX &&
                textY < indicatorY + indicatorHeight && textY + textEntry->height > indicatorY) {
                
                auto* invertedTextEntry = getCachedText("inverted_value", items[selectedIndex], 
                                                       {colors.cycleListBackground.r, colors.cycleListBackground.g, colors.cycleListBackground.b, colors.cycleListBackground.a}, 
                                                       renderer, font);
                if (invertedTextEntry && invertedTextEntry->texture) {
                    // Calculate intersection area
                    int clipX = std::max(textX, indicatorX);
                    int clipY = std::max(textY, indicatorY);
                    int clipW = std::min(textX + textEntry->width, indicatorX + segmentWidth) - clipX;
                    int clipH = std::min(textY + textEntry->height, indicatorY + indicatorHeight) - clipY;
                    
                    if (clipW > 0 && clipH > 0) {
                        SDL_Rect srcRect = {clipX - textX, clipY - textY, clipW, clipH};
                        SDL_Rect dstRect = {clipX, clipY, clipW, clipH};
                        SDL_RenderCopy(renderer, invertedTextEntry->texture, &srcRect, &dstRect);
                    }
                }
            }
        }
    }
    
    // Draw position indicator
    if (items.size() > 1) {
        int segmentWidth = width / items.size();
        int indicatorX = x + (selectedIndex * segmentWidth);
        int indicatorY = y + height - 6;
        int indicatorHeight = 4;
        
        // Draw indicator background with border color
        SDL_SetRenderDrawColor(renderer, colors.cycleListIndicator.r, colors.cycleListIndicator.g, 
                              colors.cycleListIndicator.b, colors.cycleListIndicator.a);
        SDL_Rect indicatorRect = {indicatorX, indicatorY, segmentWidth, indicatorHeight};
        SDL_RenderFillRect(renderer, &indicatorRect);
    }
    
    // Draw focus border if focused
    if (hasFocus) {
        auto focusColors = theme->focusColors();
        SDL_SetRenderDrawColor(renderer, focusColors.focusBorder.r, focusColors.focusBorder.g, 
                              focusColors.focusBorder.b, focusColors.focusBorder.a);
        SDL_Rect focusRect = {x - 1, y - 1, width + 2, height + 2};
        SDL_RenderDrawRect(renderer, &focusRect);
    }
}

void CycleList::handleEvent(const SDL_Event &e) {
    if (!visible) return;
    
    if (e.type == SDL_MOUSEBUTTONDOWN) {
        if (e.button.button == SDL_BUTTON_LEFT && containsPoint(e.button.x, e.button.y)) {
            if (coreRef) {
                coreRef->setFocus(getId());
            }
        }
    }
    
    if (!hasFocus) return;
    
    if (e.type == SDL_KEYDOWN) {
        switch (e.key.keysym.sym) {
            case SDLK_UP:
                selectPrevious();
                break;
            case SDLK_DOWN:
                selectNext();
                break;
        }
    }
}

void CycleList::setItems(const std::vector<std::string>& newItems) {
    items = newItems;
    if (items.empty()) {
        items.push_back("");
    }
    selectedIndex = std::min(selectedIndex, static_cast<int>(items.size()) - 1);
    invalidateTextCache();
}

void CycleList::setSelectedIndex(int index) {
    if (items.empty()) return;
    
    selectedIndex = std::max(0, std::min(index, static_cast<int>(items.size()) - 1));
    invalidateTextCache();
    
    if (onChange) {
        onChange(selectedIndex, items[selectedIndex]);
    }
}

const std::string& CycleList::getSelectedValue() const {
    static const std::string empty = "";
    return items.empty() ? empty : items[selectedIndex];
}

void CycleList::selectNext() {
    if (items.empty()) return;
    
    selectedIndex = (selectedIndex + 1) % items.size();
    invalidateTextCache();
    
    if (onChange) {
        onChange(selectedIndex, items[selectedIndex]);
    }
}

void CycleList::selectPrevious() {
    if (items.empty()) return;
    
    selectedIndex = (selectedIndex - 1 + items.size()) % items.size();
    invalidateTextCache();
    
    if (onChange) {
        onChange(selectedIndex, items[selectedIndex]);
    }
}

} // namespace ui
