#include "ListView.h"
#include "ThemeGlobals.h"
#include "ThemeBase.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>

namespace ui {

ListView::ListView(int x_, int y_, int w_, int h_, int itemHeight_)
    : UIElement(x_, y_, w_, h_), selectedIndex(0), itemHeight(itemHeight_) {}

void ListView::render(SDL_Renderer* renderer) {
    initFont();
    // Draw the ListView background using the theme's label background.
    SDL_Rect rect = { x, y, width, height };
    drawFilledRect(renderer, rect, g_currentTheme->labelColors().labelBackground);

    // Determine how many items fit within the view.
    int maxItems = height / itemHeight;
    for (int i = 0; i < maxItems && i < (int)items.size(); i++) {
        SDL_Rect itemRect = { x, y + i * itemHeight, width, itemHeight };
        // Highlight the selected item.
        if (i == selectedIndex)
            drawFilledRect(renderer, itemRect, g_currentTheme->optionSelectColors().selectOptionSelected);
        else
            drawFilledRect(renderer, itemRect, g_currentTheme->optionSelectColors().selectOptionUnselected);
        
        // Render the text with a left padding.
        if (globalFont) {
            SDL_Color textColor = {0, 0, 0, 255};
            SDL_Surface* surface = TTF_RenderText_Solid(globalFont, items[i].c_str(), textColor);
            if (surface) {
                SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
                int textW = surface->w, textH = surface->h;
                SDL_FreeSurface(surface);
                int pad = 5;
                SDL_Rect dst = { x + pad, y + i * itemHeight + (itemHeight - textH) / 2, textW, textH };
                SDL_RenderCopy(renderer, texture, nullptr, &dst);
                SDL_DestroyTexture(texture);
            }
        }
    }
}

void ListView::handleEvent(const SDL_Event &e) {
    if (e.type == SDL_KEYDOWN) {
        if (e.key.keysym.sym == SDLK_UP) {
            selectedIndex = (selectedIndex - 1 + items.size()) % items.size();
        } else if (e.key.keysym.sym == SDLK_DOWN) {
            selectedIndex = (selectedIndex + 1) % items.size();
        } else if (e.key.keysym.sym == SDLK_RETURN) {
            if (onSelect)
                onSelect(selectedIndex);
        }
    }
}

SDL_Rect ListView::getFocusRect() const {
    return SDL_Rect{ x, y, width, height };
}

} // namespace ui
