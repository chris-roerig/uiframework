#pragma once

#include "UIElement.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>

namespace ui {

class ProgressBar : public UIElement {
private:
    float progress = 0.0f; // Progress value (0.0 to 1.0)
    bool showText = true;  // Whether to display percentage text
    
    std::pair<int, int> getTextSize(const std::string &text, TTF_Font* font) const;

public:
    ProgressBar(int x, int y, int width, int height, float initValue = 0.0f, bool showText = true);
    virtual ~ProgressBar() = default;

    void render(SDL_Renderer* renderer, TTF_Font* font, std::shared_ptr<class Theme> theme) override;
    void handleEvent(const SDL_Event &e) override;
    bool isInteractive() const override { return false; } // Progress bars are typically not interactive
    void activate() override;

    // Progress-related methods
    void setProgress(float value);
    float getProgress() const { return progress; }
    
    void incrementProgress(float step);
    void reset(float initValue = 0.0f);

    // Setter for text visibility
    void setShowText(bool show) { showText = show; }
    bool isShowingText() const { return showText; }
    
    // Utility methods
    int getPercentage() const { return static_cast<int>(progress * 100); }
    bool isComplete() const { return progress >= 1.0f; }
};

} // namespace ui
