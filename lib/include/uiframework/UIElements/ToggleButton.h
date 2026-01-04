#pragma once
#include "Button.h"
#include <functional>

namespace ui {

class ToggleButton : public Button {
private:
    bool isToggled = false;
    bool previousToggleState = false;
    std::function<void(bool)> onToggle;
    Uint32 toggleAnimationDuration = 200;  // 200ms toggle animation
    
public:
    ToggleButton(int x_, int y_, int w_, int h_, const std::string &text_, 
                 std::function<void(bool)> toggleCallback)
      : Button(x_, y_, w_, h_, text_, nullptr), onToggle(toggleCallback) {}
    
    // Toggle state management
    void setToggled(bool toggled) { 
        if (isToggled != toggled) {
            previousToggleState = isToggled;
            isToggled = toggled;
            
            // Start toggle animation if enabled
            if (animationsEnabled) {
                startAnimation(toggleAnimationDuration);
            }
        }
    }
    bool getToggled() const { return isToggled; }
    void setToggleCallback(std::function<void(bool)> callback) { onToggle = callback; }
    
    // Animation control
    void setToggleAnimationDuration(Uint32 duration) { toggleAnimationDuration = duration; }
    
    // Override activate to toggle state
    void activate() override;

protected:
    // Override rendering to show toggle state
    void renderImpl(const RenderContext& ctx) override;
};

} // namespace ui
