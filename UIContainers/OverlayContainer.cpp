#include "OverlayContainer.h"
#include "../Theme/ThemeGlobals.h"
#include <SDL2/SDL.h>

namespace ui {

OverlayContainer::OverlayContainer(int x_, int y_, int w_, int h_)
    : UIElement(x_, y_, w_, h_)
{
}

OverlayContainer::~OverlayContainer() {}

void OverlayContainer::addChild(UIElement* child) {
    children.push_back(child);
}

const std::vector<UIElement*>& OverlayContainer::getChildren() const {
    return children;
}

void OverlayContainer::render(SDL_Renderer* renderer) {
    // Draw semi-transparent overlay.
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 128);
    SDL_Rect overlayRect = { x, y, width, height };
    SDL_RenderFillRect(renderer, &overlayRect);

    // Render children.
    for(auto &child : children) {
        child->render(renderer);
    }
}

void OverlayContainer::handleEvent(const SDL_Event &e) {
    for(auto &child : children)
        child->handleEvent(e);
}

} // namespace ui
