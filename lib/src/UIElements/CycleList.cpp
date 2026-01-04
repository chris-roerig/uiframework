#include "uiframework/UIElements/CycleList.h"
#include "uiframework/Theme/ThemeBase.h"
#include "uiframework/UICore.h"
#include "uiframework/Helpers.h"
#include <algorithm>

namespace ui {

CycleList::CycleList(int x, int y, int w, int h, const std::vector<std::string>& items_)
    : InteractiveElement(x, y, w, h), items(items_) {
    if (items.empty()) {
        items.push_back(""); // Ensure at least one empty item
    }
}

void CycleList::renderImpl(const RenderContext& ctx) {
    if (!visible) return;
    
    auto colors = ctx.cycleListColors();
    
    // Draw main background
    SDL_SetRenderDrawColor(ctx.renderer, colors.cycleListBackground.r, colors.cycleListBackground.g, 
                          colors.cycleListBackground.b, colors.cycleListBackground.a);
    SDL_Rect bgRect = {x, y, width, height};
    SDL_RenderFillRect(ctx.renderer, &bgRect);
    
    // Draw borders (top, left, right normal thickness)
    SDL_SetRenderDrawColor(ctx.renderer, colors.cycleListBorder.r, colors.cycleListBorder.g, 
                          colors.cycleListBorder.b, colors.cycleListBorder.a);
    
    // Top border
    SDL_RenderDrawLine(ctx.renderer, x, y, x + width - 1, y);
    // Left border  
    SDL_RenderDrawLine(ctx.renderer, x, y, x, y + height - 1);
    // Right border
    SDL_RenderDrawLine(ctx.renderer, x + width - 1, y, x + width - 1, y + height - 1);
    
    // Draw thicker bottom border (2px)
    SDL_RenderDrawLine(ctx.renderer, x, y + height - 1, x + width - 1, y + height - 1);
    SDL_RenderDrawLine(ctx.renderer, x, y + height - 2, x + width - 1, y + height - 2);
    
    // Render current value text
    if (!items.empty()) {
        // Calculate indicator area for text color inversion
        int segmentWidth = items.size() > 1 ? width / items.size() : 0;
        int indicatorX = items.size() > 1 ? x + (selectedIndex * segmentWidth) : 0;
        int indicatorY = y + height - 6;
        int indicatorHeight = 4;
        
        auto* textEntry = getCachedText("current_value", items[selectedIndex], 
                                       {colors.cycleListText.r, colors.cycleListText.g, colors.cycleListText.b, colors.cycleListText.a}, 
                                       ctx.renderer, ctx.font);
        if (textEntry && textEntry->texture) {
            int textX = x + 5; // Small padding
            int textY = y + (height - textEntry->height) / 2;
            SDL_Rect textRect = {textX, textY, textEntry->width, textEntry->height};
            SDL_RenderCopy(ctx.renderer, textEntry->texture, nullptr, &textRect);
            
            // Render inverted text over indicator area if text overlaps
            if (items.size() > 1 && textX < indicatorX + segmentWidth && textX + textEntry->width > indicatorX &&
                textY < indicatorY + indicatorHeight && textY + textEntry->height > indicatorY) {
                
                auto* invertedTextEntry = getCachedText("inverted_value", items[selectedIndex], 
                                                       {colors.cycleListBackground.r, colors.cycleListBackground.g, colors.cycleListBackground.b, colors.cycleListBackground.a}, 
                                                       ctx.renderer, ctx.font);
                if (invertedTextEntry && invertedTextEntry->texture) {
                    // Calculate intersection area
                    int clipX = std::max(textX, indicatorX);
                    int clipY = std::max(textY, indicatorY);
                    int clipW = std::min(textX + textEntry->width, indicatorX + segmentWidth) - clipX;
                    int clipH = std::min(textY + textEntry->height, indicatorY + indicatorHeight) - clipY;
                    
                    if (clipW > 0 && clipH > 0) {
                        SDL_Rect srcRect = {clipX - textX, clipY - textY, clipW, clipH};
                        SDL_Rect dstRect = {clipX, clipY, clipW, clipH};
                        SDL_RenderCopy(ctx.renderer, invertedTextEntry->texture, &srcRect, &dstRect);
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
        SDL_SetRenderDrawColor(ctx.renderer, colors.cycleListIndicator.r, colors.cycleListIndicator.g, 
                              colors.cycleListIndicator.b, colors.cycleListIndicator.a);
        SDL_Rect indicatorRect = {indicatorX, indicatorY, segmentWidth, indicatorHeight};
        SDL_RenderFillRect(ctx.renderer, &indicatorRect);
    }
    
    // Draw focus border if focused
    if (hasFocus) {
        auto focusColors = ctx.theme->focusColors();
        SDL_SetRenderDrawColor(ctx.renderer, focusColors.focusBorder.r, focusColors.focusBorder.g, 
                              focusColors.focusBorder.b, focusColors.focusBorder.a);
        SDL_Rect focusRect = {x - 1, y - 1, width + 2, height + 2};
        SDL_RenderDrawRect(ctx.renderer, &focusRect);
    }
}

void CycleList::onMouseDown(int x, int y) {
    // Focus handled automatically by InteractiveElement
}

void CycleList::onKeyDown(const SDL_Keycode& key) {
    switch (key) {
        case SDLK_UP:
            selectPrevious();
            break;
        case SDLK_DOWN:
            selectNext();
            break;
    }
}

void CycleList::activate() {
    // CycleList activates by cycling to next item
    selectNext();
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
    
    if (onChange && coreRef) {
        int currentIndex = selectedIndex;
        std::string currentValue = items[selectedIndex];
        coreRef->queueCallback([this, currentIndex, currentValue]() {
            if (onChange) {
                try {
                    onChange(currentIndex, currentValue);
                } catch (const std::exception& e) {
                    std::cerr << "Error in cycle list callback: " << e.what() << std::endl;
                }
            }
        });
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
    
    if (onChange && coreRef) {
        int currentIndex = selectedIndex;
        std::string currentValue = items[selectedIndex];
        coreRef->queueCallback([this, currentIndex, currentValue]() {
            if (onChange) {
                try {
                    onChange(currentIndex, currentValue);
                } catch (const std::exception& e) {
                    std::cerr << "Error in cycle list callback: " << e.what() << std::endl;
                }
            }
        });
    }
}

void CycleList::selectPrevious() {
    if (items.empty()) return;
    
    selectedIndex = (selectedIndex - 1 + items.size()) % items.size();
    invalidateTextCache();
    
    if (onChange && coreRef) {
        int currentIndex = selectedIndex;
        std::string currentValue = items[selectedIndex];
        coreRef->queueCallback([this, currentIndex, currentValue]() {
            if (onChange) {
                try {
                    onChange(currentIndex, currentValue);
                } catch (const std::exception& e) {
                    std::cerr << "Error in cycle list callback: " << e.what() << std::endl;
                }
            }
        });
    }
}

} // namespace ui
