#pragma once

#include "../Helpers.h"
#include "../UIElements/UIElement.h"
#include <SDL2/SDL.h>
#include <memory>

namespace ui {

class SplitContainer : public UIElement {
public:
    // Constructs a horizontal SplitContainer dividing the area into two resizable regions.
    // 'dividerPos' is the initial x position of the divider.
    SplitContainer(int x_, int y_, int w_, int h_, int dividerPos);
    virtual ~SplitContainer();

    // Change these methods to accept a raw pointer.
    void setLeftPanel(UIElement* panel);
    void setRightPanel(UIElement* panel);

    // Render the panels and the draggable divider.
    virtual void render(SDL_Renderer* renderer) override;
    virtual void handleEvent(const SDL_Event &e) override;
    bool isInteractive() const override { return true; }

private:
    UIElement* leftPanel;  // Now stored as raw pointers (non-owning)
    UIElement* rightPanel;
    int dividerPos; // x coordinate of divider relative to SplitContainer.
    bool dragging;
};

} // namespace ui
