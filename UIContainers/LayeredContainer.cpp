#include "LayeredContainer.h"
#include "../Theme/ThemeGlobals.h"
#include <SDL2/SDL.h>
#include <algorithm>

namespace ui {

LayeredContainer::LayeredContainer(int x_, int y_, int w_, int h_)
    : UIElement(x_, y_, w_, h_)
{
}

LayeredContainer::~LayeredContainer() {}

void LayeredContainer::addChild(std::shared_ptr<UIElement> child, int zIndex) {
    children.push_back({ zIndex, child });
}

void LayeredContainer::render(SDL_Renderer* renderer) {
    ThemeableElementColors tc = g_currentTheme->containerColors();
    Color bg = tc.containerBackground;
    SDL_SetRenderDrawColor(renderer, bg.r, bg.g, bg.b, bg.a);
    SDL_Rect bgRect = { x, y, width, height };
    SDL_RenderFillRect(renderer, &bgRect);

    // Sort children by zIndex.
    std::sort(children.begin(), children.end(), [](const LayeredChild &a, const LayeredChild &b) {
        return a.zIndex < b.zIndex;
    });

    // Render in order.
    for(auto &child : children)
        child.element->render(renderer);
}

void LayeredContainer::handleEvent(const SDL_Event &e) {
    for(auto &child : children)
        child.element->handleEvent(e);
}

} // namespace ui
