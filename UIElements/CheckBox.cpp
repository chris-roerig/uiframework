#include "CheckBox.h"
#include "../Theme/ThemeGlobals.h"
#include "../Theme/ThemeBase.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>

namespace ui {

void CheckBox::render(SDL_Renderer* renderer) {
    SDL_Rect rect = { x, y, width, height };
    ThemeableElementColors tc = g_currentTheme->checkboxColors();
    drawFilledRect(renderer, rect, tc.checkboxEnabled);
    
    // Draw 3D border for the checkbox.
    SDL_SetRenderDrawColor(renderer, tc.checkboxBorderLight.r, tc.checkboxBorderLight.g, tc.checkboxBorderLight.b, tc.checkboxBorderLight.a);
    SDL_RenderDrawLine(renderer, rect.x, rect.y, rect.x + rect.w, rect.y);         // Top edge.
    SDL_RenderDrawLine(renderer, rect.x, rect.y, rect.x, rect.y + rect.h);         // Left edge.
    SDL_SetRenderDrawColor(renderer, tc.checkboxBorderDark.r, tc.checkboxBorderDark.g, tc.checkboxBorderDark.b, tc.checkboxBorderDark.a);
    SDL_RenderDrawLine(renderer, rect.x, rect.y + rect.h, rect.x + rect.w, rect.y + rect.h); // Bottom edge.
    SDL_RenderDrawLine(renderer, rect.x + rect.w, rect.y, rect.x + rect.w, rect.y + rect.h); // Right edge.
    
    // If checked, draw a check mark using the checkbox checked color.
    if (checked) {
        drawLine(renderer, x, y, x + width, y + height, tc.checkboxChecked);
        drawLine(renderer, x + width, y, x, y + height, tc.checkboxChecked);
    }
}

void CheckBox::handleEvent(const SDL_Event &e) {
    if (hasFocus && e.type == SDL_KEYDOWN) {
        if (e.key.keysym.sym == SDLK_SPACE) {
            checked = !checked;
            if (onToggle) onToggle(checked);
        }
    }
}

void CheckBox::activate() {
  // implement activate action here. typically used when triggered by hotkey
}

} // namespace ui
