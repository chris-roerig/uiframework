#pragma once
#include "Button.h"
#include <functional>

namespace ui {

class ToggleButton : public Button {
private:
    bool isToggled = false;
    std::function<void(bool)> onToggle;
    
public:
    ToggleButton(int x_, int y_, int w_, int h_, const std::string &text_, 
                 std::function<void(bool)> toggleCallback)
      : Button(x_, y_, w_, h_, text_, nullptr), onToggle(toggleCallback) {}
    
    // Toggle state management
    void setToggled(bool toggled) { 
        if (isToggled != toggled) {
            isToggled = toggled; 
        }
    }
    bool getToggled() const { return isToggled; }
    void setToggleCallback(std::function<void(bool)> callback) { onToggle = callback; }
    
    // Override activate to toggle state
    void activate() override;

protected:
    // Override rendering to show toggle state
    void renderImpl(const RenderContext& ctx) override;
};

} // namespace ui
