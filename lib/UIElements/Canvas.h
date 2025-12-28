#pragma once
#include "UIElement.h"
#include "../../lib/Theme/Color.h"
#include <vector>
#include <string>
#include <functional>

namespace ui {

class Canvas : public UIElement {
public:
    std::vector<std::function<void(SDL_Renderer*)>> drawCommands;
    
    Canvas(int x_, int y_, int w_, int h_) : UIElement(x_, y_, w_, h_) {}
    
    void render(SDL_Renderer* renderer, TTF_Font* font, std::shared_ptr<class Theme> theme) override;
    
    // Drawing methods
    void rect(const SDL_Rect &rect, const Color &color);
    void filledRect(const SDL_Rect &rect, const Color &color);
    void line(int x1, int y1, int x2, int y2, const Color &color);
    void point(int x, int y, const Color &color);
    void clear();
    
    // Convenience methods with relative coordinates
    void rectRel(int relX, int relY, int w, int h, const Color &color);
    void filledRectRel(int relX, int relY, int w, int h, const Color &color);
    void lineRel(int x1, int y1, int x2, int y2, const Color &color);
    void pointRel(int relX, int relY, const Color &color);
};

} // namespace ui
