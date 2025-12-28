#include "TextBox.h"
#include "../../lib/Theme/ThemeBase.h"
#include "../../src/Helpers.h"
#include "../../src/UICore.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <algorithm>

namespace ui {

void TextBox::render(SDL_Renderer* renderer, TTF_Font* font, std::shared_ptr<Theme> theme) {
    if (!renderer || !theme) {
        return;
    }
    
    const int padding = 5;
    int boxHeight = height;
    if (font) {
        boxHeight = std::max(height, TTF_FontLineSkip(font) + 2 * padding);
    }
    
    SDL_Rect rect = { x, y, width, boxHeight };
    ThemeableElementColors tc = theme->textInputColors();
    
    // Draw background
    drawFilledRect(renderer, rect, tc.textInputBackground);
    
    // Draw 3D border
    SDL_SetRenderDrawColor(renderer, tc.textInputBorderLight.r, tc.textInputBorderLight.g,
                           tc.textInputBorderLight.b, tc.textInputBorderLight.a);
    SDL_RenderDrawLine(renderer, x, y, x + width, y);
    SDL_RenderDrawLine(renderer, x, y, x, y + boxHeight);
    
    SDL_SetRenderDrawColor(renderer, tc.textInputBorderDark.r, tc.textInputBorderDark.g,
                           tc.textInputBorderDark.b, tc.textInputBorderDark.a);
    SDL_RenderDrawLine(renderer, x, y + boxHeight, x + width, y + boxHeight);
    SDL_RenderDrawLine(renderer, x + width, y, x + width, y + boxHeight);
    
    // Draw focus indicator
    if (hasFocus) {
        SDL_Rect focusRect = getFocusRect();
        SDL_SetRenderDrawColor(renderer, tc.textInputText.r, tc.textInputText.g, tc.textInputText.b, tc.textInputText.a);
        SDL_RenderDrawRect(renderer, &focusRect);
    }
    
    // Render text content
    if (font && !content.empty()) {
        int availableWidth = width - 2 * padding;
        if (availableWidth <= 0) return;
        
        std::string displayText = content;
        
        // Handle text overflow with ellipsis
        int textW = 0, textH = 0;
        if (TTF_SizeText(font, displayText.c_str(), &textW, &textH) == 0 && textW > availableWidth) {
            std::string ellipsis = "...";
            int ellipsisW = 0;
            if (TTF_SizeText(font, ellipsis.c_str(), &ellipsisW, &textH) == 0) {
                // If ellipsis itself is too wide, just truncate
                if (ellipsisW >= availableWidth) {
                    displayText = "";
                } else {
                    // Binary search for optimal truncation point
                    int left = 0, right = static_cast<int>(displayText.length());
                    while (left < right) {
                        int mid = (left + right + 1) / 2;
                        std::string candidate = displayText.substr(0, mid) + ellipsis;
                        int candidateW = 0;
                        if (TTF_SizeText(font, candidate.c_str(), &candidateW, nullptr) == 0 && candidateW <= availableWidth) {
                            left = mid;
                        } else {
                            right = mid - 1;
                        }
                    }
                    if (left > 0) {
                        displayText = displayText.substr(0, left) + ellipsis;
                    } else {
                        displayText = "";
                    }
                }
            }
        }
        
        if (!displayText.empty()) {
            SDL_Color textColor = { tc.textInputText.r, tc.textInputText.g, tc.textInputText.b, tc.textInputText.a };
            SDL_Surface* surface = nullptr;
            
            if (textSelected) {
                // Render selected text with highlighted background
                SDL_Color bgColor = { tc.textInputText.r, tc.textInputText.g, tc.textInputText.b, tc.textInputText.a };
                SDL_Color selTextColor = { tc.textInputBackground.r, tc.textInputBackground.g, tc.textInputBackground.b, tc.textInputBackground.a };
                surface = TTF_RenderText_Shaded(font, displayText.c_str(), selTextColor, bgColor);
            } else {
                surface = TTF_RenderText_Solid(font, displayText.c_str(), textColor);
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
    if (!visible) return;
    
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
    } else if (e.type == SDL_TEXTINPUT && hasFocus && !textSelected) {
        // Insert text at cursor position
        std::string inputText = e.text.text;
        if (!inputText.empty()) {
            content.insert(cursorPosition, inputText);
            cursorPosition += inputText.length();
        }
    } else if (e.type == SDL_KEYDOWN && hasFocus) {
        switch (e.key.keysym.sym) {
            case SDLK_BACKSPACE:
                if (textSelected) {
                    content.clear();
                    cursorPosition = 0;
                    textSelected = false;
                } else if (cursorPosition > 0) {
                    content.erase(cursorPosition - 1, 1);
                    cursorPosition--;
                }
                break;
            case SDLK_DELETE:
                if (textSelected) {
                    content.clear();
                    cursorPosition = 0;
                    textSelected = false;
                } else if (cursorPosition < content.length()) {
                    content.erase(cursorPosition, 1);
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
    if (autoHighlight) {
        selectAll();
    }
}

void TextBox::onFocusLost() {
    hasFocus = false;
    textSelected = false;
}

void TextBox::setText(const std::string& text) {
    content = text;
    // Validate cursor position
    cursorPosition = std::min(cursorPosition, content.length());
    textSelected = false;
}

void TextBox::selectAll() {
    textSelected = !content.empty();
}

void TextBox::clearSelection() {
    textSelected = false;
}

} // namespace ui
