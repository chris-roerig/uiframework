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
    std::pair<int, int> getTextSize(const std::string &text) const;

public:
    ProgressBar(int x, int y, int width, int height, float initValue = 0.0f, bool showText = true);
    virtual ~ProgressBar();

    void render(SDL_Renderer* renderer) override;
    void handleEvent(const SDL_Event &e) override;
    bool isInteractive() const override;
    void activate() override;

    // Progress-related methods
    void setProgress(float value);
    float getProgress() const;
    
    void incrementProgress(float step);
    void reset(float initValue = 0.0f);

    // Setter for text visibility
    void setShowText(bool show);
};

} // namespace ui
