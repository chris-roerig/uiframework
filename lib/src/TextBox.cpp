#include "uiframework/UIElements/TextBox.h"
#include "uiframework/Theme/ThemeBase.h"
#include "uiframework/Helpers.h"
#include "uiframework/UICore.h"
#include "uiframework/Constants.h"
#include "uiframework/ErrorHandling.h"
#include "uiframework/Utils/TextUtils.h"
#include "uiframework/Utils/BorderRenderer.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <algorithm>

namespace ui {

std::string TextBox::getCachedTruncatedText(const std::string& text, TTF_Font* font, int availableWidth) const {
    // Check if cache is valid
    if (displayCache.valid && displayCache.originalText == text && displayCache.availableWidth == availableWidth) {
        return displayCache.truncatedText;
    }
    
    // Cache miss - use TextUtils for truncation
    displayCache.originalText = text;
    displayCache.availableWidth = availableWidth;
    displayCache.truncatedText = TextUtils::truncateWithEllipsis(text, font, availableWidth);
    displayCache.valid = true;
    
    return displayCache.truncatedText;
}

void TextBox::invalidateStringCache() {
    displayCache.valid = false;
}

void TextBox::renderImpl(const RenderContext& ctx) {
    // Check if dimensions changed and invalidate cache if needed
    if (width != lastWidth || height != lastHeight) {
        invalidateStringCache();
        lastWidth = width;
        lastHeight = height;
    }
    
    const int padding = ui::Constants::DEFAULT_PADDING;
    int boxHeight = height;
    if (ctx.font) {
        boxHeight = std::max(height, TTF_FontLineSkip(ctx.font) + 2 * padding);
    }
    
    SDL_Rect rect = { x, y, width, boxHeight };
    ThemeableElementColors tc = ctx.textInputColors();
    
    // Determine colors based on enabled state
    Color bgColor = enabled ? tc.textInputBackground : tc.textInputDisabled;
    Color textColor = enabled ? tc.textInputText : tc.textInputTextDisabled;
    
    // Draw background
    drawFilledRect(ctx.renderer, rect, bgColor);
    
    // Draw 3D border
    ui::BorderRenderer::draw3DBorder(ctx.renderer, rect, tc.textInputBorderLight, tc.textInputBorderDark);
    
    // Draw focus indicator (only when enabled)
    if (hasFocus && enabled) {
        SDL_Rect focusRect = getFocusRect();
        SDL_SetRenderDrawColor(ctx.renderer, textColor.r, textColor.g, textColor.b, textColor.a);
        SDL_RenderDrawRect(ctx.renderer, &focusRect);
    }
    
    // Render text content
    if (ctx.font && !content.empty()) {
        int availableWidth = width - 2 * padding;
        if (availableWidth <= 0) return;
        
        std::string displayText = getCachedTruncatedText(content, ctx.font, availableWidth);
        
        if (!displayText.empty()) {
            SDL_Color sdlTextColor = { textColor.r, textColor.g, textColor.b, textColor.a };
            SDL_Surface* surface = nullptr;
            
            if (textSelected && enabled) {
                // Render selected text with highlighted background (only when enabled)
                SDL_Color bgColor = { textColor.r, textColor.g, textColor.b, textColor.a };
                SDL_Color selTextColor = { bgColor.r, bgColor.g, bgColor.b, bgColor.a };
                surface = TTF_RenderText_Shaded(ctx.font, displayText.c_str(), selTextColor, bgColor);
            } else {
                surface = TTF_RenderText_Solid(ctx.font, displayText.c_str(), sdlTextColor);
            }
            
            if (surface) {
                SDL_Texture* texture = SDL_CreateTextureFromSurface(ctx.renderer, surface);
                if (texture) {
                    SDL_Rect contentRect = getContentRect();
                    SDL_Rect dst = { contentRect.x + padding, contentRect.y + padding, surface->w, surface->h };
                    SDL_RenderCopy(ctx.renderer, texture, nullptr, &dst);
                    SDL_DestroyTexture(texture);
                } else {
                    SDL_FreeSurface(surface);
                    return;
                }
                SDL_FreeSurface(surface);
            }
        }
        
        // Draw cursor if focused and not selected
        if (hasFocus && !textSelected) {
            int cursorX = x + padding;
            if (cursorPosition > 0 && cursorPosition <= content.length()) {
                std::string beforeCursor = content.substr(0, cursorPosition);
                int beforeWidth = 0;
                if (TTF_SizeText(ctx.font, beforeCursor.c_str(), &beforeWidth, nullptr) == 0) {
                    cursorX += beforeWidth;
                }
            }
            SDL_Color textColor = { tc.textInputText.r, tc.textInputText.g, tc.textInputText.b, tc.textInputText.a };
            SDL_SetRenderDrawColor(ctx.renderer, textColor.r, textColor.g, textColor.b, textColor.a);
            SDL_Rect contentRect = getContentRect();
            SDL_RenderDrawLine(ctx.renderer, cursorX, contentRect.y + padding, cursorX, contentRect.y + contentRect.h - padding);
        }
    }
}

void TextBox::onMouseDown(int x, int y) {
    // Focus is automatically handled by InteractiveElement
}

void TextBox::onTextInput(const std::string& inputText) {
    if (!inputText.empty()) {
        if (textSelected) {
            // Replace selected text
            content = inputText;
            cursorPosition = inputText.length();
            textSelected = false;
            invalidateStringCache();
        } else {
            // Insert at cursor position
            content.insert(cursorPosition, inputText);
            cursorPosition += inputText.length();
            invalidateStringCache();
        }
    }
}

void TextBox::onKeyDown(const SDL_Keycode& key) {
    switch (key) {
        case SDLK_BACKSPACE:
            if (textSelected) {
                content.clear();
                cursorPosition = 0;
                textSelected = false;
                invalidateStringCache();
            } else if (cursorPosition > 0) {
                content.erase(cursorPosition - 1, 1);
                cursorPosition--;
                invalidateStringCache();
            }
            break;
        case SDLK_DELETE:
            if (textSelected) {
                content.clear();
                cursorPosition = 0;
                textSelected = false;
                invalidateStringCache();
            } else if (cursorPosition < content.length()) {
                content.erase(cursorPosition, 1);
                invalidateStringCache();
            }
            break;
        case SDLK_LEFT:
            if (cursorPosition > 0) {
                cursorPosition--;
            }
            textSelected = false;
            break;
        case SDLK_RIGHT:
            if (cursorPosition < content.length()) {
                cursorPosition++;
            }
            textSelected = false;
            break;
        case SDLK_HOME:
            cursorPosition = 0;
            textSelected = false;
            break;
        case SDLK_END:
            cursorPosition = content.length();
            textSelected = false;
            break;
        case SDLK_a:
            if (SDL_GetModState() & KMOD_CTRL) {
                selectAll();
            }
            break;
        case SDLK_RETURN:
        case SDLK_KP_ENTER:
            activate();
            break;
    }
}

void TextBox::activate() {
    // Default activation clears selection
    textSelected = false;
}

SDL_Rect TextBox::getFocusRect() const {
    return SDL_Rect{ x - 2, y - 2, width + 4, height + 4 };
}

void TextBox::onFocusGained() {
    hasFocus = true;
    SDL_StartTextInput();
    if (autoHighlight) {
        selectAll();
    }
}

void TextBox::onFocusLost() {
    hasFocus = false;
    textSelected = false;
    SDL_StopTextInput();
}

void TextBox::setText(const std::string& text) {
    content = text;
    // Validate cursor position
    cursorPosition = std::min(cursorPosition, content.length());
    textSelected = false;
    invalidateStringCache();
}

void TextBox::selectAll() {
    textSelected = !content.empty();
}

void TextBox::clearSelection() {
    textSelected = false;
}

std::pair<int, int> TextBox::getPreferredSize(TTF_Font* font) const {
    if (!font) {
        return {width, height};
    }
    
    // Base size on content or placeholder
    std::string sampleText = content.empty() ? "Sample Text" : content;
    auto [textW, textH] = TextUtils::getTextSize(sampleText, font);
    return {textW + 20, textH + 8}; // Add padding for cursor and borders
}

std::pair<int, int> TextBox::getMinimumSize() const {
    return {60, 20}; // Minimum for cursor visibility and usability
}

void TextBox::autoSize(TTF_Font* font) {
    auto [prefW, prefH] = getPreferredSize(font);
    width = prefW;
    height = prefH;
}

} // namespace ui
