#include "Label.h"
#include "../Theme/ThemeGlobals.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>

namespace ui {

void Label::render(SDL_Renderer* renderer) {
    ThemeableElementColors tc = g_currentTheme->labelColors();
    renderText(renderer, text, x, y, tc.labelText);
}

void Label::setText(const std::string &newText) {
    text = newText;
    initFont();
    if (globalFont) {
        TTF_SizeText(globalFont, text.c_str(), &width, &height);
    }
}

} // namespace ui