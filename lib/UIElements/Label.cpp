#include "Label.h"
#include "../../lib/Theme/ThemeBase.h"
#include "../../src/Helpers.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>

namespace ui {

void Label::render(SDL_Renderer* renderer, TTF_Font* font, std::shared_ptr<Theme> theme) {
    if (!renderer || !theme || text.empty()) {
        return;
    }
    
    ThemeableElementColors tc = theme->labelColors();
    
    if (font) {
        SDL_Color sdlColor = { tc.labelText.r, tc.labelText.g, tc.labelText.b, tc.labelText.a };
        SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), sdlColor);
        if (surface) {
            SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
            if (texture) {
                SDL_Rect dst = { x, y, surface->w, surface->h };
                SDL_RenderCopy(renderer, texture, nullptr, &dst);
                SDL_DestroyTexture(texture);
            }
            SDL_FreeSurface(surface);
        }
    }
}

void Label::setText(const std::string &newText) {
    text = newText;
}

void Label::autoSize(TTF_Font* font) {
    if (font && !text.empty()) {
        TTF_SizeText(font, text.c_str(), &width, &height);
    }
}

} // namespace ui
