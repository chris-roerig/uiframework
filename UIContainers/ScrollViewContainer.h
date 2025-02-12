#pragma once

#include "../Helpers.h"
#include "../UIElements/UIElement.h"
#include <SDL2/SDL.h>
#include <vector>
#include <memory>

namespace ui {

class ScrollViewContainer : public UIElement {
public:
    // Constructs a ScrollViewContainer at (x,y) with width w and height h.
    // minWidth and minHeight define the smallest allowed viewport.
    ScrollViewContainer(int x_, int y_, int w_, int h_, int minWidth_, int minHeight_);
    virtual ~ScrollViewContainer();

    // Adds a child UIElement to the scroll view container
    void addChild(UIElement* child);

    // Returns the list of children
    const std::vector<UIElement*>& getChildren() const;

    // Set the scroll offset.
    void setScrollOffset(int offsetX, int offsetY);
    void scrollBy(int dx, int dy);
    void getScrollOffset(int &offsetX, int &offsetY) const;

    // Renders the viewport and its children, clipping content outside.
    virtual void render(SDL_Renderer* renderer) override;
    virtual void handleEvent(const SDL_Event &e) override;

private:
    std::vector<UIElement*> children;
    int scrollX, scrollY;
    int minWidth, minHeight;
};

} // namespace ui
