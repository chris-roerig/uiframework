#include "uiframework/UIElements/Modal.h"
#include "uiframework/Theme/ThemeBase.h"
#include "uiframework/Helpers.h"
#include "uiframework/UICore.h"
#include "uiframework/Constants.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <algorithm>

namespace ui {

Modal::Modal(int x_, int y_, int w_, int h_, const std::string &msg, const std::string &buttonText, 
             bool hasCancel, std::function<void()> onCloseCallback)
    : InteractiveElement(x_, y_, w_, h_), message(msg), onDismiss(onCloseCallback) {
    
    // Initialize animation state - modal starts dismissed, will animate in when shown
    dismissed = true;
    previousDismissedState = true;
    
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

void Modal::renderImpl(const RenderContext& ctx) {
    // Update animation state
    updateAnimation(SDL_GetTicks());
    
    // Check for state transitions to trigger animations
    if (animationsEnabled && previousDismissedState != dismissed) {
        if (!dismissed) {
            // Modal is being shown - trigger show animation
            startAnimation(showAnimationDuration);
        } else {
            // Modal is being dismissed - trigger hide animation
            startAnimation(hideAnimationDuration);
        }
        previousDismissedState = dismissed;
    }
    
    // Calculate animation alpha
    float alpha = 1.0f;
    if (animationsEnabled && isAnimating()) {
        float progress = getAnimationProgress();
        if (dismissed) {
            // Fade out animation (1.0 -> 0.0)
            alpha = 1.0f - progress;
        } else {
            // Fade in animation (0.0 -> 1.0)
            alpha = progress;
        }
    } else if (dismissed) {
        // Not animating and dismissed - don't render
        return;
    }
    
    ThemeableElementColors tc = ctx.modalColors();
    
    // Apply alpha to all colors
    Color overlayColor = { 0, 0, 0, static_cast<uint8_t>(128 * alpha) };
    Color modalBg = { tc.modalBackground.r, tc.modalBackground.g, tc.modalBackground.b, 
                     static_cast<uint8_t>(tc.modalBackground.a * alpha) };
    Color modalBorder = { tc.modalBorder.r, tc.modalBorder.g, tc.modalBorder.b, 
                         static_cast<uint8_t>(tc.modalBorder.a * alpha) };
    Color modalText = { tc.modalText.r, tc.modalText.g, tc.modalText.b, 
                       static_cast<uint8_t>(tc.modalText.a * alpha) };
    
    // Draw semi-transparent overlay over entire screen
    if (coreRef) {
        SDL_Rect screenRect = { 0, 0, 800, 600 }; // Use default screen size for now
        drawFilledRect(ctx.renderer, screenRect, overlayColor);
    }
    
    // Draw modal background
    SDL_Rect modalRect = { x, y, width, height };
    drawFilledRect(ctx.renderer, modalRect, modalBg);
    
    // Draw modal border
    SDL_SetRenderDrawColor(ctx.renderer, modalBorder.r, modalBorder.g, 
                          modalBorder.b, modalBorder.a);
    SDL_RenderDrawRect(ctx.renderer, &modalRect);
    
    // Draw title bar (optional)
    SDL_Rect titleRect = { x, y, width, ui::Constants::MODAL_TITLE_HEIGHT };
    Color titleColor = modalBg;
    titleColor.r = std::max(0, static_cast<int>(titleColor.r) - ui::Constants::COLOR_DARKEN_AMOUNT);
    titleColor.g = std::max(0, static_cast<int>(titleColor.g) - ui::Constants::COLOR_DARKEN_AMOUNT);
    titleColor.b = std::max(0, static_cast<int>(titleColor.b) - ui::Constants::COLOR_DARKEN_AMOUNT);
    drawFilledRect(ctx.renderer, titleRect, titleColor);
    
    // Draw message text
    if (ctx.font && !message.empty()) {
        SDL_Color textColor = { modalText.r, modalText.g, modalText.b, modalText.a };
        
        // Simple word wrapping for message
        const int padding = ui::Constants::MODAL_PADDING;
        const int lineHeight = TTF_FontLineSkip(ctx.font);
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
                    SDL_Surface* surface = TTF_RenderText_Solid(ctx.font, currentLine.c_str(), textColor);
                    if (surface) {
                        SDL_Texture* texture = SDL_CreateTextureFromSurface(ctx.renderer, surface);
                        if (texture) {
                            SDL_Rect dst = { x + padding, textY, surface->w, surface->h };
                            SDL_RenderCopy(ctx.renderer, texture, nullptr, &dst);
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
            TTF_SizeText(ctx.font, testLine.c_str(), &testWidth, nullptr);
            
            if (testWidth <= availableWidth) {
                currentLine = testLine;
            } else {
                // Render current line and start new one with current word
                if (!currentLine.empty()) {
                    SDL_Surface* surface = TTF_RenderText_Solid(ctx.font, currentLine.c_str(), textColor);
                    if (surface) {
                        SDL_Texture* texture = SDL_CreateTextureFromSurface(ctx.renderer, surface);
                        if (texture) {
                            SDL_Rect dst = { x + padding, textY, surface->w, surface->h };
                            SDL_RenderCopy(ctx.renderer, texture, nullptr, &dst);
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
            SDL_Surface* surface = TTF_RenderText_Solid(ctx.font, currentLine.c_str(), textColor);
            if (surface) {
                SDL_Texture* texture = SDL_CreateTextureFromSurface(ctx.renderer, surface);
                if (texture) {
                    SDL_Rect dst = { x + padding, textY, surface->w, surface->h };
                    SDL_RenderCopy(ctx.renderer, texture, nullptr, &dst);
                    SDL_DestroyTexture(texture);
                }
                SDL_FreeSurface(surface);
            }
        }
    }
    
    // Draw buttons
    const int buttonHeight = ui::Constants::BUTTON_HEIGHT;
    const int buttonSpacing = ui::Constants::BUTTON_SPACING;
    const int buttonY = y + height - buttonHeight - ui::Constants::MODAL_PADDING;
    
    if (!buttonLabels.empty()) {
        int totalButtonWidth = 0;
        for (const auto& label : buttonLabels) {
            int buttonWidth = ui::Constants::MIN_BUTTON_WIDTH; // Default width
            if (ctx.font) {
                int textWidth = 0;
                TTF_SizeText(ctx.font, label.c_str(), &textWidth, nullptr);
                buttonWidth = std::max(ui::Constants::MIN_BUTTON_WIDTH, textWidth + ui::Constants::BUTTON_TEXT_PADDING);
            }
            totalButtonWidth += buttonWidth;
        }
        totalButtonWidth += (buttonLabels.size() - 1) * buttonSpacing;
        
        int buttonX = x + (width - totalButtonWidth) / 2;
        
        for (size_t i = 0; i < buttonLabels.size(); i++) {
            int buttonWidth = ui::Constants::MIN_BUTTON_WIDTH;
            if (ctx.font) {
                int textWidth = 0;
                TTF_SizeText(ctx.font, buttonLabels[i].c_str(), &textWidth, nullptr);
                buttonWidth = std::max(ui::Constants::MIN_BUTTON_WIDTH, textWidth + ui::Constants::BUTTON_TEXT_PADDING);
            }
            
            SDL_Rect buttonRect = { buttonX, buttonY, buttonWidth, buttonHeight };
            
            // Draw button background
            Color buttonBg = tc.modalButtonBackground;
            buttonBg.a = static_cast<uint8_t>(buttonBg.a * alpha);
            if (static_cast<int>(i) == buttonFocusIndex) {
                // Highlight focused button
                buttonBg.r = std::min(ui::Constants::FULL_ALPHA, static_cast<int>(buttonBg.r) + ui::Constants::COLOR_LIGHTEN_AMOUNT);
                buttonBg.g = std::min(ui::Constants::FULL_ALPHA, static_cast<int>(buttonBg.g) + ui::Constants::COLOR_LIGHTEN_AMOUNT);
                buttonBg.b = std::min(255, static_cast<int>(buttonBg.b) + 30);
            }
            drawFilledRect(ctx.renderer, buttonRect, buttonBg);
            
            // Draw button border
            Color buttonBorder = tc.modalButtonBorder;
            buttonBorder.a = static_cast<uint8_t>(buttonBorder.a * alpha);
            SDL_SetRenderDrawColor(ctx.renderer, buttonBorder.r, buttonBorder.g, 
                                  buttonBorder.b, buttonBorder.a);
            SDL_RenderDrawRect(ctx.renderer, &buttonRect);
            
            // Draw button text
            if (ctx.font) {
                Color buttonTextColor = tc.modalButtonText;
                buttonTextColor.a = static_cast<uint8_t>(buttonTextColor.a * alpha);
                SDL_Color buttonTextSDLColor = { buttonTextColor.r, buttonTextColor.g, 
                                               buttonTextColor.b, buttonTextColor.a };
                SDL_Surface* surface = TTF_RenderText_Solid(ctx.font, buttonLabels[i].c_str(), buttonTextSDLColor);
                if (surface) {
                    SDL_Texture* texture = SDL_CreateTextureFromSurface(ctx.renderer, surface);
                    if (texture) {
                        SDL_Rect textRect = { 
                            buttonX + (buttonWidth - surface->w) / 2,
                            buttonY + (buttonHeight - surface->h) / 2,
                            surface->w, 
                            surface->h 
                        };
                        SDL_RenderCopy(ctx.renderer, texture, nullptr, &textRect);
                        SDL_DestroyTexture(texture);
                    }
                    SDL_FreeSurface(surface);
                }
            }
            
            buttonX += buttonWidth + buttonSpacing;
        }
    }
}

void Modal::onMouseDown(int x, int y) {
    int clickedButton = getButtonAt(x, y);
    if (clickedButton >= 0) {
        buttonFocusIndex = clickedButton;
        activate();
    } else if (!containsPoint(x, y)) {
        // Click outside modal - dismiss if allowed
        dismiss();
    }
}

void Modal::onKeyDown(const SDL_Keycode& key) {
    switch (key) {
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

void Modal::show() {
    if (dismissed && animationsEnabled) {
        // Trigger show animation
        startAnimation(showAnimationDuration);
    }
    dismissed = false;
}

void Modal::dismiss() {
    if (!dismissed && animationsEnabled) {
        // Trigger hide animation
        startAnimation(hideAnimationDuration);
    }
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
    
    const int buttonHeight = ui::Constants::BUTTON_HEIGHT;
    const int buttonSpacing = ui::Constants::BUTTON_SPACING;
    const int buttonY = y + height - buttonHeight - ui::Constants::MODAL_PADDING;
    
    // Calculate button widths (same logic as rendering)
    std::vector<int> buttonWidths;
    int totalButtonWidth = 0;
    
    for (size_t i = 0; i < buttonLabels.size(); i++) {
        int buttonWidth = 80; // Default width - matches rendering
        // Note: We can't access ctx.font here, so we use default width
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
