#pragma once
#include "UIElement.h"
#include <vector>
#include <string>
#include <functional>
#include <iostream>

namespace ui {

class Label : public UIElement {
private:
    std::string text;
    
    // Real-time double buffering
    std::string textBuffers[2];
    std::atomic<int> currentTextBuffer{0};
    
public:
    Label(int x_, int y_, int w_, int h_, const std::string &text_)
      : UIElement(x_, y_, w_, h_), text(text_) {}
    
    void setText(const std::string &text);
    const std::string& getText() const { return text; }
    
protected:
    void renderImpl(const RenderContext& ctx) override;
    
public:
    // Sizing API implementation
    std::pair<int, int> getPreferredSize(TTF_Font* font) const override;
    std::pair<int, int> getMinimumSize() const override;
    void autoSize(TTF_Font* font) override;
    
    // Real-time safe methods (lock-free, audio thread safe)
    void realtimeSetText(const std::string& newText);
};

} // namespace ui
