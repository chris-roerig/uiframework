#include "ScrollViewContainer.h"
#include "../Theme/ThemeGlobals.h"
#include <SDL2/SDL.h>
#include <algorithm>

namespace ui {

ScrollViewContainer::ScrollViewContainer(int x_, int y_, int w_, int h_, int minWidth_, int minHeight_)
    : UIElement(x_, y_, w_, h_), scrollX(0), scrollY(0), minWidth(minWidth_), minHeight(minHeight_)
{
}

ScrollViewContainer::~ScrollViewContainer() {}

void ScrollViewContainer::addChild(std::shared_ptr<UIElement> child) {
    children.push_back(child);
}

void ScrollViewContainer::setScrollOffset(int offsetX, int offsetY) {
    scrollX = offsetX;
    scrollY = offsetY;
}

void ScrollViewContainer::scrollBy(int dx, int dy) {
    scrollX += dx;
    scrollY += dy;
}

void ScrollViewContainer::getScrollOffset(int &offsetX, int &offsetY) const {
    offsetX = scrollX;
    offsetY = scrollY;
}

void ScrollViewContainer::render(SDL_Renderer* renderer) {
    ThemeableElementColors tc = g_currentTheme->containerColors();
    Color bg = tc.containerBackground;
    SDL_SetRenderDrawColor(renderer, bg.r, bg.g, bg.b, bg.a);
    SDL_Rect bgRect = { x, y, width, height };
    SDL_RenderFillRect(renderer, &bgRect);

    // Save previous clip.
    SDL_Rect prevClip;
    SDL_RenderGetClipRect(renderer, &prevClip);
    // Set clip to the viewport.
    SDL_Rect clipRect = { x, y, width, height };
    SDL_RenderSetClipRect(renderer, &clipRect);

    // Render children offset by scroll values.
    for(auto &child : children) {
        int origX = child->x;
        int origY = child->y;
        child->x = origX - scrollX;
        child->y = origY - scrollY;
        child->render(renderer);
        child->x = origX;
        child->y = origY;
    }
    // Restore clip.
    SDL_RenderSetClipRect(renderer, &prevClip);
}

void ScrollViewContainer::handleEvent(const SDL_Event &e) {
    if(e.type == SDL_KEYDOWN) {
        switch(e.key.keysym.sym) {
            case SDLK_LEFT:  scrollBy(-10, 0); break;
            case SDLK_RIGHT: scrollBy(10, 0);  break;
            case SDLK_UP:    scrollBy(0, -10); break;
            case SDLK_DOWN:  scrollBy(0, 10);  break;
            default: break;
        }
    }
    for(auto &child : children)
        child->handleEvent(e);
}

} // namespace ui
