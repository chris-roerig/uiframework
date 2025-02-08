#include "StackContainer.h"
#include "../Theme/ThemeGlobals.h"
#include <SDL2/SDL.h>

namespace ui {

StackContainer::StackContainer(int x_, int y_, int w_, int h_, StackOrientation orientation_)
    : UIElement(x_, y_, w_, h_), orientation(orientation_), spacing(5)
{
}

StackContainer::~StackContainer() {}

void StackContainer::addChild(std::shared_ptr<UIElement> child) {
    children.push_back(child);
}

void StackContainer::setSpacing(int sp) {
    spacing = sp;
}

void StackContainer::render(SDL_Renderer* renderer) {
    ThemeableElementColors tc = g_currentTheme->containerColors();
    Color bg = tc.containerBackground;
    SDL_SetRenderDrawColor(renderer, bg.r, bg.g, bg.b, bg.a);
    SDL_Rect bgRect = { x, y, width, height };
    SDL_RenderFillRect(renderer, &bgRect);

    int curX = x, curY = y;
    for(auto &child : children) {
        child->x = curX;
        child->y = curY;
        child->render(renderer);
        if(orientation == StackOrientation::Vertical)
            curY += child->height + spacing;
        else
            curX += child->width + spacing;
    }
}

void StackContainer::handleEvent(const SDL_Event &e) {
    for(auto &child : children)
        child->handleEvent(e);
}

} // namespace ui
