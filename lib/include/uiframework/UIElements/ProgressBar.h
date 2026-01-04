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
    
    // Real-time double buffering
    float progressBuffers[2];
    std::atomic<int> currentProgressBuffer{0};
    
    std::pair<int, int> getTextSize(const std::string &text, TTF_Font* font) const;

public:
    ProgressBar(int x, int y, int width, int height, float initValue = 0.0f, bool showText = true);
    virtual ~ProgressBar() override = default;

protected:
    void renderImpl(const RenderContext& ctx) override;
    
public:
    // Sizing API implementation
    std::pair<int, int> getPreferredSize(TTF_Font* font) const override;
    std::pair<int, int> getMinimumSize() const override;

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
    
    // Real-time safe methods (lock-free, audio thread safe)
    void realtimeSetProgress(float value);
};

} // namespace ui
