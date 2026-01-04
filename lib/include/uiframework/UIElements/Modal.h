#pragma once
#include "InteractiveElement.h"
#include <string>
#include <functional>
#include <vector>

namespace ui {

class Modal : public InteractiveElement {
private:
    int buttonFocusIndex = 0;
    bool dismissed = false;
    
    // Animation support
    bool animationsEnabled = true;
    uint32_t showAnimationDuration = 300; // 300ms show animation
    uint32_t hideAnimationDuration = 200; // 200ms hide animation
    bool previousDismissedState = true; // Track previous dismissed state for animations
    
public:
    std::string message;
    std::vector<std::string> buttonLabels;
    std::vector<std::function<void()>> buttonCallbacks;
    std::function<void()> onDismiss;
    
    // Constructor for general modal
    Modal(int x_, int y_, int w_, int h_, const std::string &msg, const std::string &buttonText, 
          bool hasCancel = false, std::function<void()> onCloseCallback = nullptr);
    
    SDL_Rect getFocusRect() const override;
    void activate() override;

protected:
    void onMouseDown(int x, int y) override;
    void onKeyDown(const SDL_Keycode& key) override;
    void renderImpl(const RenderContext& ctx) override;
    
public:
    // Modal management
    void show();
    void dismiss();
    bool isDismissed() const { return dismissed; }
    void setMessage(const std::string& msg) { message = msg; }
    
    // Animation control
    void setAnimationsEnabled(bool enabled) { animationsEnabled = enabled; }
    bool getAnimationsEnabled() const { return animationsEnabled; }
    void setShowAnimationDuration(uint32_t duration) { showAnimationDuration = duration; }
    uint32_t getShowAnimationDuration() const { return showAnimationDuration; }
    void setHideAnimationDuration(uint32_t duration) { hideAnimationDuration = duration; }
    uint32_t getHideAnimationDuration() const { return hideAnimationDuration; }
    
    // Button management
    void addButton(const std::string& label, std::function<void()> callback);
    void clearButtons();
    void setButtonFocus(int index);
    int getButtonFocus() const { return buttonFocusIndex; }
    
    // Utility methods
    void centerOnScreen(int screenWidth, int screenHeight);
    SDL_Rect getButtonRect(int buttonIndex) const;
    int getButtonAt(int mouseX, int mouseY) const;
};

} // namespace ui
