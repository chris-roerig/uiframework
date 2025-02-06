#include "Modal.h"
#include "../Theme/ThemeGlobals.h"
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

    // --- Draw the modal overlay ---
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    int winW, winH;
    SDL_GetRendererOutputSize(renderer, &winW, &winH);
    SDL_Rect overlay = { 0, 0, winW, winH };
    Color overlayColor = g_currentTheme->modalColors().modalOverlay;
    SDL_SetRenderDrawColor(renderer, overlayColor.r, overlayColor.g, overlayColor.b, overlayColor.a);
    SDL_RenderFillRect(renderer, &overlay);
    
    // --- Draw the modal box ---
    SDL_Rect box = { x, y, width, height };
    Color modalBg = g_currentTheme->modalColors().modalBackground;
    drawFilledRect(renderer, box, modalBg);
    
    // --- Draw the modal box border ---
    Color modalBorder = g_currentTheme->modalColors().modalBorder;
    SDL_SetRenderDrawColor(renderer, modalBorder.r, modalBorder.g, modalBorder.b, modalBorder.a);
    SDL_RenderDrawRect(renderer, &box);
    
    // --- Render the modal message text ---
    if (globalFont) {
        SDL_Color textColor = { 
            g_currentTheme->modalColors().modalText.r,
            g_currentTheme->modalColors().modalText.g,
            g_currentTheme->modalColors().modalText.b,
            g_currentTheme->modalColors().modalText.a
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
    
    // --- Draw the modal buttons ---
    int numButtons = buttonLabels.size();
    int btnW = 80;    // Fixed button width
    int btnH = 30;    // Fixed button height
    int spacing = 20; // Space between buttons
    int totalButtonsWidth = numButtons * btnW + (numButtons - 1) * spacing;
    
    // Center the buttons horizontally within the modal box.
    int startX = x + (width - totalButtonsWidth) / 2;
    int btnY = y + height - btnH - 10; // 10 pixels above the bottom edge.
    
    for (int i = 0; i < numButtons; i++) {
        int btnX = startX + i * (btnW + spacing);
        SDL_Rect btnRect = { btnX, btnY, btnW, btnH };
        
        // Fill button with the same color regardless of focus.
        Color btnBg = g_currentTheme->modalColors().modalButtonBackground;
        drawFilledRect(renderer, btnRect, btnBg);
        
        // Draw default button border.
        Color btnBorder = g_currentTheme->modalColors().modalButtonBorder;
        SDL_SetRenderDrawColor(renderer, btnBorder.r, btnBorder.g, btnBorder.b, btnBorder.a);
        SDL_RenderDrawRect(renderer, &btnRect);
        
        // If the button is focused, draw an additional highlighted border.
        if (i == buttonFocusIndex) {
            Color highlightColor = g_currentTheme->modalColors().modalButtonHighlight; // or use a dedicated modalButtonHighlight if defined
            SDL_SetRenderDrawColor(renderer, highlightColor.r, highlightColor.g, highlightColor.b, highlightColor.a);
            SDL_Rect hlRect = { btnRect.x - 1, btnRect.y - 1, btnRect.w + 2, btnRect.h + 2 };
            SDL_RenderDrawRect(renderer, &hlRect);
        }
        
        // Render the button label centered.
        if (globalFont) {
            SDL_Color btnTextColor = { 
                g_currentTheme->modalColors().modalButtonText.r,
                g_currentTheme->modalColors().modalButtonText.g,
                g_currentTheme->modalColors().modalButtonText.b,
                g_currentTheme->modalColors().modalButtonText.a
            };
            SDL_Surface* btnSurface = TTF_RenderText_Solid(globalFont, buttonLabels[i].c_str(), btnTextColor);
            if (btnSurface) {
                SDL_Texture* btnTexture = SDL_CreateTextureFromSurface(renderer, btnSurface);
                int textW = btnSurface->w, textH = btnSurface->h;
                SDL_FreeSurface(btnSurface);
                SDL_Rect dst = { btnX + (btnW - textW) / 2, btnY + (btnH - textH) / 2, textW, textH };
                SDL_RenderCopy(renderer, btnTexture, nullptr, &dst);
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
