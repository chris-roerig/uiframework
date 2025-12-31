#include "uiframework/UIElements/Label.h"
#include "uiframework/Theme/ThemeBase.h"
#include "uiframework/Helpers.h"
#include "uiframework/ErrorHandling.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>

namespace ui {

void Label::renderImpl(const RenderContext& ctx) {
    ThemeableElementColors tc = ctx.labelColors();
    SDL_Color sdlColor = { tc.labelText.r, tc.labelText.g, tc.labelText.b, tc.labelText.a };
    SDL_Rect contentRect = getContentRect();
    
    if (ctx.font) {
        TextCacheEntry* cached = getCachedText("main", text, sdlColor, ctx.renderer, ctx.font);
        if (cached && cached->texture) {
            SDL_Rect dst = { contentRect.x, contentRect.y, cached->width, cached->height };
            SDL_RenderCopy(ctx.renderer, cached->texture, nullptr, &dst);
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

std::pair<int, int> Label::getPreferredSize(TTF_Font* font) const {
    if (!font || text.empty()) {
        return {width, height};
    }
    
    int textW = 0, textH = 0;
    TTF_SizeText(font, text.c_str(), &textW, &textH);
    return {textW, textH};
}

std::pair<int, int> Label::getMinimumSize() const {
    return {0, 0}; // Labels can be any size
}

} // namespace ui
