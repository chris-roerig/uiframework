#include "uiframework/UIElements/CheckBox.h"
#include "uiframework/Theme/ThemeBase.h"
#include "uiframework/Helpers.h"
#include "uiframework/UICore.h"
#include "uiframework/Constants.h"
#include "uiframework/ErrorHandling.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>

namespace ui {

void CheckBox::render(SDL_Renderer* renderer, TTF_Font* font, std::shared_ptr<Theme> theme) {
    if (!ErrorHandling::validateRenderParams(renderer, theme)) {
        return;
    }
    
    SDL_Rect rect = { x, y, width, height };
    ThemeableElementColors tc = theme->checkboxColors();
    
    // Determine colors based on enabled state
    Color bgColor = enabled ? tc.checkboxEnabled : tc.checkboxDisabled;
    Color checkColor = enabled ? tc.checkboxChecked : tc.checkboxDisabled;
    
    // Draw checkbox background
    drawFilledRect(renderer, rect, bgColor);
    
    // Draw 3D border for the checkbox
    SDL_SetRenderDrawColor(renderer, tc.checkboxBorderLight.r, tc.checkboxBorderLight.g, tc.checkboxBorderLight.b, tc.checkboxBorderLight.a);
    SDL_RenderDrawLine(renderer, rect.x, rect.y, rect.x + rect.w, rect.y);         // Top edge
    SDL_RenderDrawLine(renderer, rect.x, rect.y, rect.x, rect.y + rect.h);         // Left edge
    
    SDL_SetRenderDrawColor(renderer, tc.checkboxBorderDark.r, tc.checkboxBorderDark.g, tc.checkboxBorderDark.b, tc.checkboxBorderDark.a);
    SDL_RenderDrawLine(renderer, rect.x, rect.y + rect.h, rect.x + rect.w, rect.y + rect.h); // Bottom edge
    SDL_RenderDrawLine(renderer, rect.x + rect.w, rect.y, rect.x + rect.w, rect.y + rect.h); // Right edge
    
    // Draw focus indicator (only when enabled)
    if (hasFocus && enabled) {
        SDL_Rect focusRect = getFocusRect();
        SDL_SetRenderDrawColor(renderer, checkColor.r, checkColor.g, checkColor.b, checkColor.a);
        SDL_RenderDrawRect(renderer, &focusRect);
    }
    
    // If checked, draw a filled square
    if (checked) {
        SDL_Rect checkRect = {
            x + ui::Constants::BORDER_OFFSET + 2,
            y + ui::Constants::BORDER_OFFSET + 2,
            width - 2 * (ui::Constants::BORDER_OFFSET + 2),
            height - 2 * (ui::Constants::BORDER_OFFSET + 2)
        };
        drawFilledRect(renderer, checkRect, checkColor);
    }
}

void CheckBox::handleEvent(const SDL_Event &e) {
    if (!visible || !enabled) {
        return;
    }
    
    if (e.type == SDL_MOUSEBUTTONDOWN) {
        if (e.button.button == SDL_BUTTON_LEFT) {
            int mouseX = e.button.x;
            int mouseY = e.button.y;
            if (containsPoint(mouseX, mouseY)) {
                activate();
            }
        }
    } else if (e.type == SDL_KEYDOWN && hasFocus) {
        if (e.key.keysym.sym == SDLK_SPACE || e.key.keysym.sym == SDLK_RETURN) {
            activate();
        }
    }
}

void CheckBox::activate() {
    setChecked(!checked);
}

void CheckBox::setChecked(bool state) {
    if (checked != state) {
        checked = state;
        if (onToggle && coreRef) {
            bool currentState = checked;
            coreRef->queueCallback([this, currentState]() {
                if (onToggle) {
                    try {
                        onToggle(currentState);
                    } catch (const std::exception& e) {
                        std::cerr << "Error in checkbox callback: " << e.what() << std::endl;
                    }
                }
            });
        }
    }
}

std::pair<int, int> CheckBox::getPreferredSize(TTF_Font* font) const {
    return {20, 20}; // Fixed checkbox size
}

std::pair<int, int> CheckBox::getMinimumSize() const {
    return {20, 20}; // Same as preferred - fixed size
}

} // namespace ui
