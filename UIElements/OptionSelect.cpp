#include "OptionSelect.h"
#include "../Theme/ThemeGlobals.h"
#include "../Theme/ThemeBase.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>

namespace ui {

void OptionSelect::render(SDL_Renderer* renderer) {
    const int padding = 5;
    initFont();
    // Collapsed state uses dynamic height (like TextBox)
    int collapsedHeight = TTF_FontLineSkip(globalFont) + 2 * padding;
    
    if (!expanded) {
        // Collapsed: Draw one cell using the collapsed height.
        SDL_Rect cellRect = { x, y, width, collapsedHeight };
        ThemeableElementColors tc = g_currentTheme->optionSelectColors();
        drawFilledRect(renderer, cellRect, tc.selectOptionUnselected);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderDrawRect(renderer, &cellRect);
        
        // Compute available width for text, leaving room for the arrow.
        int arrowWidth = 10;
        int availWidth = width - padding - arrowWidth;
        std::string displayText = options[activeIndex];
        
        // Measure text width.
        int textW = 0, textH = 0;
        TTF_SizeText(globalFont, displayText.c_str(), &textW, &textH);
        // If text is too wide, truncate and append ellipsis.
        if (textW > availWidth) {
            std::string ellipsis = "...";
            int ellipsisW = 0;
            TTF_SizeText(globalFont, ellipsis.c_str(), &ellipsisW, &textH);
            // Remove characters until it fits.
            while (!displayText.empty()) {
                std::string candidate = displayText + ellipsis;
                TTF_SizeText(globalFont, candidate.c_str(), &textW, &textH);
                if (textW <= availWidth)
                    break;
                displayText.pop_back();
            }
            displayText += ellipsis;
        }
        
        // Render the active option text (left aligned with padding).
        if (globalFont) {
            SDL_Color textColor = { 0, 0, 0, 255 };
            SDL_Surface* surface = TTF_RenderText_Solid(globalFont, displayText.c_str(), textColor);
            if (surface) {
                SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
                int finalTextW = surface->w, finalTextH = surface->h;
                SDL_FreeSurface(surface);
                SDL_Rect dst = { x + padding, y + (collapsedHeight - finalTextH) / 2, finalTextW, finalTextH };
                SDL_RenderCopy(renderer, texture, nullptr, &dst);
                SDL_DestroyTexture(texture);
            }
        }
        
        // Draw a downward arrow on the right.
        int arrowPadding = 5;
        SDL_Point arrow[3];
        arrow[0] = { x + width - arrowPadding - 10, y + collapsedHeight / 2 - 3 };
        arrow[1] = { x + width - arrowPadding, y + collapsedHeight / 2 - 3 };
        arrow[2] = { x + width - arrowPadding - 5, y + collapsedHeight / 2 + 3 };
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderDrawLines(renderer, arrow, 3);
        SDL_RenderDrawLine(renderer, arrow[2].x, arrow[2].y, arrow[0].x, arrow[0].y);
    } else {
        // Expanded state: For each option, use a fixed cell height (e.g., 30 pixels).
        int cellHeight = 30;
        ThemeableElementColors tc = g_currentTheme->optionSelectColors();
        for (size_t i = 0; i < options.size(); i++) {
            SDL_Rect cellRect = { x, y + static_cast<int>(i) * cellHeight, width, cellHeight };
            if (static_cast<int>(i) == selectedIndex)
                drawFilledRect(renderer, cellRect, tc.selectOptionSelected);
            else
                drawFilledRect(renderer, cellRect, tc.selectOptionUnselected);
            
            // Draw 3D border using theme border colors.
            SDL_SetRenderDrawColor(renderer, tc.selectOptionBorderLight.r, tc.selectOptionBorderLight.g,
                                    tc.selectOptionBorderLight.b, tc.selectOptionBorderLight.a);
            SDL_RenderDrawLine(renderer, cellRect.x, cellRect.y, cellRect.x + cellRect.w, cellRect.y);
            SDL_RenderDrawLine(renderer, cellRect.x, cellRect.y, cellRect.x, cellRect.y + cellRect.h);
            SDL_SetRenderDrawColor(renderer, tc.selectOptionBorderDark.r, tc.selectOptionBorderDark.g,
                                    tc.selectOptionBorderDark.b, tc.selectOptionBorderDark.a);
            SDL_RenderDrawLine(renderer, cellRect.x, cellRect.y + cellRect.h, cellRect.x + cellRect.w, cellRect.y + cellRect.h);
            SDL_RenderDrawLine(renderer, cellRect.x + cellRect.w, cellRect.y, cellRect.x + cellRect.w, cellRect.y + cellRect.h);
            
            // Truncate text if necessary.
            int availableWidth = cellRect.w - 2 * padding;
            std::string optionText = options[i];
            int textW = 0, textH = 0;
            TTF_SizeText(globalFont, optionText.c_str(), &textW, &textH);
            if (textW > availableWidth) {
                std::string ellipsis = "...";
                int ellipsisW = 0;
                TTF_SizeText(globalFont, ellipsis.c_str(), &ellipsisW, &textH);
                while (!optionText.empty()) {
                    std::string candidate = optionText + ellipsis;
                    TTF_SizeText(globalFont, candidate.c_str(), &textW, &textH);
                    if (textW <= availableWidth)
                        break;
                    optionText.pop_back();
                }
                optionText += ellipsis;
            }
            
            // Render the option text centered.
            if (globalFont) {
                SDL_Color textColor = { 0, 0, 0, 255 };
                SDL_Surface* surface = TTF_RenderText_Solid(globalFont, optionText.c_str(), textColor);
                if (surface) {
                    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
                    int finalTextW = surface->w, finalTextH = surface->h;
                    SDL_FreeSurface(surface);
                    SDL_Rect dst = { cellRect.x + (cellRect.w - finalTextW) / 2,
                                     cellRect.y + (cellRect.h - finalTextH) / 2,
                                     finalTextW, finalTextH };
                    SDL_RenderCopy(renderer, texture, nullptr, &dst);
                    SDL_DestroyTexture(texture);
                }
            }
        }
    }
}

void OptionSelect::handleEvent(const SDL_Event &e) {
    if (expanded) {
        if (e.type == SDL_KEYDOWN) {
            if (e.key.keysym.sym == SDLK_UP) {
                selectedIndex = (selectedIndex - 1 + options.size()) % options.size();
            } else if (e.key.keysym.sym == SDLK_DOWN) {
                selectedIndex = (selectedIndex + 1) % options.size();
            } else if (e.key.keysym.sym == SDLK_RETURN) {
                activeIndex = selectedIndex;
                expanded = false;
                if (onSelect) onSelect(activeIndex);
            }
        }
    } else {
        if (hasFocus && e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_RETURN) {
            expanded = true;
            selectedIndex = activeIndex;
        }
    }
}

// Override getFocusRect so the focus outline uses the collapsed height when not expanded.
SDL_Rect OptionSelect::getFocusRect() const {
    if (!expanded) {
        // Collapsed state: use dynamic height as before.
        const int padding = 5;
        initFont();
        int collapsedHeight = TTF_FontLineSkip(globalFont) + 2 * padding;
        return SDL_Rect{ x - 2, y - 2, width + 4, collapsedHeight + 4 };
    } else {
        // Expanded state: use the fixed cell height (30 pixels) for each option.
        int cellHeight = 30;
        return SDL_Rect{ x - 2, y - 2, width + 4, static_cast<int>(options.size()) * cellHeight + 4 };
    }
}

void OptionSelect::activate() {}

} // namespace ui
