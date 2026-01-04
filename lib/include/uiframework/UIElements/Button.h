#pragma once
#include "InteractiveElement.h"
#include <vector>
#include <string>
#include <functional>

namespace ui {

enum class ButtonState {
    Normal,
    Hover, 
    Pressed,
    Disabled,
    Loading
};

class Button : public InteractiveElement {
private:
    std::string text;
    std::function<void()> onClick;
    
protected:
    // Real-time double buffering (protected for ToggleButton access)
    std::string textBuffers[2];
    bool enabledBuffers[2];
    std::atomic<int> currentTextBuffer{0};
    std::atomic<int> currentEnabledBuffer{0};
    
    ButtonState currentState = ButtonState::Normal;
    ButtonState previousState = ButtonState::Normal;
    
    // Animation support
    bool animationsEnabled = true;
    Uint32 hoverAnimationDuration = 150;  // 150ms hover animation
    Uint32 pressAnimationDuration = 100;  // 100ms press animation
    
    // Icon support
    std::string iconPath;
    SDL_Texture* iconTexture = nullptr;
    int iconWidth = 0;
    int iconHeight = 0;
    
public:
    Button(int x_, int y_, int w_, int h_, const std::string &text_, std::function<void()> callback)
      : InteractiveElement(x_, y_, w_, h_), text(text_), onClick(callback) {}
    
    ~Button() override {
        if (iconTexture) {
            SDL_DestroyTexture(iconTexture);
        }
    }

protected:
    void renderImpl(const RenderContext& ctx) override;
    void onMouseEnter() override;
    void onMouseLeave() override;
    void onMouseDown(int x, int y) override;
    void onMouseUp(int x, int y) override;
    
    // Protected getters for derived classes
    const std::string& getTextInternal() const { return text; }
    
public:
    void activate() override;
    
    // Sizing API implementation
    std::pair<int, int> getPreferredSize(TTF_Font* font) const override;
    std::pair<int, int> getMinimumSize() const override;
    void autoSize(TTF_Font* font) override;
    
    // Button-specific methods
    void setText(const std::string& newText) { 
        if (text != newText) {
            text = newText; 
            invalidateTextCache();
        }
    }
    const std::string& getText() const { return text; }
    void setCallback(std::function<void()> callback) { onClick = callback; }
    
    // State management
    ButtonState getState() const { return currentState; }
    void setState(ButtonState state) { currentState = state; }
    
    // Animation control
    void setAnimationsEnabled(bool enabled) { animationsEnabled = enabled; }
    bool getAnimationsEnabled() const { return animationsEnabled; }
    void setHoverAnimationDuration(Uint32 duration) { hoverAnimationDuration = duration; }
    void setPressAnimationDuration(Uint32 duration) { pressAnimationDuration = duration; }
    
    // Icon support
    void setIcon(const std::string& path);
    void clearIcon();
    bool hasIcon() const { return iconTexture != nullptr; }
    const std::string& getIconPath() const { return iconPath; }
    
    // Real-time safe methods (lock-free, audio thread safe)
    void realtimeSetText(const std::string& newText);
    void realtimeSetEnabled(bool enabled);
};

} // namespace ui
