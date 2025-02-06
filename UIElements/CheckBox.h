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
    void render(SDL_Renderer* renderer) override;
    void handleEvent(const SDL_Event &e) override;
    bool isInteractive() const override { return true; }
    virtual void activate() override;
};

} // namespace ui
