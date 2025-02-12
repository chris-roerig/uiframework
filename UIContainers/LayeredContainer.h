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

    // Adds a child UIElement to the layered container
    void addChild(UIElement* child);

    // Returns the list of children
    const std::vector<UIElement*>& getChildren() const;

    virtual void render(SDL_Renderer* renderer) override;
    virtual void handleEvent(const SDL_Event &e) override;

private:
    std::vector<UIElement*> children;
};

} // namespace ui
