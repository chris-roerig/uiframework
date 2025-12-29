#include "uiframework/UIElements/Button.h"
#include "uiframework/Theme/ThemeBase.h"
#include "uiframework/Helpers.h"
#include "uiframework/UICore.h"
#include "uiframework/ErrorHandling.h"
#include "uiframework/Constants.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

namespace ui {

void Button::render(SDL_Renderer* renderer, TTF_Font* font, std::shared_ptr<Theme> theme) {
    if (!ErrorHandling::validateRenderParams(renderer, theme)) {
        return;
    }
    
    // Retrieve button colors from the theme
    ThemeableElementColors tc = theme->buttonColors();
    SDL_Rect rect = { x, y, width, height };

    // Determine background color; darken if pressed
    Color bg = tc.buttonBackground;
    if (pressed) {
        bg = Color(static_cast<uint8_t>(bg.r * Constants::BUTTON_PRESSED_DARKEN_FACTOR),
                   static_cast<uint8_t>(bg.g * Constants::BUTTON_PRESSED_DARKEN_FACTOR),
                   static_cast<uint8_t>(bg.b * Constants::BUTTON_PRESSED_DARKEN_FACTOR),
                   bg.a);
    }
    
    // Draw button background
    drawFilledRect(renderer, rect, bg);
    
    // Draw button border if focused
    if (hasFocus) {
        SDL_Rect focusRect = getFocusRect();
        SDL_SetRenderDrawColor(renderer, tc.buttonText.r, tc.buttonText.g, tc.buttonText.b, tc.buttonText.a);
        SDL_RenderDrawRect(renderer, &focusRect);
    }
    
    // Draw the button text
    if (font && !text.empty()) {
        SDL_Color sdlColor = { tc.buttonText.r, tc.buttonText.g, tc.buttonText.b, tc.buttonText.a };
        TextCacheEntry* cached = getCachedText("main", text, sdlColor, renderer, font);
        if (cached && cached->texture) {
            SDL_Rect dst;
            dst.w = cached->width;
            dst.h = cached->height;
            dst.x = x + (width - dst.w) / 2;
            dst.y = y + (height - dst.h) / 2;
            
            SDL_RenderCopy(renderer, cached->texture, nullptr, &dst);
        }
    }
}

void Button::handleEvent(const SDL_Event &e) {
    if (!visible) return;
    
    if (e.type == SDL_MOUSEBUTTONDOWN) {
        if (e.button.button == SDL_BUTTON_LEFT) {
            int mouseX = e.button.x;
            int mouseY = e.button.y;
            if (containsPoint(mouseX, mouseY)) {
                pressed = true;
            }
        }
    } else if (e.type == SDL_MOUSEBUTTONUP) {
        if (e.button.button == SDL_BUTTON_LEFT && pressed) {
            int mouseX = e.button.x;
            int mouseY = e.button.y;
            pressed = false;
            if (containsPoint(mouseX, mouseY)) {
                activate();
            }
        }
    } else if (e.type == SDL_KEYDOWN && hasFocus) {
        if (e.key.keysym.sym == SDLK_RETURN || e.key.keysym.sym == SDLK_SPACE) {
            activate();
        }
    }
}

void Button::activate() {
    if (onClick && coreRef) {
        // Queue the button callback to avoid deadlock
        coreRef->queueCallback(onClick);
    }
}

} // namespace ui
