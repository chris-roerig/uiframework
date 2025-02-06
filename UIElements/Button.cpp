#include "Button.h"
#include "../Theme/ThemeGlobals.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>

namespace ui {

void Button::render(SDL_Renderer* renderer) {
    // Retrieve button colors from the current theme.
    ThemeableElementColors tc = g_currentTheme->buttonColors();
    SDL_Rect rect = { x, y, width, height };

    // Determine background color; darken if pressed.
    Color bg = tc.buttonBackground;
    if (pressed) {
        bg = Color(static_cast<uint8_t>(bg.r * 0.8),
                   static_cast<uint8_t>(bg.g * 0.8),
                   static_cast<uint8_t>(bg.b * 0.8),
                   bg.a);
    }
    drawFilledRect(renderer, rect, bg);
    
    // Draw the button text using the theme's button text color.
    initFont();
    if (!globalFont) return;
    SDL_Color sdlColor = { tc.buttonText.r, tc.buttonText.g, tc.buttonText.b, tc.buttonText.a };
    SDL_Surface* surface = TTF_RenderText_Solid(globalFont, text.c_str(), sdlColor);
    if (surface) {
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_Rect dst;
        dst.w = surface->w;
        dst.h = surface->h;
        dst.x = x + (width - dst.w) / 2;
        dst.y = y + (height - dst.h) / 2;
        SDL_FreeSurface(surface);
        SDL_RenderCopy(renderer, texture, nullptr, &dst);
        SDL_DestroyTexture(texture);
    }
    
    // Draw 3D border using theme's button border colors.
    // These should be provided by your theme (defaultButtonBorderLight and defaultButtonBorderDark).
    SDL_SetRenderDrawColor(renderer, tc.buttonBorderLight.r, tc.buttonBorderLight.g,
                            tc.buttonBorderLight.b, tc.buttonBorderLight.a);
    SDL_RenderDrawLine(renderer, x, y, x + width, y);           // Top edge
    SDL_RenderDrawLine(renderer, x, y, x, y + height);          // Left edge

    SDL_SetRenderDrawColor(renderer, tc.buttonBorderDark.r, tc.buttonBorderDark.g,
                            tc.buttonBorderDark.b, tc.buttonBorderDark.a);
    SDL_RenderDrawLine(renderer, x, y + height, x + width, y + height); // Bottom edge
    SDL_RenderDrawLine(renderer, x + width, y, x + width, y + height);  // Right edge
}

void Button::handleEvent(const SDL_Event &e) {
    if (hasFocus) {
        // On key down, if Enter or Space is pressed, mark as pressed.
        if (e.type == SDL_KEYDOWN && (e.key.keysym.sym == SDLK_RETURN || e.key.keysym.sym == SDLK_SPACE)) {
            pressed = true;
        }
        // On key up, if Enter or Space is released, unmark pressed and invoke callback.
        if (e.type == SDL_KEYUP && (e.key.keysym.sym == SDLK_RETURN || e.key.keysym.sym == SDLK_SPACE)) {
            pressed = false;
            if (onClick) onClick();
        }
    }
}

void Button::activate() {
    if (onClick)
        onClick();
}

} // namespace ui
