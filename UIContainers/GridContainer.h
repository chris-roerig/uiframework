#pragma once

#include "../Helpers.h"
#include "../UIElements/UIElement.h"
#include <SDL2/SDL.h>
#include <vector>
#include <memory>

namespace ui {

class GridContainer : public UIElement {
public:
    // Constructs a GridContainer at (x,y) with width w and height h, arranging children in the specified number of columns.
    GridContainer(int x_, int y_, int w_, int h_, int columns);
    virtual ~GridContainer();

    // Add a child element.
    void addChild(UIElement* child);

    // Set horizontal and vertical spacing between grid cells.
    void setSpacing(int horizontal, int vertical);

    // Render children in a grid layout.
    virtual void render(SDL_Renderer* renderer) override;

    virtual void handleEvent(const SDL_Event &e) override;

private:
    std::vector<std::shared_ptr<UIElement>> children;
    int columns;
    int horizontalSpacing;
    int verticalSpacing;
};

} // namespace ui
