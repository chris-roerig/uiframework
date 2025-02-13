#include "ProgressBar.h"
#include "../Theme/ThemeGlobals.h"
#include "../Helpers.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>

namespace ui {

// Constructor
ProgressBar::ProgressBar(int x, int y, int width, int height, float initValue, bool showText)
    : UIElement(x, y, width, height), progress(initValue), showText(showText) {}

// Destructor
ProgressBar::~ProgressBar() {}

std::pair<int, int> ProgressBar::getTextSize(const std::string &text) const {
    if (!globalFont) return {0, 0};

    int textW, textH;
    if (TTF_SizeText(globalFont, text.c_str(), &textW, &textH) == 0) {
        return {textW, textH};
    }

    return {0, 0}; // Fallback if there's an error
}

// Render method
void ProgressBar::render(SDL_Renderer* renderer) {
    ThemeableElementColors tc = g_currentTheme->progressBarColors();

    // Border (draw FIRST to ensure it remains visible)
    SDL_Rect borderRect = { x, y, width, height };
    SDL_SetRenderDrawColor(renderer, tc.progressBarBorder.r, tc.progressBarBorder.g, tc.progressBarBorder.b, tc.progressBarBorder.a);
    SDL_RenderDrawRect(renderer, &borderRect);

    // Background rectangle (draw INSIDE the border)
    SDL_Rect bgRect = { x + 1, y + 1, width - 2, height - 2 };
    drawFilledRect(renderer, bgRect, tc.progressBarBackground);

    // Progress Fill (ensuring it stays within borders)
    int fillWidth = static_cast<int>((width - 2) * progress);  // Adjust for border
    if (fillWidth > 0) fillWidth = std::max(fillWidth, 1); // Ensure at least 1px fill when progress > 0
    
    SDL_Rect fillRect = { x + 1, y + 1, fillWidth, height - 2 };  // Adjust for border padding
    drawFilledRect(renderer, fillRect, tc.progressBarForeground);

    // Display Percentage Text (if enabled)
    if (showText && progress > 0.0f) {
        std::string progressText = std::to_string(static_cast<int>(progress * 100)) + "%";

        // Get text dimensions WITHOUT rendering
        auto [textW, textH] = getTextSize(progressText);

        // Render text centered inside the bar
        renderText(renderer, progressText, x + (width - textW) / 2, y + (height - textH) / 2, tc.progressBarText);
    }
}

// Handle events (not needed for a simple progress bar, but kept for future extensibility)
void ProgressBar::handleEvent(const SDL_Event &e) {}

// Mark as interactive (progress bars are typically not interactive)
bool ProgressBar::isInteractive() const {
    return false;
}

// Activation (for potential key-triggered updates)
void ProgressBar::activate() {}

// Set Progress Value
void ProgressBar::setProgress(float value) {
    progress = std::clamp(value, 0.0f, 1.0f);
}

// Get Progress Value
float ProgressBar::getProgress() const {
    return progress;
}

// Overloaded Increment Progress Functions
void ProgressBar::incrementProgress(float step) {
    setProgress(progress + step);
}

// Overloaded Reset Functions
void ProgressBar::reset(float initValue) {
    setProgress(initValue);
}

// Toggle percentage text display
void ProgressBar::setShowText(bool show) {
    showText = show;
}

} // namespace ui
