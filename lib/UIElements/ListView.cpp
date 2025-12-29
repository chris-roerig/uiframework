#include "ListView.h"
#include "Theme/ThemeBase.h"
#include "Helpers.h"
#include "UICore.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <algorithm>
#include <cctype>

namespace ui {

ListView::ListView(int x_, int y_, int w_, int h_, const std::vector<std::string>& items_, int itemHeight_)
    : UIElement(x_, y_, w_, h_), items(items_), itemHeight(itemHeight_) {
    filteredItems = items; // Initially show all items
}

void ListView::render(SDL_Renderer* renderer, TTF_Font* font, std::shared_ptr<Theme> theme) {
    if (!renderer || !theme) {
        return;
    }
    
    ThemeableElementColors tc = theme->listViewColors();
    
    // Draw background
    SDL_Rect bgRect = { x, y, width, height };
    drawFilledRect(renderer, bgRect, tc.listViewBackground);
    
    // Draw border
    SDL_SetRenderDrawColor(renderer, tc.listViewBorder.r, tc.listViewBorder.g, 
                          tc.listViewBorder.b, tc.listViewBorder.a);
    SDL_RenderDrawRect(renderer, &bgRect);
    
    // Draw focus indicator
    if (hasFocus) {
        SDL_Rect focusRect = getFocusRect();
        SDL_SetRenderDrawColor(renderer, tc.listViewSelectedItem.r, tc.listViewSelectedItem.g, tc.listViewSelectedItem.b, tc.listViewSelectedItem.a);
        SDL_RenderDrawRect(renderer, &focusRect);
    }
    
    const auto& currentItems = getCurrentItems();
    int visibleItems = getVisibleItemCount();
    int maxScroll = std::max(0, static_cast<int>(currentItems.size()) - visibleItems);
    scrollOffset = std::clamp(scrollOffset, 0, maxScroll);
    
    // Set clipping rectangle
    SDL_RenderSetClipRect(renderer, &bgRect);
    
    // Render visible items
    for (int i = 0; i < visibleItems && (scrollOffset + i) < static_cast<int>(currentItems.size()); i++) {
        int itemIndex = scrollOffset + i;
        SDL_Rect itemRect = { x, y + i * itemHeight, width, itemHeight };
        
        // Determine item state
        bool isSelected = false;
        if (multiSelect) {
            isSelected = std::find(selectedIndices.begin(), selectedIndices.end(), itemIndex) != selectedIndices.end();
        } else {
            isSelected = (itemIndex == selectedIndex);
        }
        
        bool isHovered = (itemIndex == hoveredIndex);
        
        // Draw item background
        if (isSelected) {
            drawFilledRect(renderer, itemRect, tc.listViewSelectedItem);
        } else if (isHovered) {
            Color hoverColor = tc.listViewSelectedItem;
            hoverColor.a = 128; // Semi-transparent
            drawFilledRect(renderer, itemRect, hoverColor);
        }
        
        // Draw item text
        if (font && itemIndex < static_cast<int>(currentItems.size())) {
            const std::string& itemText = currentItems[itemIndex];
            if (!itemText.empty()) {
                SDL_Color textColor = { tc.listViewText.r, tc.listViewText.g, tc.listViewText.b, tc.listViewText.a };
                SDL_Surface* surface = TTF_RenderText_Solid(font, itemText.c_str(), textColor);
                if (surface) {
                    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
                    if (texture) {
                        SDL_Rect textRect = { 
                            x + 5, 
                            y + i * itemHeight + (itemHeight - surface->h) / 2, 
                            std::min(surface->w, width - 10), 
                            surface->h 
                        };
                        SDL_RenderCopy(renderer, texture, nullptr, &textRect);
                        SDL_DestroyTexture(texture);
                    }
                    SDL_FreeSurface(surface);
                }
            }
        }
        
        // Draw item separator
        SDL_SetRenderDrawColor(renderer, tc.listViewBorder.r, tc.listViewBorder.g, 
                              tc.listViewBorder.b, tc.listViewBorder.a);
        SDL_RenderDrawLine(renderer, x, y + (i + 1) * itemHeight, x + width, y + (i + 1) * itemHeight);
    }
    
    // Reset clipping
    SDL_RenderSetClipRect(renderer, nullptr);
    
    // Draw scrollbar if needed
    if (static_cast<int>(currentItems.size()) > visibleItems) {
        int scrollbarWidth = 10;
        int scrollbarX = x + width - scrollbarWidth;
        SDL_Rect scrollbarTrack = { scrollbarX, y, scrollbarWidth, height };
        drawFilledRect(renderer, scrollbarTrack, tc.listViewBackground);
        
        // Calculate scrollbar thumb
        float thumbRatio = static_cast<float>(visibleItems) / currentItems.size();
        int thumbHeight = std::max(10, static_cast<int>(height * thumbRatio));
        float scrollRatio = static_cast<float>(scrollOffset) / (currentItems.size() - visibleItems);
        int thumbY = y + static_cast<int>((height - thumbHeight) * scrollRatio);
        
        SDL_Rect scrollbarThumb = { scrollbarX, thumbY, scrollbarWidth, thumbHeight };
        drawFilledRect(renderer, scrollbarThumb, tc.listViewSelectedItem);
    }
}

void ListView::handleEvent(const SDL_Event &e) {
    if (!visible) return;
    
    const auto& currentItems = getCurrentItems();
    
    if (e.type == SDL_MOUSEBUTTONDOWN) {
        if (e.button.button == SDL_BUTTON_LEFT) {
            int mouseX = e.button.x;
            int mouseY = e.button.y;
            
            if (containsPoint(mouseX, mouseY)) {
                if (coreRef) {
                    std::string elementIdCopy = elementId;
                    coreRef->queueCallback([this, elementIdCopy]() {
                        if (coreRef) {
                            coreRef->setFocus(elementIdCopy);
                        }
                    });
                }
                
                int clickedItem = getItemAt(mouseX, mouseY);
                if (clickedItem >= 0 && clickedItem < static_cast<int>(currentItems.size())) {
                    if (multiSelect && (e.button.clicks == 1)) {
                        // Toggle selection in multi-select mode
                        auto it = std::find(selectedIndices.begin(), selectedIndices.end(), clickedItem);
                        if (it != selectedIndices.end()) {
                            selectedIndices.erase(it);
                        } else {
                            selectedIndices.push_back(clickedItem);
                        }
                        if (onSelectionChange && coreRef) {
                            std::vector<int> currentSelection = selectedIndices;
                            coreRef->queueCallback([this, currentSelection]() {
                                if (onSelectionChange) {
                                    onSelectionChange(currentSelection);
                                }
                            });
                        }
                    } else {
                        // Single selection
                        setSelectedIndex(clickedItem);
                        if (e.button.clicks == 2 && onItemActivated && coreRef) {
                            int activatedItem = clickedItem;
                            coreRef->queueCallback([this, activatedItem]() {
                                if (onItemActivated) {
                                    onItemActivated(activatedItem);
                                }
                            });
                        }
                    }
                }
            }
        }
    } else if (e.type == SDL_MOUSEMOTION) {
        int mouseX = e.motion.x;
        int mouseY = e.motion.y;
        
        if (containsPoint(mouseX, mouseY)) {
            hoveredIndex = getItemAt(mouseX, mouseY);
        } else {
            hoveredIndex = -1;
        }
    } else if (e.type == SDL_KEYDOWN && hasFocus) {
        switch (e.key.keysym.sym) {
            case SDLK_UP:
                if (selectedIndex > 0) {
                    setSelectedIndex(selectedIndex - 1);
                    scrollToItem(selectedIndex);
                }
                break;
            case SDLK_DOWN:
                if (selectedIndex < static_cast<int>(currentItems.size()) - 1) {
                    setSelectedIndex(selectedIndex + 1);
                    scrollToItem(selectedIndex);
                }
                break;
            case SDLK_PAGEUP:
                scrollUp();
                break;
            case SDLK_PAGEDOWN:
                scrollDown();
                break;
            case SDLK_HOME:
                setSelectedIndex(0);
                scrollToItem(0);
                break;
            case SDLK_END:
                setSelectedIndex(static_cast<int>(currentItems.size()) - 1);
                scrollToItem(selectedIndex);
                break;
            case SDLK_RETURN:
                activate();
                break;
        }
    } else if (e.type == SDL_MOUSEWHEEL) {
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        if (containsPoint(mouseX, mouseY)) {
            if (e.wheel.y > 0) {
                scrollUp();
            } else if (e.wheel.y < 0) {
                scrollDown();
            }
        }
    }
}

SDL_Rect ListView::getFocusRect() const {
    return SDL_Rect{ x - 2, y - 2, width + 4, height + 4 };
}

void ListView::activate() {
    if (onItemActivated && selectedIndex >= 0 && selectedIndex < static_cast<int>(getCurrentItems().size()) && coreRef) {
        int currentSelectedIndex = selectedIndex;
        coreRef->queueCallback([this, currentSelectedIndex]() {
            if (onItemActivated && currentSelectedIndex >= 0 && currentSelectedIndex < static_cast<int>(getCurrentItems().size())) {
                onItemActivated(currentSelectedIndex);
            }
        });
    }
}

void ListView::setItems(const std::vector<std::string>& newItems) {
    items = newItems;
    filteredItems = items;
    clearSelection();
}

void ListView::addItem(const std::string& item) {
    items.push_back(item);
    if (filteredItems.size() == items.size() - 1) { // No filter applied
        filteredItems.push_back(item);
    }
}

void ListView::removeItem(int index) {
    if (index >= 0 && index < static_cast<int>(items.size())) {
        items.erase(items.begin() + index);
        // Rebuild filtered items
        // This is simplified - in a real implementation you'd want to maintain the filter
        filteredItems = items;
        clearSelection();
    }
}

void ListView::clearItems() {
    items.clear();
    filteredItems.clear();
    clearSelection();
}

void ListView::setSelectedIndex(int index) {
    const auto& currentItems = getCurrentItems();
    if (index >= 0 && index < static_cast<int>(currentItems.size())) {
        selectedIndex = index;
        if (!multiSelect) {
            selectedIndices = {index};
        }
        if (onSelectionChange && coreRef) {
            std::vector<int> currentSelection = selectedIndices;
            coreRef->queueCallback([this, currentSelection]() {
                if (onSelectionChange) {
                    onSelectionChange(currentSelection);
                }
            });
        }
    }
}

void ListView::clearSelection() {
    selectedIndex = 0;
    selectedIndices.clear();
    scrollOffset = 0;
}

void ListView::applyFilter(const std::string &filter) {
    filteredItems.clear();
    
    if (filter.empty()) {
        filteredItems = items;
    } else {
        std::string lowerFilter = filter;
        std::transform(lowerFilter.begin(), lowerFilter.end(), lowerFilter.begin(), ::tolower);
        
        for (const auto& item : items) {
            std::string lowerItem = item;
            std::transform(lowerItem.begin(), lowerItem.end(), lowerItem.begin(), ::tolower);
            if (lowerItem.find(lowerFilter) != std::string::npos) {
                filteredItems.push_back(item);
            }
        }
    }
    
    clearSelection();
}

void ListView::clearFilter() {
    filteredItems = items;
    clearSelection();
}

void ListView::scrollToItem(int index) {
    int visibleItems = getVisibleItemCount();
    if (index < scrollOffset) {
        scrollOffset = index;
    } else if (index >= scrollOffset + visibleItems) {
        scrollOffset = index - visibleItems + 1;
    }
}

void ListView::scrollUp() {
    scrollOffset = std::max(0, scrollOffset - 1);
}

void ListView::scrollDown() {
    const auto& currentItems = getCurrentItems();
    int maxScroll = std::max(0, static_cast<int>(currentItems.size()) - getVisibleItemCount());
    scrollOffset = std::min(maxScroll, scrollOffset + 1);
}

bool ListView::isItemVisible(int index) const {
    int visibleItems = getVisibleItemCount();
    return index >= scrollOffset && index < scrollOffset + visibleItems;
}

int ListView::getItemAt(int mouseX, int mouseY) const {
    if (!containsPoint(mouseX, mouseY)) {
        return -1;
    }
    
    int relativeY = mouseY - y;
    int itemIndex = scrollOffset + (relativeY / itemHeight);
    
    const auto& currentItems = getCurrentItems();
    if (itemIndex >= 0 && itemIndex < static_cast<int>(currentItems.size())) {
        return itemIndex;
    }
    
    return -1;
}

const std::vector<std::string>& ListView::getCurrentItems() const {
    return filteredItems.empty() ? items : filteredItems;
}

} // namespace ui
