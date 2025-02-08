#pragma once

#include "../Helpers.h"
#include "../UIElements/UIElement.h"
#include <SDL2/SDL.h>
#include <vector>
#include <memory>

namespace ui {

class OverlayContainer : public UIElement {
public:
    // Constructs an OverlayContainer that covers the entire area specified.
    OverlayContainer(int x_, int y_, int w_, int h_);
    virtual ~OverlayContainer();

    // Add a child element (these are drawn on top of a semi-transparent overlay).
    void addChild(std::shared_ptr<UIElement> child);

    virtual void render(SDL_Renderer* renderer) override;
    virtual void handleEvent(const SDL_Event &e) override;

private:
    std::vector<std::shared_ptr<UIElement>> children;
};

} // namespace ui
