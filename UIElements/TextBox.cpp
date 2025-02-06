#include "TextBox.h"
#include "../Theme/ThemeGlobals.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>

namespace ui {

// --- TextBox ---
void TextBox::render(SDL_Renderer* renderer) {
    initFont();
    if (!globalFont) return;
    const int padding = 5;
    int boxHeight = TTF_FontLineSkip(globalFont) + 2 * padding;
    SDL_Rect rect = { x, y, width, boxHeight };
    ThemeableElementColors tc = g_currentTheme->textInputColors();
    drawFilledRect(renderer, rect, tc.textInputBackground);
    
    // Draw the border using theme-provided border colors.
    SDL_SetRenderDrawColor(renderer, tc.textInputBorderLight.r, tc.textInputBorderLight.g,
                           tc.textInputBorderLight.b, tc.textInputBorderLight.a);
    SDL_RenderDrawLine(renderer, x, y, x + width, y);
    SDL_RenderDrawLine(renderer, x, y, x, y + boxHeight);
    SDL_SetRenderDrawColor(renderer, tc.textInputBorderDark.r, tc.textInputBorderDark.g,
                           tc.textInputBorderDark.b, tc.textInputBorderDark.a);
    SDL_RenderDrawLine(renderer, x, y + boxHeight, x + width, y + boxHeight);
    SDL_RenderDrawLine(renderer, x + width, y, x + width, y + boxHeight);
    
    // Compute available width for text.
    int availableWidth = width - 2 * padding;
    std::string displayText = content;
    
    int textW = 0, textH = 0;
    TTF_SizeText(globalFont, displayText.c_str(), &textW, &textH);
    if (textW > availableWidth) {
        std::string ellipsis = "...";
        int ellipsisW = 0;
        TTF_SizeText(globalFont, ellipsis.c_str(), &ellipsisW, &textH);
        while (!displayText.empty()) {
            std::string candidate = displayText + ellipsis;
            TTF_SizeText(globalFont, candidate.c_str(), &textW, &textH);
            if (textW <= availableWidth)
                break;
            displayText.pop_back();
        }
        displayText += ellipsis;
    }
    
    SDL_Color sdlColor = { tc.textInputText.r, tc.textInputText.g, tc.textInputText.b, tc.textInputText.a };
    SDL_Surface* surface = TTF_RenderText_Solid(globalFont, displayText.c_str(), sdlColor);
    if (!surface) return;
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    int finalTextW = surface->w, finalTextH = surface->h;
    SDL_FreeSurface(surface);
    SDL_Rect dst = { x + padding, y + padding, finalTextW, finalTextH };
    SDL_Rect clipRect = { x + padding, y + padding, availableWidth, boxHeight - 2 * padding };
    SDL_RenderSetClipRect(renderer, &clipRect);
    SDL_RenderCopy(renderer, texture, nullptr, &dst);
    SDL_RenderSetClipRect(renderer, nullptr);
    SDL_DestroyTexture(texture);
}

void TextBox::onFocusGained() {
    // Only auto-select if autoHighlight is true and there is text.
    if (autoHighlight && !content.empty()) {
        textSelected = true;
    } else {
        textSelected = false;
    }
}

void TextBox::onFocusLost() {
    textSelected = false;
}

void TextBox::handleEvent(const SDL_Event &e) {
    if (hasFocus) {
        if (e.type == SDL_KEYDOWN) {
            // If Enter is pressed, cancel selection (if any) and push a TAB event.
            if (e.key.keysym.sym == SDLK_RETURN) {
                if (textSelected)
                    textSelected = false;
                // Push a synthetic TAB key event.
                SDL_Event tabEvent;
                tabEvent.type = SDL_KEYDOWN;
                tabEvent.key.keysym.sym = SDLK_TAB;
                SDL_PushEvent(&tabEvent);
                return;
            }
            // If text is currently selected:
            if (textSelected) {
                // If right arrow is pressed, cancel selection.
                if (e.key.keysym.sym == SDLK_RIGHT) {
                    textSelected = false;
                    return;
                } else {
                    // For any other key, clear content and cancel selection.
                    content = "";
                    textSelected = false;
                    // Continue processing key event if necessary.
                }
            }
            // Process backspace normally if no selection.
            if (e.key.keysym.sym == SDLK_BACKSPACE && !content.empty()) {
                content.pop_back();
                return;
            }
        }
        if (e.type == SDL_TEXTINPUT) {
            if (textSelected) {
                // If text was selected, clear it before appending.
                content = "";
                textSelected = false;
            }
            content += e.text.text;
        }
    }
}

SDL_Rect TextBox::getFocusRect() const {
    const int padding = 5;
    initFont();
    int boxHeight = TTF_FontLineSkip(globalFont) + 2 * padding;
    return SDL_Rect{ x - 2, y - 2, width + 4, boxHeight + 4 };
}


} // namespace ui
