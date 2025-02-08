#pragma once

#include "../Helpers.h"
#include "../UIElements/UIElement.h"
#include <SDL2/SDL.h>
#include <vector>
#include <memory>

namespace ui {

struct LayeredChild {
    int zIndex;
    std::shared_ptr<UIElement> element;
};

class LayeredContainer : public UIElement {
public:
    // Constructs a LayeredContainer at (x,y) with width w and height h.
    LayeredContainer(int x_, int y_, int w_, int h_);
    virtual ~LayeredContainer();

    // Add a child element with a specified z-index.
    void addChild(std::shared_ptr<UIElement> child, int zIndex);

    virtual void render(SDL_Renderer* renderer) override;
    virtual void handleEvent(const SDL_Event &e) override;

private:
    std::vector<LayeredChild> children;
};

} // namespace ui
