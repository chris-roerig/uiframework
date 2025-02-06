#pragma once
#include "../UICore.h"
#include <vector>
#include <string>
#include <functional>

namespace ui {

class Canvas : public UIElement {
public:
    std::vector<std::function<void(SDL_Renderer*)>> drawCommands;
    Canvas(int x_, int y_, int w_, int h_) : UIElement(x_, y_, w_, h_) {}
    void render(SDL_Renderer* renderer) override;
    void rect(const SDL_Rect &rect, const Color &color);
    void line(int x1, int y1, int x2, int y2, const Color &color);
};

} // namespace ui
