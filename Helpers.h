#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include "Theme/Color.h"

namespace ui {
    extern TTF_Font* globalFont;
    void initFont();
    void drawFilledRect(SDL_Renderer* renderer, const SDL_Rect &rect, const Color &color);
    void drawLine(SDL_Renderer* renderer, int x1, int y1, int x2, int y2, const Color &color);
    void renderText(SDL_Renderer* renderer, const std::string &text, int x, int y, const Color &color);
}
