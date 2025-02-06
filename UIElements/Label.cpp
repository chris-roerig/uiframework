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

} // namespace ui