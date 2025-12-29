#include "OptionSelect.h"
#include "Theme/ThemeBase.h"
#include "Helpers.h"
#include "UICore.h"
#include "../Constants.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <algorithm>

namespace ui {

std::string OptionSelect::getCachedTruncatedText(const std::string& text, TTF_Font* font, int availableWidth, StringCache& cache) const {
    // Check if cache is valid
    if (cache.valid && cache.originalText == text && cache.availableWidth == availableWidth) {
        return cache.truncatedText;
    }
    
    // Cache miss - calculate truncated text
    cache.originalText = text;
    cache.availableWidth = availableWidth;
    cache.truncatedText = truncateText(text, font, availableWidth);
    cache.valid = true;
    
    return cache.truncatedText;
}

void OptionSelect::invalidateStringCache() {
    displayCache.valid = false;
    for (auto& cache : dropdownCache) {
        cache.valid = false;
    }
}

void OptionSelect::setOptions(const std::vector<std::string>& newOptions) {
    options = newOptions;
    invalidateStringCache();
    if (!isValidIndex(currentIndex)) {
        currentIndex = options.empty() ? -1 : 0;
    }
}

void OptionSelect::addOption(const std::string& option) {
    options.push_back(option);
    if (currentIndex == -1 && !options.empty()) {
        currentIndex = 0;
    }
    invalidateStringCache();
}

void OptionSelect::removeOption(int index) {
    if (isValidIndex(index)) {
        options.erase(options.begin() + index);
        
        // Adjust currentIndex based on what was removed
        if (currentIndex > index) {
            // Current selection was after removed item - shift down
            currentIndex--;
        } else if (currentIndex == index) {
            // Current selection was removed - keep same index if valid, otherwise adjust
            if (currentIndex >= static_cast<int>(options.size())) {
                currentIndex = options.empty() ? -1 : static_cast<int>(options.size()) - 1;
            }
        }
        // If currentIndex < index, no change needed
        
        invalidateStringCache();
    }
}

std::string OptionSelect::truncateText(const std::string& text, TTF_Font* font, int availableWidth) const {
    if (!font || availableWidth <= 0) return "";
    
    int textW = 0;
    if (TTF_SizeText(font, text.c_str(), &textW, nullptr) == 0 && textW <= availableWidth) {
        return text;
    }
    
    std::string ellipsis = "...";
    int ellipsisW = 0;
    if (TTF_SizeText(font, ellipsis.c_str(), &ellipsisW, nullptr) != 0 || ellipsisW >= availableWidth) {
        return "";
    }
    
    // Binary search for optimal truncation point
    int left = 0, right = static_cast<int>(text.length());
    while (left < right) {
        int mid = (left + right + 1) / 2;
        std::string candidate = text.substr(0, mid) + ellipsis;
        int candidateW = 0;
        if (TTF_SizeText(font, candidate.c_str(), &candidateW, nullptr) == 0 && candidateW <= availableWidth) {
            left = mid;
        } else {
            right = mid - 1;
        }
    }
    
    return left > 0 ? text.substr(0, left) + ellipsis : "";
}

bool OptionSelect::isValidIndex(int index) const {
    return index >= 0 && index < static_cast<int>(options.size());
}

void OptionSelect::renderCollapsed(SDL_Renderer* renderer, TTF_Font* font, std::shared_ptr<Theme> theme) {
    const int padding = Constants::DEFAULT_PADDING;
    const int arrowWidth = Constants::ARROW_WIDTH;
    
    ThemeableElementColors tc = theme->optionSelectColors();
    
    // Draw selection box
    SDL_Rect cellRect = { x, y, width, height };
    drawFilledRect(renderer, cellRect, tc.selectOptionUnselected);
    
    // Draw border
    SDL_SetRenderDrawColor(renderer, tc.selectOptionSelected.r, tc.selectOptionSelected.g, 
                           tc.selectOptionSelected.b, tc.selectOptionSelected.a);
    SDL_RenderDrawRect(renderer, &cellRect);
    
    // Draw focus indicator
    if (hasFocus) {
        SDL_Rect focusRect = getFocusRect();
        SDL_SetRenderDrawColor(renderer, tc.selectOptionSelected.r, tc.selectOptionSelected.g, 
                               tc.selectOptionSelected.b, tc.selectOptionSelected.a);
        SDL_RenderDrawRect(renderer, &focusRect);
    }
    
    // Render current selection text
    if (font && isValidIndex(currentIndex)) {
        int availableWidth = width - 2 * padding - arrowWidth;
        std::string displayText = getCachedTruncatedText(options[currentIndex], font, availableWidth, displayCache);
        
        if (!displayText.empty()) {
            SDL_Color textColor = { tc.selectOptionTextUnselected.r, tc.selectOptionTextUnselected.g, 
                                    tc.selectOptionTextUnselected.b, tc.selectOptionTextUnselected.a };
            
            std::string cacheKey = "option_" + std::to_string(currentIndex);
            TextCacheEntry* cached = getCachedText(cacheKey, displayText, textColor, renderer, font);
            if (cached && cached->texture) {
                SDL_Rect dst = { 
                    x + padding, 
                    y + (height - cached->height) / 2, 
                    cached->width, 
                    cached->height 
                };
                SDL_RenderCopy(renderer, cached->texture, nullptr, &dst);
            }
        }
    }
    
    // Draw dropdown arrow
    int arrowX = x + width - arrowWidth + 2;
    int arrowY = y + height / 2;
    SDL_SetRenderDrawColor(renderer, tc.selectOptionSelected.r, tc.selectOptionSelected.g, 
                           tc.selectOptionSelected.b, tc.selectOptionSelected.a);
    // Simple triangle pointing down
    for (int i = 0; i < 5; i++) {
        SDL_RenderDrawLine(renderer, 
            arrowX + i, arrowY - 2 + i,
            arrowX + 8 - i, arrowY - 2 + i);
    }
}

void OptionSelect::renderExpanded(SDL_Renderer* renderer, TTF_Font* font, std::shared_ptr<Theme> theme) {
    const int padding = Constants::DEFAULT_PADDING;
    int itemHeight = height;
    int totalHeight = itemHeight * static_cast<int>(options.size());
    
    ThemeableElementColors tc = theme->optionSelectColors();
    
    // Draw dropdown background
    SDL_Rect dropdownRect = { x, y, width, totalHeight };
    drawFilledRect(renderer, dropdownRect, tc.selectOptionUnselected);
    SDL_SetRenderDrawColor(renderer, tc.selectOptionSelected.r, tc.selectOptionSelected.g, 
                           tc.selectOptionSelected.b, tc.selectOptionSelected.a);
    SDL_RenderDrawRect(renderer, &dropdownRect);
    
    // Draw each option
    for (int i = 0; i < static_cast<int>(options.size()); i++) {
        SDL_Rect itemRect = { x, y + i * itemHeight, width, itemHeight };
        
        // Highlight current, selected, or hovered item
        if (i == currentIndex) {
            drawFilledRect(renderer, itemRect, tc.selectOptionSelected);
        } else if (i == hoveredIndex) {
            Color hoverColor = tc.selectOptionSelected;
            hoverColor.a = 128; // Semi-transparent
            drawFilledRect(renderer, itemRect, hoverColor);
        }
        
        // Draw item border
        SDL_SetRenderDrawColor(renderer, tc.selectOptionSelected.r, tc.selectOptionSelected.g, 
                               tc.selectOptionSelected.b, tc.selectOptionSelected.a);
        SDL_RenderDrawRect(renderer, &itemRect);
        
        // Render option text
        if (font && !options[i].empty()) {
            // Ensure dropdown cache is properly sized
            if (dropdownCache.size() != options.size()) {
                dropdownCache.resize(options.size());
            }
            
            int availableWidth = width - 2 * padding;
            std::string displayText = getCachedTruncatedText(options[i], font, availableWidth, dropdownCache[i]);
            
            if (!displayText.empty()) {
                // Use selected text color for current/hovered items, unselected for others
                bool isHighlighted = (i == currentIndex || i == hoveredIndex);
                SDL_Color textColor = isHighlighted ? 
                    SDL_Color{ tc.selectOptionTextSelected.r, tc.selectOptionTextSelected.g, 
                              tc.selectOptionTextSelected.b, tc.selectOptionTextSelected.a } :
                    SDL_Color{ tc.selectOptionTextUnselected.r, tc.selectOptionTextUnselected.g, 
                              tc.selectOptionTextUnselected.b, tc.selectOptionTextUnselected.a };
                SDL_Surface* surface = TTF_RenderText_Solid(font, displayText.c_str(), textColor);
                if (surface) {
                    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
                    if (texture) {
                        SDL_Rect dst = { 
                            x + padding, 
                            y + i * itemHeight + (itemHeight - surface->h) / 2, 
                            surface->w, 
                            surface->h 
                        };
                        SDL_RenderCopy(renderer, texture, nullptr, &dst);
                        SDL_DestroyTexture(texture);
                    } else {
                        SDL_FreeSurface(surface);
                        continue;
                    }
                    SDL_FreeSurface(surface);
                }
            }
        }
    }
}

void OptionSelect::render(SDL_Renderer* renderer, TTF_Font* font, std::shared_ptr<Theme> theme) {
    if (!renderer || !theme || options.empty()) {
        return;
    }
    
    // Check if dimensions changed and invalidate cache if needed
    if (width != lastWidth || height != lastHeight) {
        invalidateStringCache();
        lastWidth = width;
        lastHeight = height;
    }
    
    if (expanded) {
        renderExpanded(renderer, font, theme);
    } else {
        renderCollapsed(renderer, font, theme);
    }
}

void OptionSelect::handleEvent(const SDL_Event &e) {
    if (!visible || options.empty()) return;
    
    if (e.type == SDL_MOUSEBUTTONDOWN) {
        if (e.button.button == SDL_BUTTON_LEFT) {
            int mouseX = e.button.x;
            int mouseY = e.button.y;
            
            if (!expanded) {
                // Click on collapsed dropdown
                if (containsPoint(mouseX, mouseY)) {
                    expand();
                    if (coreRef && !hasFocus) {
                        coreRef->setFocus(elementId);
                    }
                }
            } else {
                // Click on expanded dropdown
                int itemHeight = height;
                int totalHeight = itemHeight * static_cast<int>(options.size());
                
                if (mouseX >= x && mouseX < x + width &&
                    mouseY >= y && mouseY < y + totalHeight) {
                    // Click inside dropdown
                    int clickedIndex = (mouseY - y) / itemHeight;
                    if (isValidIndex(clickedIndex)) {
                        setSelectedIndex(clickedIndex);
                        collapse();
                    }
                } else {
                    // Click outside dropdown - collapse
                    collapse();
                }
            }
        }
    } else if (e.type == SDL_MOUSEMOTION && expanded) {
        // Update hovered item
        int mouseX = e.motion.x;
        int mouseY = e.motion.y;
        int itemHeight = height;
        int totalHeight = itemHeight * static_cast<int>(options.size());
        
        if (mouseX >= x && mouseX < x + width &&
            mouseY >= y && mouseY < y + totalHeight) {
            int newHoveredIndex = (mouseY - y) / itemHeight;
            hoveredIndex = isValidIndex(newHoveredIndex) ? newHoveredIndex : -1;
        } else {
            hoveredIndex = -1;
        }
    } else if (e.type == SDL_KEYDOWN && hasFocus) {
        if (!expanded) {
            switch (e.key.keysym.sym) {
                case SDLK_RETURN:
                case SDLK_SPACE:
                    expand();
                    break;
                case SDLK_UP:
                    if (currentIndex > 0) {
                        setSelectedIndex(currentIndex - 1);
                    }
                    break;
                case SDLK_DOWN:
                    if (currentIndex < static_cast<int>(options.size()) - 1) {
                        setSelectedIndex(currentIndex + 1);
                    }
                    break;
            }
        } else {
            switch (e.key.keysym.sym) {
                case SDLK_ESCAPE:
                    collapse();
                    break;
                case SDLK_RETURN:
                    if (isValidIndex(hoveredIndex)) {
                        setSelectedIndex(hoveredIndex);
                    }
                    collapse();
                    break;
                case SDLK_UP:
                    if (hoveredIndex > 0) {
                        hoveredIndex--;
                    } else if (hoveredIndex == -1 && !options.empty()) {
                        hoveredIndex = static_cast<int>(options.size()) - 1;
                    }
                    break;
                case SDLK_DOWN:
                    if (hoveredIndex < static_cast<int>(options.size()) - 1) {
                        hoveredIndex++;
                    } else if (hoveredIndex == -1 && !options.empty()) {
                        hoveredIndex = 0;
                    }
                    break;
            }
        }
    }
}

SDL_Rect OptionSelect::getFocusRect() const {
    if (expanded) {
        int totalHeight = height * static_cast<int>(options.size());
        return SDL_Rect{ x - 2, y - 2, width + 4, totalHeight + 4 };
    }
    return SDL_Rect{ x - 2, y - 2, width + 4, height + 4 };
}

void OptionSelect::activate() {
    if (expanded) {
        if (isValidIndex(hoveredIndex)) {
            setSelectedIndex(hoveredIndex);
        }
        collapse();
    } else {
        expand();
    }
}

void OptionSelect::onFocusLost() {
    hasFocus = false;
    collapse();
}

void OptionSelect::setSelectedIndex(int index) {
    if (isValidIndex(index) && index != currentIndex) {
        currentIndex = index;
        invalidateStringCache();
        if (onSelect) {
            onSelect(currentIndex);
        }
    }
}

const std::string& OptionSelect::getSelectedOption() const {
    static const std::string empty;
    return isValidIndex(currentIndex) ? options[currentIndex] : empty;
}

void OptionSelect::clearOptions() {
    options.clear();
    currentIndex = -1;
    invalidateStringCache();
    collapse();
}

} // namespace ui
