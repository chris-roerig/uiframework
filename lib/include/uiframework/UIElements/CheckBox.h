#pragma once
#include "UIElement.h"
#include <vector>
#include <string>
#include <functional>

namespace ui {

class CheckBox : public UIElement {
public:
    bool checked;
    std::function<void(bool)> onToggle;
    
    CheckBox(int x_, int y_, int size, bool initial = false, std::function<void(bool)> callback = nullptr)
      : UIElement(x_, y_, size, size), checked(initial), onToggle(callback) {}
    
    void handleEvent(const SDL_Event &e) override;
    bool isInteractive() const override { return true; }
    void activate() override;
    
protected:
    void renderImpl(const RenderContext& ctx) override;
    
public:
    // Sizing API implementation
    std::pair<int, int> getPreferredSize(TTF_Font* font) const override;
    std::pair<int, int> getMinimumSize() const override;
    bool hasFixedSize() const override { return true; }
    
    // CheckBox-specific methods
    void setChecked(bool state);
    bool isChecked() const { return checked; }
    void setCallback(std::function<void(bool)> callback) { onToggle = callback; }
};

} // namespace ui
