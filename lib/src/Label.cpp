#include "uiframework/UIElements/Label.h"
#include "uiframework/Theme/ThemeBase.h"
#include "uiframework/Helpers.h"
#include "uiframework/ErrorHandling.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>

namespace ui {

void Label::renderImpl(const RenderContext& ctx) {
    // Process real-time updates
    int textBufferIndex = currentTextBuffer.load();
    if (!textBuffers[textBufferIndex].empty()) {
        text = textBuffers[textBufferIndex];
        invalidateTextCache();
    }
    
    ThemeableElementColors tc = ctx.labelColors();
    SDL_Color sdlColor = { tc.labelText.r, tc.labelText.g, tc.labelText.b, tc.labelText.a };
    SDL_Rect contentRect = getContentRect();
    
    // Phase 2: Use element's effective font instead of context font
    TTF_Font* effectiveFont = getEffectiveFont();
    if (!effectiveFont) {
        effectiveFont = ctx.font; // Fallback to context font
    }
    
    if (effectiveFont) {
        TextCacheEntry* cached = getCachedText("main", text, sdlColor, ctx.renderer, effectiveFont);
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
    // Phase 2: Use element's effective font for auto-sizing
    TTF_Font* effectiveFont = getEffectiveFont();
    if (!effectiveFont) {
        effectiveFont = font; // Fallback to provided font
    }
    
    if (effectiveFont && !text.empty()) {
        TTF_SizeText(effectiveFont, text.c_str(), &width, &height);
        // Add padding to the calculated size
        width += padding.left + padding.right;
        height += padding.top + padding.bottom;
    }
}

std::pair<int, int> Label::getPreferredSize(TTF_Font* font) const {
    // Phase 2: Use element's effective font for size calculation
    TTF_Font* effectiveFont = getEffectiveFont();
    if (!effectiveFont) {
        effectiveFont = font; // Fallback to provided font
    }
    
    if (!effectiveFont || text.empty()) {
        return {width, height};
    }
    
    int textW = 0, textH = 0;
    TTF_SizeText(effectiveFont, text.c_str(), &textW, &textH);
    // Add padding to the calculated size
    return {textW + padding.left + padding.right, textH + padding.top + padding.bottom};
}

std::pair<int, int> Label::getMinimumSize() const {
    return {0, 0}; // Labels can be any size
}

// Real-time safe methods (lock-free, audio thread safe)
void Label::realtimeSetText(const std::string& newText) {
    int nextBuffer = 1 - currentTextBuffer.load();
    textBuffers[nextBuffer] = newText;
    currentTextBuffer.store(nextBuffer);
}

} // namespace ui
