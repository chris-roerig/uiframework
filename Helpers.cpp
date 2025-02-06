#include "Helpers.h"
#include "Theme/ThemeGlobals.h"
#include "Theme/ThemeFrameworkDefault.h"  // Add this include
#include <iostream>

namespace ui {
    std::shared_ptr<Theme> g_currentTheme = std::make_shared<ThemeFrameworkDefault>();
    TTF_Font* globalFont = nullptr;

    void initFont() {
        if (!globalFont) {
            if (TTF_Init() == -1) {
                std::cerr << "TTF_Init error: " << TTF_GetError() << std::endl;
                return;
            }
            globalFont = TTF_OpenFont("Assets/default_font.ttf", 14);
            if (!globalFont)
                std::cerr << "TTF_OpenFont error: " << TTF_GetError() << std::endl;
        }
    }
    
    void drawFilledRect(SDL_Renderer* renderer, const SDL_Rect &rect, const Color &color) {
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        SDL_RenderFillRect(renderer, &rect);
    }

    void drawLine(SDL_Renderer* renderer, int x1, int y1, int x2, int y2, const Color &color) {
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
    }

    void renderText(SDL_Renderer* renderer, const std::string &text, int x, int y, const Color &color) {
        initFont();
        if (!globalFont) return;
        SDL_Color sdlColor = { color.r, color.g, color.b, color.a };
        SDL_Surface* surface = TTF_RenderText_Solid(globalFont, text.c_str(), sdlColor);
        if (!surface) return;
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_Rect dst = { x, y, surface->w, surface->h };
        SDL_FreeSurface(surface);
        SDL_RenderCopy(renderer, texture, nullptr, &dst);
        SDL_DestroyTexture(texture);
    }
}
