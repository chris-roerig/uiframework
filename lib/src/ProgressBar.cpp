#include "uiframework/UIElements/ProgressBar.h"
#include "uiframework/Theme/ThemeBase.h"
#include "uiframework/Helpers.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <algorithm>

namespace ui {

// Constructor
ProgressBar::ProgressBar(int x, int y, int width, int height, float initValue, bool showText)
    : UIElement(x, y, width, height), progress(std::clamp(initValue, 0.0f, 1.0f)), showText(showText) {}

std::pair<int, int> ProgressBar::getTextSize(const std::string &text, TTF_Font* font) const {
    if (!font) return {0, 0};

    int textW, textH;
    if (TTF_SizeText(font, text.c_str(), &textW, &textH) == 0) {
        return {textW, textH};
    }

    return {0, 0}; // Fallback if there's an error
}

void ProgressBar::renderImpl(const RenderContext& ctx) {
    // Process real-time updates
    int progressBufferIndex = currentProgressBuffer.load();
    if (progressBuffers[progressBufferIndex] != progress) {
        progress = std::clamp(progressBuffers[progressBufferIndex], 0.0f, 1.0f);
    }
    
    ThemeableElementColors tc = ctx.progressBarColors();

    // Draw border
    SDL_Rect borderRect = { x, y, width, height };
    SDL_SetRenderDrawColor(ctx.renderer, tc.progressBarBorder.r, tc.progressBarBorder.g, tc.progressBarBorder.b, tc.progressBarBorder.a);
    SDL_RenderDrawRect(ctx.renderer, &borderRect);

    // Draw background (inside the border)
    SDL_Rect bgRect = { x + 1, y + 1, width - 2, height - 2 };
    drawFilledRect(ctx.renderer, bgRect, tc.progressBarBackground);

    // Draw progress fill (ensuring it stays within borders)
    int fillWidth = static_cast<int>((width - 2) * progress);
    if (fillWidth > 0) {
        fillWidth = std::max(fillWidth, 1); // Ensure at least 1px fill when progress > 0
        SDL_Rect fillRect = { x + 1, y + 1, fillWidth, height - 2 };
        drawFilledRect(ctx.renderer, fillRect, tc.progressBarForeground);
    }

    // Draw focus indicator
    if (hasFocus) {
        SDL_Rect focusRect = getFocusRect();
        SDL_SetRenderDrawColor(ctx.renderer, tc.progressBarBorder.r, tc.progressBarBorder.g, tc.progressBarBorder.b, tc.progressBarBorder.a);
        SDL_RenderDrawRect(ctx.renderer, &focusRect);
    }

    // Display percentage text (if enabled)
    if (showText && ctx.font) {
        std::string progressText = std::to_string(getPercentage()) + "%";
        auto [textW, textH] = getTextSize(progressText, ctx.font);
        
        if (textW > 0 && textH > 0) {
            // Center the text in the progress bar
            int textX = x + (width - textW) / 2;
            int textY = y + (height - textH) / 2;
            
            // Choose text color based on progress - use contrasting color
            SDL_Color textColor;
            if (progress > 0.5f) {
                // Use background color for contrast against filled area
                textColor = { tc.progressBarBackground.r, tc.progressBarBackground.g, tc.progressBarBackground.b, tc.progressBarBackground.a };
            } else {
                // Use foreground color for contrast against background
                textColor = { tc.progressBarForeground.r, tc.progressBarForeground.g, tc.progressBarForeground.b, tc.progressBarForeground.a };
            }
            
            SDL_Surface* surface = TTF_RenderText_Solid(ctx.font, progressText.c_str(), textColor);
            if (surface) {
                SDL_Texture* texture = SDL_CreateTextureFromSurface(ctx.renderer, surface);
                if (texture) {
                    SDL_Rect dst = { textX, textY, surface->w, surface->h };
                    SDL_RenderCopy(ctx.renderer, texture, nullptr, &dst);
                    SDL_DestroyTexture(texture);
                }
                SDL_FreeSurface(surface);
            }
        }
    }
}

void ProgressBar::setProgress(float value) {
    progress = std::clamp(value, 0.0f, 1.0f);
}

void ProgressBar::incrementProgress(float step) {
    setProgress(progress + step);
}

void ProgressBar::reset(float initValue) {
    setProgress(initValue);
}

std::pair<int, int> ProgressBar::getPreferredSize(TTF_Font* font) const {
    return {200, 20}; // Standard progress bar size
}

std::pair<int, int> ProgressBar::getMinimumSize() const {
    return {50, 10}; // Minimum visible progress bar
}

// Real-time safe methods (lock-free, audio thread safe)
void ProgressBar::realtimeSetProgress(float value) {
    int nextBuffer = 1 - currentProgressBuffer.load();
    progressBuffers[nextBuffer] = value;
    currentProgressBuffer.store(nextBuffer);
}

} // namespace ui
