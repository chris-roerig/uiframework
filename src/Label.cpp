#include "uiframework/UIElements/Label.h"
#include "uiframework/Theme/ThemeBase.h"
#include "uiframework/Helpers.h"
#include "uiframework/ErrorHandling.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>

namespace ui {

void Label::render(SDL_Renderer* renderer, TTF_Font* font, std::shared_ptr<Theme> theme) {
    if (!ErrorHandling::validateTextRenderParams(renderer, theme, font, text)) {
        return;
    }
    
    ThemeableElementColors tc = theme->labelColors();
    SDL_Color sdlColor = { tc.labelText.r, tc.labelText.g, tc.labelText.b, tc.labelText.a };
    
    if (font) {
        TextCacheEntry* cached = getCachedText("main", text, sdlColor, renderer, font);
        if (cached && cached->texture) {
            SDL_Rect dst = { x, y, cached->width, cached->height };
            SDL_RenderCopy(renderer, cached->texture, nullptr, &dst);
        }
    }
}

void Label::setText(const std::string &newText) {
    if (text != newText) {
        text = newText;
        invalidateTextCache();
    }
}

void Label::autoSize(TTF_Font* font) {
    if (font && !text.empty()) {
        TTF_SizeText(font, text.c_str(), &width, &height);
    }
}

} // namespace ui
