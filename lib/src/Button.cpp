#include "uiframework/UIElements/Button.h"
#include "uiframework/Theme/ThemeBase.h"
#include "uiframework/Helpers.h"
#include "uiframework/UICore.h"
#include "uiframework/ErrorHandling.h"
#include "uiframework/Constants.h"
#include "uiframework/Utils/TextUtils.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

namespace ui {

void Button::renderImpl(const RenderContext& ctx) {
    // Retrieve button colors from the theme
    auto colors = ctx.buttonColors();
    SDL_Rect rect = { x, y, width, height };

    // Determine background and text colors based on enabled state
    Color bg, textColor;
    if (!enabled) {
        bg = colors.buttonDisabled;
        textColor = colors.buttonTextDisabled;
    } else {
        bg = colors.buttonBackground;
        textColor = colors.buttonText;
        // Darken if pressed
        if (getIsPressed()) {
            bg = Color(static_cast<uint8_t>(bg.r * Constants::BUTTON_PRESSED_DARKEN_FACTOR),
                       static_cast<uint8_t>(bg.g * Constants::BUTTON_PRESSED_DARKEN_FACTOR),
                       static_cast<uint8_t>(bg.b * Constants::BUTTON_PRESSED_DARKEN_FACTOR),
                       bg.a);
        }
    }
    
    // Draw button background
    drawFilledRect(ctx.renderer, rect, bg);
    
    // Draw button border if focused (only when enabled)
    if (hasFocus && enabled) {
        SDL_Rect focusRect = getFocusRect();
        SDL_SetRenderDrawColor(ctx.renderer, textColor.r, textColor.g, textColor.b, textColor.a);
        SDL_RenderDrawRect(ctx.renderer, &focusRect);
    }
    
    // Draw the button text
    if (ctx.font && !text.empty()) {
        SDL_Color sdlColor = { textColor.r, textColor.g, textColor.b, textColor.a };
        TextCacheEntry* cached = getCachedText("main", text, sdlColor, ctx.renderer, ctx.font);
        if (cached && cached->texture) {
            SDL_Rect dst;
            dst.w = cached->width;
            dst.h = cached->height;
            dst.x = x + (width - dst.w) / 2;
            dst.y = y + (height - dst.h) / 2;
            
            SDL_RenderCopy(ctx.renderer, cached->texture, nullptr, &dst);
        }
    }
}

void Button::activate() {
    if (onClick && coreRef) {
        // Queue the button callback to avoid deadlock
        coreRef->queueCallback(onClick);
    }
}

std::pair<int, int> Button::getPreferredSize(TTF_Font* font) const {
    if (!font || text.empty()) {
        return {width, height};
    }
    
    auto [textW, textH] = TextUtils::getTextSize(text, font);
    return {textW + 20, textH + 10}; // Add padding
}

std::pair<int, int> Button::getMinimumSize() const {
    return {40, 20}; // Minimum clickable size
}

void Button::autoSize(TTF_Font* font) {
    auto [prefW, prefH] = getPreferredSize(font);
    width = prefW;
    height = prefH;
}

} // namespace ui
