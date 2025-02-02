#include "Modal.h"
#include "ThemeGlobals.h"
#include "ThemeBase.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>

namespace ui {

Modal::Modal(int x_, int y_, int w_, int h_, const std::string &msg)
    : UIElement(x_, y_, w_, h_), message(msg), buttonFocusIndex(0), dismissed(false)
{
    // For a generic modal (info modal), we default to one button labeled "OK".
    // (For a confirm modal, the UI façade will override these fields.)
    if(buttonLabels.empty()) {
        buttonLabels.push_back("OK");
        // Default callback: simply dismiss the modal.
        buttonCallbacks.push_back([this]() {
            dismissed = true;
        });
    }
}

void Modal::render(SDL_Renderer* renderer) {
    if (dismissed)
        return;
    
    initFont();
    // Draw a semi-transparent overlay over the whole window.
    SDL_Rect overlay = {0, 0, 800, 600};  // Ideally, derive from actual window dimensions.
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 128);
    SDL_RenderFillRect(renderer, &overlay);
    
    // Draw the modal box.
    SDL_Rect box = { x, y, width, height };
    drawFilledRect(renderer, box, g_currentTheme->buttonColors().buttonBackground);
    
    // Draw a border around the modal box.
    SDL_SetRenderDrawColor(renderer,
                           g_currentTheme->buttonColors().buttonBorderDark.r,
                           g_currentTheme->buttonColors().buttonBorderDark.g,
                           g_currentTheme->buttonColors().buttonBorderDark.b,
                           g_currentTheme->buttonColors().buttonBorderDark.a);
    SDL_RenderDrawRect(renderer, &box);
    
    // Render the message text centered (adjust vertical offset if needed).
    if (globalFont) {
        SDL_Color textColor = { 
            g_currentTheme->buttonColors().buttonText.r,
            g_currentTheme->buttonColors().buttonText.g,
            g_currentTheme->buttonColors().buttonText.b,
            g_currentTheme->buttonColors().buttonText.a 
        };
        SDL_Surface* surface = TTF_RenderText_Solid(globalFont, message.c_str(), textColor);
        if (surface) {
            SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
            int textW = surface->w, textH = surface->h;
            SDL_FreeSurface(surface);
            SDL_Rect dst = { x + (width - textW) / 2, y + (height - textH) / 2 - 20, textW, textH };
            SDL_RenderCopy(renderer, texture, nullptr, &dst);
            SDL_DestroyTexture(texture);
        }
    }
    
    // --- Draw the Buttons ---
    // For this example, we lay out the buttons in a row at the bottom of the modal.
    // Calculate total button area width.
    int numButtons = buttonLabels.size();
    int btnW = 80;   // Fixed button width (could also be computed from text).
    int btnH = 30;   // Fixed button height.
    int spacing = 20; // Space between buttons.
    int totalBtnAreaWidth = numButtons * btnW + (numButtons - 1) * spacing;
    
    // Start X such that buttons are centered horizontally in the modal.
    int startX = x + (width - totalBtnAreaWidth) / 2;
    int btnY = y + height - btnH - 10; // 10 pixels above modal bottom.
    
    // Render each button.
    for (int i = 0; i < numButtons; i++) {
        int btnX = startX + i * (btnW + spacing);
        SDL_Rect btnRect = { btnX, btnY, btnW, btnH };
        
        // Highlight the button if it has focus.
        if (i == buttonFocusIndex)
            drawFilledRect(renderer, btnRect, g_currentTheme->buttonColors().buttonBackground);
        else
            drawFilledRect(renderer, btnRect, g_currentTheme->buttonColors().buttonForeground);
        
        // Draw a border around the button.
        SDL_SetRenderDrawColor(renderer,
                               g_currentTheme->buttonColors().buttonBorderDark.r,
                               g_currentTheme->buttonColors().buttonBorderDark.g,
                               g_currentTheme->buttonColors().buttonBorderDark.b,
                               g_currentTheme->buttonColors().buttonBorderDark.a);
        SDL_RenderDrawRect(renderer, &btnRect);
        
        // Render the button label (centered).
        if (globalFont) {
            SDL_Color btnTextColor = { 
                g_currentTheme->buttonColors().buttonText.r,
                g_currentTheme->buttonColors().buttonText.g,
                g_currentTheme->buttonColors().buttonText.b,
                g_currentTheme->buttonColors().buttonText.a 
            };
            SDL_Surface* btnSurface = TTF_RenderText_Solid(globalFont, buttonLabels[i].c_str(), btnTextColor);
            if (btnSurface) {
                SDL_Texture* btnTexture = SDL_CreateTextureFromSurface(renderer, btnSurface);
                int textW = btnSurface->w, textH = btnSurface->h;
                SDL_FreeSurface(btnSurface);
                SDL_Rect btnDst = { btnX + (btnW - textW) / 2, btnY + (btnH - textH) / 2, textW, textH };
                SDL_RenderCopy(renderer, btnTexture, nullptr, &btnDst);
                SDL_DestroyTexture(btnTexture);
            }
        }
    }
}

void Modal::handleEvent(const SDL_Event &e) {
    if (dismissed)
        return; // Skip processing if already dismissed.
    
    if (e.type == SDL_KEYDOWN) {
        // ESC always dismisses the modal.
        if (e.key.keysym.sym == SDLK_ESCAPE) {
            dismissed = true;
            if (onDismiss)
                onDismiss();
            hasFocus = false;
            return;
        }
        // Allow navigation between buttons.
        else if (e.key.keysym.sym == SDLK_LEFT) {
            if (buttonFocusIndex > 0)
                buttonFocusIndex--;
            else
                buttonFocusIndex = buttonLabels.size() - 1;
            return;
        }
        else if (e.key.keysym.sym == SDLK_RIGHT) {
            buttonFocusIndex = (buttonFocusIndex + 1) % buttonLabels.size();
            return;
        }
        // If ENTER is pressed, trigger the callback and dismiss.
        else if (e.key.keysym.sym == SDLK_RETURN) {
            if (buttonFocusIndex >= 0 && buttonFocusIndex < buttonCallbacks.size()) {
                auto cb = buttonCallbacks[buttonFocusIndex];
                if (cb)
                    cb();
            }
            dismissed = true;
            if (onDismiss)
                onDismiss();
            hasFocus = false;
            return;
        }
    }
}


SDL_Rect Modal::getFocusRect() const {
    return SDL_Rect{ x, y, width, height };
}

} // namespace ui
