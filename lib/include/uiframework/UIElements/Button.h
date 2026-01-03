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
    ButtonState currentState = ButtonState::Normal;
    
public:
    Button(int x_, int y_, int w_, int h_, const std::string &text_, std::function<void()> callback)
      : InteractiveElement(x_, y_, w_, h_), text(text_), onClick(callback) {}

protected:
    void renderImpl(const RenderContext& ctx) override;
    void onMouseEnter() override;
    void onMouseLeave() override;
    void onMouseDown(int x, int y) override;
    void onMouseUp(int x, int y) override;
    
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
};

} // namespace ui
