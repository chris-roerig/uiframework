#include "ListView.h"
#include "ThemeGlobals.h"
#include "ThemeBase.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <algorithm>
#include <cctype>
#include <iostream>

namespace ui {

// Helper: convert string to lowercase.
static std::string toLower(const std::string &s) {
    std::string ret = s;
    for (char &ch : ret)
        ch = static_cast<char>(std::tolower(ch));
    return ret;
}

int ListView::getSelectedIndex() const {
    return selectedIndex;
}

std::string ListView::getSelectedItem() const {
    // Use filteredItems if a filter is active; otherwise use full items list.
    const std::vector<std::string>& list = (!filteredItems.empty()) ? filteredItems : items;
    if (selectedIndex >= 0 && selectedIndex < static_cast<int>(list.size()))
        return list[selectedIndex];
    return "";
}

void ListView::setSelectedIndex(int index) {
    const std::vector<std::string>& list = (!filteredItems.empty()) ? filteredItems : items;
    if (index >= 0 && index < static_cast<int>(list.size())) {
        selectedIndex = index;
        // Optionally notify via callback.
        if (onSelectionChange)
            onSelectionChange({ selectedIndex });
    }
}

ListView::ListView(int x_, int y_, int w_, int h_, int itemHeight_)
    : UIElement(x_, y_, w_, h_), itemHeight(itemHeight_)
{
    // Initially, no filter applied: filteredItems is same as items.
    filteredItems = items;
}

SDL_Rect ListView::getFocusRect() const {
    return SDL_Rect{ x, y, width, height };
}

void ListView::render(SDL_Renderer* renderer) {
    // Use filteredItems if available; if filter applied and no items match, we draw nothing.
    const std::vector<std::string>& list = (!filteredItems.empty()) ? filteredItems : items;
    
    // Draw the background (using the theme's label background, for example).
    SDL_Rect bgRect = { x, y, width, height };
    drawFilledRect(renderer, bgRect, g_currentTheme->labelColors().labelBackground);
    
    // Determine how many items fit in the view.
    int visibleCount = height / itemHeight;
    int totalItems = list.size();
    
    // Adjust scrollOffset if necessary.
    if (scrollOffset > totalItems - visibleCount)
        scrollOffset = std::max(0, totalItems - visibleCount);
    
    // Render visible items.
    for (int i = 0; i < visibleCount && (i + scrollOffset) < totalItems; i++) {
        int itemIndex = i + scrollOffset;
        SDL_Rect itemRect = { x, y + i * itemHeight, width, itemHeight };
        
        // Determine selection background.
        bool isSelected = false;
        if (!multiSelect) {
            isSelected = (itemIndex == selectedIndex);
        } else {
            isSelected = (std::find(selectedIndices.begin(), selectedIndices.end(), itemIndex) != selectedIndices.end());
        }
        
        // Use OptionSelect colors for selection.
        if (isSelected)
            drawFilledRect(renderer, itemRect, g_currentTheme->optionSelectColors().selectOptionSelected);
        else
            drawFilledRect(renderer, itemRect, g_currentTheme->optionSelectColors().selectOptionUnselected);
        
        // Render text for the item (left-justified with a small padding).
        if (globalFont) {
            SDL_Color textColor = { 0, 0, 0, 255 };  // Black text for example.
            SDL_Surface* surface = TTF_RenderText_Solid(globalFont, list[itemIndex].c_str(), textColor);
            if (surface) {
                SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
                int textW = surface->w, textH = surface->h;
                SDL_FreeSurface(surface);
                int padding = 5;
                SDL_Rect dst = { x + padding, y + i * itemHeight + (itemHeight - textH) / 2, textW, textH };
                SDL_RenderCopy(renderer, texture, nullptr, &dst);
                SDL_DestroyTexture(texture);
            }
        }
    }
    
    // --- Draw the Scrollbar (if necessary) ---
    if (totalItems > visibleCount) {
        // Set scrollbar width (for example, 10 pixels)
        int scrollbarWidth = 10;
        
        // Draw the scrollbar track on the right.
        SDL_Rect trackRect = { x + width - scrollbarWidth, y, scrollbarWidth, height };
        Color trackColor = g_currentTheme->listViewColors().listViewScrollbarBackground; // Assumed new theme property
        drawFilledRect(renderer, trackRect, trackColor);
        
        // Calculate the thumb (handle) height.
        float proportionVisible = (float)visibleCount / totalItems;
        int thumbHeight = std::max(10, (int)(height * proportionVisible));
        
        // Calculate the thumb position relative to scrollOffset.
        float proportionOffset = (float)scrollOffset / (totalItems - visibleCount);
        int thumbY = y + (int)((height - thumbHeight) * proportionOffset);
        SDL_Rect thumbRect = { x + width - scrollbarWidth, thumbY, scrollbarWidth, thumbHeight };
        Color thumbColor = g_currentTheme->listViewColors().listViewScrollbarThumb; // Assumed new theme property
        drawFilledRect(renderer, thumbRect, thumbColor);
    }
}


void ListView::handleEvent(const SDL_Event &e) {
    // We'll handle keyboard navigation only.
    // Use filteredItems if not empty, else items.
    const std::vector<std::string>& list = (!filteredItems.empty()) ? filteredItems : items;
    int totalItems = list.size();
    if (totalItems == 0)
        return;
    
    if (e.type == SDL_KEYDOWN) {
        switch (e.key.keysym.sym) {
            case SDLK_UP:
                if (!multiSelect) {
                    int oldIndex = selectedIndex;
                    selectedIndex = (selectedIndex - 1 + totalItems) % totalItems;
                    if (selectedIndex > oldIndex) {
                        // Wrapped around from the first element to the last: scroll so that the last page of items is visible.
                        scrollOffset = std::max(0, totalItems - (height / itemHeight));
                    } else if (selectedIndex < scrollOffset) {
                        scrollOffset = selectedIndex;
                    }
                } else {
                    selectedIndex = (selectedIndex - 1 + totalItems) % totalItems;
                }
                if (onSelectionChange) {
                    if (!multiSelect)
                        onSelectionChange({selectedIndex});
                    else
                        onSelectionChange(selectedIndices);
                }
                break;
            case SDLK_DOWN:
                if (!multiSelect) {
                    int oldIndex = selectedIndex;
                    selectedIndex = (selectedIndex + 1) % totalItems;
                    if (selectedIndex < oldIndex) {
                        // Wrapped around from last to first: reset scroll offset to show the first item at the top.
                        scrollOffset = 0;
                    } else if (selectedIndex >= scrollOffset + (height / itemHeight)) {
                        scrollOffset = selectedIndex - (height / itemHeight) + 1;
                    }
                } else {
                    selectedIndex = (selectedIndex + 1) % totalItems;
                }
                if (onSelectionChange) {
                    if (!multiSelect)
                        onSelectionChange({selectedIndex});
                    else
                        onSelectionChange(selectedIndices);
                }
                break;
            case SDLK_PAGEUP:
                selectedIndex = std::max(0, selectedIndex - (height / itemHeight));
                scrollOffset = std::max(0, scrollOffset - (height / itemHeight));
                if (onSelectionChange) {
                    if (!multiSelect)
                        onSelectionChange({selectedIndex});
                    else
                        onSelectionChange(selectedIndices);
                }
                break;
            case SDLK_PAGEDOWN:
                selectedIndex = std::min(totalItems - 1, selectedIndex + (height / itemHeight));
                scrollOffset = std::min(totalItems - (height / itemHeight), scrollOffset + (height / itemHeight));
                if (onSelectionChange) {
                    if (!multiSelect)
                        onSelectionChange({selectedIndex});
                    else
                        onSelectionChange(selectedIndices);
                }
                break;
            case SDLK_HOME:
                selectedIndex = 0;
                scrollOffset = 0;
                if (onSelectionChange) {
                    if (!multiSelect)
                        onSelectionChange({selectedIndex});
                    else
                        onSelectionChange(selectedIndices);
                }
                break;
            case SDLK_END:
                selectedIndex = totalItems - 1;
                scrollOffset = std::max(0, totalItems - (height / itemHeight));
                if (onSelectionChange) {
                    if (!multiSelect)
                        onSelectionChange({selectedIndex});
                    else
                        onSelectionChange(selectedIndices);
                }
                break;
            case SDLK_RETURN:
                if (onItemActivated)
                    onItemActivated(selectedIndex);
                break;
            default:
                break;
        }
    }
}

void ListView::applyFilter(const std::string &filter) {
    filteredItems.clear();
    std::string lowerFilter = toLower(filter);
    for (const auto &item : items) {
        if (toLower(item).find(lowerFilter) != std::string::npos) {
            filteredItems.push_back(item);
        }
    }
    // Reset selection and scroll.
    selectedIndex = 0;
    scrollOffset = 0;
}

void ListView::sortItems(std::function<bool(const std::string&, const std::string&)> comp) {
    std::sort(items.begin(), items.end(), comp);
    // Re-apply filter if active.
    if (!filteredItems.empty()) {
        applyFilter("");
    }
}

} // namespace ui
