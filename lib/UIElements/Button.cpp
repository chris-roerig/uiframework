#include "Button.h"
#include "../../lib/Theme/ThemeBase.h"
#include "../../src/Helpers.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>

namespace ui {

void Button::render(SDL_Renderer* renderer, TTF_Font* font, std::shared_ptr<Theme> theme) {
    if (!renderer || !theme) {
        return;
    }
    
    // Retrieve button colors from the theme
    ThemeableElementColors tc = theme->buttonColors();
    SDL_Rect rect = { x, y, width, height };

    // Determine background color; darken if pressed
    Color bg = tc.buttonBackground;
    if (pressed) {
        bg = Color(static_cast<uint8_t>(bg.r * 0.8),
                   static_cast<uint8_t>(bg.g * 0.8),
                   static_cast<uint8_t>(bg.b * 0.8),
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
        SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), sdlColor);
        if (surface) {
            SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
            if (texture) {
                SDL_Rect dst;
                dst.w = surface->w;
                dst.h = surface->h;
                dst.x = x + (width - dst.w) / 2;
                dst.y = y + (height - dst.h) / 2;
                
                SDL_RenderCopy(renderer, texture, nullptr, &dst);
                SDL_DestroyTexture(texture);
            }
            SDL_FreeSurface(surface);
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
    if (onClick) {
        try {
            onClick();
        } catch (const std::exception& e) {
            std::cerr << "Error in button callback: " << e.what() << std::endl;
        }
    }
}

} // namespace ui
