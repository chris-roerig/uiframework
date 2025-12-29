#include "Modal.h"
#include "Theme/ThemeBase.h"
#include "Helpers.h"
#include "UICore.h"
#include "../Constants.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <algorithm>

namespace ui {

Modal::Modal(int x_, int y_, int w_, int h_, const std::string &msg, const std::string &buttonText, 
             bool hasCancel, std::function<void()> onCloseCallback)
    : UIElement(x_, y_, w_, h_), message(msg), onDismiss(onCloseCallback) {
    
    // Add primary button
    addButton(buttonText, [this, onCloseCallback]() {
        // Dismiss immediately, but defer the user callback
        dismiss();
        if (onCloseCallback && coreRef) {
            coreRef->queueCallback(onCloseCallback);
        }
    });
    
    // Add cancel button if requested
    if (hasCancel) {
        addButton("Cancel", [this]() {
            dismiss();
        });
    }
}

void Modal::render(SDL_Renderer* renderer, TTF_Font* font, std::shared_ptr<Theme> theme) {
    if (!renderer || !theme || dismissed) {
        return;
    }
    
    ThemeableElementColors tc = theme->modalColors();
    
    // Draw semi-transparent overlay over entire screen
    if (coreRef) {
        SDL_Rect screenRect = { 0, 0, 800, 600 }; // Use default screen size for now
        Color overlayColor = { 0, 0, 0, 128 }; // Semi-transparent black
        drawFilledRect(renderer, screenRect, overlayColor);
    }
    
    // Draw modal background
    SDL_Rect modalRect = { x, y, width, height };
    drawFilledRect(renderer, modalRect, tc.modalBackground);
    
    // Draw modal border
    SDL_SetRenderDrawColor(renderer, tc.modalBorder.r, tc.modalBorder.g, 
                          tc.modalBorder.b, tc.modalBorder.a);
    SDL_RenderDrawRect(renderer, &modalRect);
    
    // Draw title bar (optional)
    SDL_Rect titleRect = { x, y, width, 30 };
    Color titleColor = tc.modalBackground;
    titleColor.r = std::max(0, static_cast<int>(titleColor.r) - 20);
    titleColor.g = std::max(0, static_cast<int>(titleColor.g) - 20);
    titleColor.b = std::max(0, static_cast<int>(titleColor.b) - 20);
    drawFilledRect(renderer, titleRect, titleColor);
    
    // Draw message text
    if (font && !message.empty()) {
        SDL_Color textColor = { tc.modalText.r, tc.modalText.g, tc.modalText.b, tc.modalText.a };
        
        // Simple word wrapping for message
        const int padding = Constants::MODAL_PADDING;
        const int lineHeight = TTF_FontLineSkip(font);
        int textY = y + 40; // Below title bar
        int availableWidth = width - 2 * padding;
        
        // Split message into words
        std::vector<std::string> words;
        std::string currentWord;
        for (char c : message) {
            if (c == ' ' || c == '\n') {
                if (!currentWord.empty()) {
                    words.push_back(currentWord);
                    currentWord.clear();
                }
                if (c == '\n') {
                    words.push_back("\n");
                }
            } else {
                currentWord += c;
            }
        }
        if (!currentWord.empty()) {
            words.push_back(currentWord);
        }
        
        // Render words with wrapping
        std::string currentLine;
        for (const auto& word : words) {
            if (word == "\n") {
                // Render current line and start new one
                if (!currentLine.empty()) {
                    SDL_Surface* surface = TTF_RenderText_Solid(font, currentLine.c_str(), textColor);
                    if (surface) {
                        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
                        if (texture) {
                            SDL_Rect dst = { x + padding, textY, surface->w, surface->h };
                            SDL_RenderCopy(renderer, texture, nullptr, &dst);
                            SDL_DestroyTexture(texture);
                        }
                        SDL_FreeSurface(surface);
                    }
                    currentLine.clear();
                }
                textY += lineHeight;
                continue;
            }
            
            std::string testLine = currentLine.empty() ? word : currentLine + " " + word;
            int testWidth = 0;
            TTF_SizeText(font, testLine.c_str(), &testWidth, nullptr);
            
            if (testWidth <= availableWidth) {
                currentLine = testLine;
            } else {
                // Render current line and start new one with current word
                if (!currentLine.empty()) {
                    SDL_Surface* surface = TTF_RenderText_Solid(font, currentLine.c_str(), textColor);
                    if (surface) {
                        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
                        if (texture) {
                            SDL_Rect dst = { x + padding, textY, surface->w, surface->h };
                            SDL_RenderCopy(renderer, texture, nullptr, &dst);
                            SDL_DestroyTexture(texture);
                        }
                        SDL_FreeSurface(surface);
                    }
                    textY += lineHeight;
                }
                currentLine = word;
            }
        }
        
        // Render final line
        if (!currentLine.empty()) {
            SDL_Surface* surface = TTF_RenderText_Solid(font, currentLine.c_str(), textColor);
            if (surface) {
                SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
                if (texture) {
                    SDL_Rect dst = { x + padding, textY, surface->w, surface->h };
                    SDL_RenderCopy(renderer, texture, nullptr, &dst);
                    SDL_DestroyTexture(texture);
                }
                SDL_FreeSurface(surface);
            }
        }
    }
    
    // Draw buttons
    const int buttonHeight = Constants::BUTTON_HEIGHT;
    const int buttonSpacing = Constants::BUTTON_SPACING;
    const int buttonY = y + height - buttonHeight - Constants::MODAL_PADDING;
    
    if (!buttonLabels.empty()) {
        int totalButtonWidth = 0;
        for (const auto& label : buttonLabels) {
            int buttonWidth = 80; // Default width
            if (font) {
                int textWidth = 0;
                TTF_SizeText(font, label.c_str(), &textWidth, nullptr);
                buttonWidth = std::max(80, textWidth + 20);
            }
            totalButtonWidth += buttonWidth;
        }
        totalButtonWidth += (buttonLabels.size() - 1) * buttonSpacing;
        
        int buttonX = x + (width - totalButtonWidth) / 2;
        
        for (size_t i = 0; i < buttonLabels.size(); i++) {
            int buttonWidth = 80;
            if (font) {
                int textWidth = 0;
                TTF_SizeText(font, buttonLabels[i].c_str(), &textWidth, nullptr);
                buttonWidth = std::max(80, textWidth + 20);
            }
            
            SDL_Rect buttonRect = { buttonX, buttonY, buttonWidth, buttonHeight };
            
            // Draw button background
            Color buttonBg = tc.modalButtonBackground;
            if (static_cast<int>(i) == buttonFocusIndex) {
                // Highlight focused button
                buttonBg.r = std::min(255, static_cast<int>(buttonBg.r) + 30);
                buttonBg.g = std::min(255, static_cast<int>(buttonBg.g) + 30);
                buttonBg.b = std::min(255, static_cast<int>(buttonBg.b) + 30);
            }
            drawFilledRect(renderer, buttonRect, buttonBg);
            
            // Draw button border
            SDL_SetRenderDrawColor(renderer, tc.modalButtonBorder.r, tc.modalButtonBorder.g, 
                                  tc.modalButtonBorder.b, tc.modalButtonBorder.a);
            SDL_RenderDrawRect(renderer, &buttonRect);
            
            // Draw button text
            if (font) {
                SDL_Color buttonTextColor = { tc.modalButtonText.r, tc.modalButtonText.g, 
                                            tc.modalButtonText.b, tc.modalButtonText.a };
                SDL_Surface* surface = TTF_RenderText_Solid(font, buttonLabels[i].c_str(), buttonTextColor);
                if (surface) {
                    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
                    if (texture) {
                        SDL_Rect textRect = { 
                            buttonX + (buttonWidth - surface->w) / 2,
                            buttonY + (buttonHeight - surface->h) / 2,
                            surface->w, 
                            surface->h 
                        };
                        SDL_RenderCopy(renderer, texture, nullptr, &textRect);
                        SDL_DestroyTexture(texture);
                    }
                    SDL_FreeSurface(surface);
                }
            }
            
            buttonX += buttonWidth + buttonSpacing;
        }
    }
}

void Modal::handleEvent(const SDL_Event &e) {
    if (!visible || dismissed) return;
    
    if (e.type == SDL_KEYDOWN) {
        switch (e.key.keysym.sym) {
            case SDLK_ESCAPE:
                dismiss();
                break;
            case SDLK_RETURN:
                activate();
                break;
            case SDLK_TAB:
                if (!buttonLabels.empty()) {
                    buttonFocusIndex = (buttonFocusIndex + 1) % buttonLabels.size();
                }
                break;
            case SDLK_LEFT:
                if (buttonFocusIndex > 0) {
                    buttonFocusIndex--;
                }
                break;
            case SDLK_RIGHT:
                if (buttonFocusIndex < static_cast<int>(buttonLabels.size()) - 1) {
                    buttonFocusIndex++;
                }
                break;
        }
    } else if (e.type == SDL_MOUSEBUTTONDOWN) {
        if (e.button.button == SDL_BUTTON_LEFT) {
            int mouseX = e.button.x;
            int mouseY = e.button.y;
            
            int clickedButton = getButtonAt(mouseX, mouseY);
            if (clickedButton >= 0) {
                buttonFocusIndex = clickedButton;
                activate();
            } else if (!containsPoint(mouseX, mouseY)) {
                // Click outside modal - dismiss if allowed
                dismiss();
            }
        }
    }
}

SDL_Rect Modal::getFocusRect() const {
    return SDL_Rect{ x - 2, y - 2, width + 4, height + 4 };
}

void Modal::activate() {
    if (buttonFocusIndex >= 0 && buttonFocusIndex < static_cast<int>(buttonCallbacks.size())) {
        if (buttonCallbacks[buttonFocusIndex] && coreRef) {
            // Queue the button callback to avoid deadlock
            coreRef->queueCallback(buttonCallbacks[buttonFocusIndex]);
        }
    }
}

void Modal::dismiss() {
    dismissed = true;
    // Don't queue onDismiss here - it's handled by button callbacks
}

void Modal::addButton(const std::string& label, std::function<void()> callback) {
    buttonLabels.push_back(label);
    buttonCallbacks.push_back(callback);
}

void Modal::clearButtons() {
    buttonLabels.clear();
    buttonCallbacks.clear();
    buttonFocusIndex = 0;
}

void Modal::setButtonFocus(int index) {
    if (index >= 0 && index < static_cast<int>(buttonLabels.size())) {
        buttonFocusIndex = index;
    }
}

void Modal::centerOnScreen(int screenWidth, int screenHeight) {
    x = (screenWidth - width) / 2;
    y = (screenHeight - height) / 2;
}

SDL_Rect Modal::getButtonRect(int buttonIndex) const {
    if (buttonIndex < 0 || buttonIndex >= static_cast<int>(buttonLabels.size())) {
        return {0, 0, 0, 0};
    }
    
    const int buttonHeight = Constants::BUTTON_HEIGHT;
    const int buttonSpacing = Constants::BUTTON_SPACING;
    const int buttonY = y + height - buttonHeight - Constants::MODAL_PADDING;
    
    // Calculate button widths (same logic as rendering)
    std::vector<int> buttonWidths;
    int totalButtonWidth = 0;
    
    for (size_t i = 0; i < buttonLabels.size(); i++) {
        int buttonWidth = 80; // Default width - matches rendering
        // Note: We can't access font here, so we use default width
        // This is a limitation, but better than the previous mismatch
        buttonWidths.push_back(buttonWidth);
        totalButtonWidth += buttonWidth;
    }
    totalButtonWidth += (buttonLabels.size() - 1) * buttonSpacing;
    
    // Center buttons horizontally (same as rendering)
    int buttonX = x + (width - totalButtonWidth) / 2;
    
    // Calculate position for requested button
    for (int i = 0; i < buttonIndex; i++) {
        buttonX += buttonWidths[i] + buttonSpacing;
    }
    
    return {buttonX, buttonY, buttonWidths[buttonIndex], buttonHeight};
}

int Modal::getButtonAt(int mouseX, int mouseY) const {
    for (int i = 0; i < static_cast<int>(buttonLabels.size()); i++) {
        SDL_Rect buttonRect = getButtonRect(i);
        if (mouseX >= buttonRect.x && mouseX < buttonRect.x + buttonRect.w &&
            mouseY >= buttonRect.y && mouseY < buttonRect.y + buttonRect.h) {
            return i;
        }
    }
    return -1;
}

} // namespace ui
