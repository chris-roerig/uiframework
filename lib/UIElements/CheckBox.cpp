#include "CheckBox.h"
#include "Theme/ThemeBase.h"
#include "Helpers.h"
#include "UICore.h"
#include "../Constants.h"
#include "../ErrorHandling.h"
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
    
    // Draw checkbox background
    drawFilledRect(renderer, rect, tc.checkboxEnabled);
    
    // Draw 3D border for the checkbox
    SDL_SetRenderDrawColor(renderer, tc.checkboxBorderLight.r, tc.checkboxBorderLight.g, tc.checkboxBorderLight.b, tc.checkboxBorderLight.a);
    SDL_RenderDrawLine(renderer, rect.x, rect.y, rect.x + rect.w, rect.y);         // Top edge
    SDL_RenderDrawLine(renderer, rect.x, rect.y, rect.x, rect.y + rect.h);         // Left edge
    
    SDL_SetRenderDrawColor(renderer, tc.checkboxBorderDark.r, tc.checkboxBorderDark.g, tc.checkboxBorderDark.b, tc.checkboxBorderDark.a);
    SDL_RenderDrawLine(renderer, rect.x, rect.y + rect.h, rect.x + rect.w, rect.y + rect.h); // Bottom edge
    SDL_RenderDrawLine(renderer, rect.x + rect.w, rect.y, rect.x + rect.w, rect.y + rect.h); // Right edge
    
    // Draw focus indicator
    if (hasFocus) {
        SDL_Rect focusRect = getFocusRect();
        SDL_SetRenderDrawColor(renderer, tc.checkboxChecked.r, tc.checkboxChecked.g, tc.checkboxChecked.b, tc.checkboxChecked.a);
        SDL_RenderDrawRect(renderer, &focusRect);
    }
    
    // If checked, draw a check mark
    if (checked) {
        drawLine(renderer, x + Constants::BORDER_OFFSET, y + Constants::BORDER_OFFSET, 
                x + width - Constants::BORDER_OFFSET, y + height - Constants::BORDER_OFFSET, tc.checkboxChecked);
        drawLine(renderer, x + width - Constants::BORDER_OFFSET, y + Constants::BORDER_OFFSET, 
                x + Constants::BORDER_OFFSET, y + height - Constants::BORDER_OFFSET, tc.checkboxChecked);
    }
}

void CheckBox::handleEvent(const SDL_Event &e) {
    if (!visible) {
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

} // namespace ui
