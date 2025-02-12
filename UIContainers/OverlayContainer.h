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

    // Adds a child UIElement to the overlay container
    void addChild(UIElement* child);

    // Returns the list of children
    const std::vector<UIElement*>& getChildren() const;

    virtual void render(SDL_Renderer* renderer) override;
    virtual void handleEvent(const SDL_Event &e) override;

private:
    std::vector<UIElement*> children;
};

} // namespace ui
