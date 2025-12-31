#pragma once
#include "InteractiveElement.h"
#include <vector>
#include <string>
#include <functional>

namespace ui {

class Button : public InteractiveElement {
public:
    std::string text;
    std::function<void()> onClick;
    
    Button(int x_, int y_, int w_, int h_, const std::string &text_, std::function<void()> callback)
      : InteractiveElement(x_, y_, w_, h_), text(text_), onClick(callback) {}

protected:
    void renderImpl(const RenderContext& ctx) override;
    
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
};

} // namespace ui
