#include "uiframework/UIElements/TextBox.h"
#include "uiframework/Theme/ThemeBase.h"
#include "uiframework/Helpers.h"
#include "uiframework/UICore.h"
#include "uiframework/Constants.h"
#include "uiframework/ErrorHandling.h"
#include "uiframework/Utils/TextUtils.h"
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

void TextBox::render(SDL_Renderer* renderer, TTF_Font* font, std::shared_ptr<Theme> theme) {
    if (!ErrorHandling::validateRenderParams(renderer, theme)) {
        return;
    }
    
    // Check if dimensions changed and invalidate cache if needed
    if (width != lastWidth || height != lastHeight) {
        invalidateStringCache();
        lastWidth = width;
        lastHeight = height;
    }
    
    const int padding = ui::Constants::DEFAULT_PADDING;
    int boxHeight = height;
    if (font) {
        boxHeight = std::max(height, TTF_FontLineSkip(font) + 2 * padding);
    }
    
    SDL_Rect rect = { x, y, width, boxHeight };
    ThemeableElementColors tc = theme->textInputColors();
    
    // Determine colors based on enabled state
    Color bgColor = enabled ? tc.textInputBackground : tc.textInputDisabled;
    Color textColor = enabled ? tc.textInputText : tc.textInputTextDisabled;
    
    // Draw background
    drawFilledRect(renderer, rect, bgColor);
    
    // Draw 3D border
    SDL_SetRenderDrawColor(renderer, tc.textInputBorderLight.r, tc.textInputBorderLight.g,
                           tc.textInputBorderLight.b, tc.textInputBorderLight.a);
    SDL_RenderDrawLine(renderer, x, y, x + width, y);
    SDL_RenderDrawLine(renderer, x, y, x, y + boxHeight);
    
    SDL_SetRenderDrawColor(renderer, tc.textInputBorderDark.r, tc.textInputBorderDark.g,
                           tc.textInputBorderDark.b, tc.textInputBorderDark.a);
    SDL_RenderDrawLine(renderer, x, y + boxHeight, x + width, y + boxHeight);
    SDL_RenderDrawLine(renderer, x + width, y, x + width, y + boxHeight);
    
    // Draw focus indicator (only when enabled)
    if (hasFocus && enabled) {
        SDL_Rect focusRect = getFocusRect();
        SDL_SetRenderDrawColor(renderer, textColor.r, textColor.g, textColor.b, textColor.a);
        SDL_RenderDrawRect(renderer, &focusRect);
    }
    
    // Render text content
    if (font && !content.empty()) {
        int availableWidth = width - 2 * padding;
        if (availableWidth <= 0) return;
        
        std::string displayText = getCachedTruncatedText(content, font, availableWidth);
        
        if (!displayText.empty()) {
            SDL_Color sdlTextColor = { textColor.r, textColor.g, textColor.b, textColor.a };
            SDL_Surface* surface = nullptr;
            
            if (textSelected && enabled) {
                // Render selected text with highlighted background (only when enabled)
                SDL_Color bgColor = { textColor.r, textColor.g, textColor.b, textColor.a };
                SDL_Color selTextColor = { bgColor.r, bgColor.g, bgColor.b, bgColor.a };
                surface = TTF_RenderText_Shaded(font, displayText.c_str(), selTextColor, bgColor);
            } else {
                surface = TTF_RenderText_Solid(font, displayText.c_str(), sdlTextColor);
            }
            
            if (surface) {
                SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
                if (texture) {
                    SDL_Rect dst = { x + padding, y + padding, surface->w, surface->h };
                    SDL_RenderCopy(renderer, texture, nullptr, &dst);
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
                if (TTF_SizeText(font, beforeCursor.c_str(), &beforeWidth, nullptr) == 0) {
                    cursorX += beforeWidth;
                }
            }
            SDL_Color textColor = { tc.textInputText.r, tc.textInputText.g, tc.textInputText.b, tc.textInputText.a };
            SDL_SetRenderDrawColor(renderer, textColor.r, textColor.g, textColor.b, textColor.a);
            SDL_RenderDrawLine(renderer, cursorX, y + padding, cursorX, y + boxHeight - padding);
        }
    }
}

void TextBox::handleEvent(const SDL_Event &e) {
    if (!visible || !enabled) return;
    
    if (e.type == SDL_MOUSEBUTTONDOWN) {
        if (e.button.button == SDL_BUTTON_LEFT) {
            int mouseX = e.button.x;
            int mouseY = e.button.y;
            if (containsPoint(mouseX, mouseY)) {
                if (coreRef) {
                    coreRef->setFocus(elementId);
                }
            }
        }
    } else if (e.type == SDL_TEXTINPUT && hasFocus) {
        // Insert text at cursor position or replace selected text
        std::string inputText = e.text.text;
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
    } else if (e.type == SDL_KEYDOWN && hasFocus) {
        switch (e.key.keysym.sym) {
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
                if (e.key.keysym.mod & KMOD_CTRL) {
                    selectAll();
                }
                break;
            case SDLK_RETURN:
            case SDLK_KP_ENTER:
                activate();
                break;
        }
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
