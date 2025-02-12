#pragma once

#include "../Helpers.h"
#include "../UIElements/UIElement.h"
#include <SDL2/SDL.h>
#include <vector>
#include <memory>

namespace ui {

class FlowContainer : public UIElement {
public:
    // Constructs a FlowContainer at (x,y) with width w and height h.
    FlowContainer(int x_, int y_, int w_, int h_);
    virtual ~FlowContainer();

    // Add a child element to the container.
    void addChild(UIElement* child);

    // Set spacing between elements.
    void setSpacing(int horizontal, int vertical);

    // Render the container and its children using a flow layout.
    virtual void render(SDL_Renderer* renderer) override;

    const std::vector<UIElement*>& getChildren() const;

    // Handle events (if needed, e.g. for mouse interactions).
    virtual void handleEvent(const SDL_Event &e) override;

private:
    std::vector<UIElement*> children;
    int horizontalSpacing;
    int verticalSpacing;
};

} // namespace ui
