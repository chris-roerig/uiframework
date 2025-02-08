#pragma once

#include "../Helpers.h"
#include "../UIElements/UIElement.h"
#include <SDL2/SDL.h>
#include <vector>
#include <memory>

namespace ui {

enum class StackOrientation {
    Vertical,
    Horizontal
};

class StackContainer : public UIElement {
public:
    // Constructs a StackContainer at (x,y) with width w and height h.
    // Orientation determines whether children are arranged vertically or horizontally.
    StackContainer(int x_, int y_, int w_, int h_, StackOrientation orientation = StackOrientation::Vertical);
    virtual ~StackContainer();

    // Add a child element.
    void addChild(std::shared_ptr<UIElement> child);

    // Set spacing between elements.
    void setSpacing(int spacing);

    // Render children in a stack.
    virtual void render(SDL_Renderer* renderer) override;

    virtual void handleEvent(const SDL_Event &e) override;

private:
    std::vector<std::shared_ptr<UIElement>> children;
    StackOrientation orientation;
    int spacing;
};

} // namespace ui
