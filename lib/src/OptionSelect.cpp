#include "uiframework/UIElements/OptionSelect.h"
#include "uiframework/Theme/ThemeBase.h"
#include "uiframework/Helpers.h"
#include "uiframework/UICore.h"
#include "uiframework/Constants.h"
#include "uiframework/Utils/TextUtils.h"
#include "uiframework/Utils/BorderRenderer.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <algorithm>

namespace ui {

std::string OptionSelect::getCachedTruncatedText(const std::string& text, TTF_Font* font, int availableWidth, StringCache& cache) const {
    // Check if cache is valid
    if (cache.valid && cache.originalText == text && cache.availableWidth == availableWidth) {
        return cache.truncatedText;
    }
    
    // Cache miss - use TextUtils for truncation
    cache.originalText = text;
    cache.availableWidth = availableWidth;
    cache.truncatedText = TextUtils::truncateWithEllipsis(text, font, availableWidth);
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

bool OptionSelect::isValidIndex(int index) const {
    return index >= 0 && index < static_cast<int>(options.size());
}

void OptionSelect::renderCollapsed(SDL_Renderer* renderer, TTF_Font* font, std::shared_ptr<Theme> theme) {
    const int padding = ui::Constants::DEFAULT_PADDING;
    const int arrowWidth = ui::Constants::ARROW_WIDTH;
    
    ThemeableElementColors tc = theme->optionSelectColors();
    
    // Determine colors based on enabled state
    Color bgColor = enabled ? tc.selectOptionUnselected : tc.selectOptionDisabled;
    Color textColor = enabled ? tc.selectOptionTextUnselected : tc.selectOptionTextDisabled;
    Color borderColor = enabled ? tc.selectOptionSelected : tc.selectOptionDisabled;
    
    // Draw selection box
    SDL_Rect cellRect = { x, y, width, height };
    drawFilledRect(renderer, cellRect, bgColor);
    
    // Draw border
    ui::BorderRenderer::drawFlatBorder(renderer, cellRect, borderColor);
    
    // Draw focus indicator (only when enabled)
    if (hasFocus && enabled) {
        SDL_Rect focusRect = getFocusRect();
        ui::BorderRenderer::drawFocusBorder(renderer, focusRect, borderColor);
    }
    
    // Render current selection text
    if (font && isValidIndex(currentIndex)) {
        int availableWidth = width - 2 * padding - arrowWidth;
        std::string displayText = getCachedTruncatedText(options[currentIndex], font, availableWidth, displayCache);
        
        if (!displayText.empty()) {
            SDL_Color sdlTextColor = { textColor.r, textColor.g, textColor.b, textColor.a };
            
            std::string cacheKey = "option_" + std::to_string(currentIndex);
            TextCacheEntry* cached = getCachedText(cacheKey, displayText, sdlTextColor, renderer, font);
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
    SDL_SetRenderDrawColor(renderer, borderColor.r, borderColor.g, borderColor.b, borderColor.a);
    // Simple triangle pointing down
    for (int i = 0; i < 5; i++) {
        SDL_RenderDrawLine(renderer, 
            arrowX + i, arrowY - 2 + i,
            arrowX + 8 - i, arrowY - 2 + i);
    }
}

void OptionSelect::renderExpanded(SDL_Renderer* renderer, TTF_Font* font, std::shared_ptr<Theme> theme) {
    const int padding = ui::Constants::DEFAULT_PADDING;
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

void OptionSelect::renderImpl(const RenderContext& ctx) {
    if (options.empty()) {
        return;
    }
    
    // Check if dimensions changed and invalidate cache if needed
    if (width != lastWidth || height != lastHeight) {
        invalidateStringCache();
        lastWidth = width;
        lastHeight = height;
    }
    
    if (expanded) {
        renderExpanded(ctx.renderer, ctx.font, ctx.theme);
    } else {
        renderCollapsed(ctx.renderer, ctx.font, ctx.theme);
    }
}

void OptionSelect::onMouseDown(int x, int y) {
    if (options.empty()) return;
    
    if (!expanded) {
        expand();
    } else {
        // Click on expanded dropdown
        int itemHeight = height;
        int totalHeight = itemHeight * static_cast<int>(options.size());
        
        if (x >= this->x && x < this->x + width &&
            y >= this->y && y < this->y + totalHeight) {
            // Click inside dropdown
            int clickedIndex = (y - this->y) / itemHeight;
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

void OptionSelect::onKeyDown(const SDL_Keycode& key) {
    if (!expanded) {
        switch (key) {
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
        switch (key) {
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

void OptionSelect::activate() {
    if (!expanded) {
        expand();
    } else {
        collapse();
    }
}

SDL_Rect OptionSelect::getFocusRect() const {
    if (expanded) {
        int totalHeight = height * static_cast<int>(options.size());
        return SDL_Rect{ x - 2, y - 2, width + 4, totalHeight + 4 };
    }
    return SDL_Rect{ x - 2, y - 2, width + 4, height + 4 };
}

void OptionSelect::onFocusLost() {
    hasFocus = false;
    collapse();
}

void OptionSelect::setSelectedIndex(int index) {
    if (isValidIndex(index) && index != currentIndex) {
        currentIndex = index;
        invalidateStringCache();
        if (onSelect && coreRef) {
            int selectedIndex = currentIndex;
            coreRef->queueCallback([this, selectedIndex]() {
                if (onSelect) {
                    try {
                        onSelect(selectedIndex);
                    } catch (const std::exception& e) {
                        std::cerr << "Error in option select callback: " << e.what() << std::endl;
                    }
                }
            });
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
