#include "FlowContainer.h"
#include "../Theme/ThemeGlobals.h"
#include <SDL2/SDL.h>

namespace ui {

FlowContainer::FlowContainer(int x_, int y_, int w_, int h_)
    : UIElement(x_, y_, w_, h_), horizontalSpacing(5), verticalSpacing(5)
{
}

FlowContainer::~FlowContainer() {}

void FlowContainer::addChild(std::shared_ptr<UIElement> child) {
    children.push_back(child);
}

void FlowContainer::setSpacing(int horizontal, int vertical) {
    horizontalSpacing = horizontal;
    verticalSpacing = vertical;
}

void FlowContainer::render(SDL_Renderer* renderer) {
    // Render the container background using a themeable color.
    ThemeableElementColors tc = g_currentTheme->containerColors();
    Color bg = tc.containerBackground;
    SDL_SetRenderDrawColor(renderer, bg.r, bg.g, bg.b, bg.a);
    SDL_Rect bgRect = { x, y, width, height };
    SDL_RenderFillRect(renderer, &bgRect);

    // Layout children sequentially; wrap to next line if needed.
    int curX = x;
    int curY = y;
    int maxHeightInLine = 0;

    for(auto &child : children) {
        // If placing the child would overflow the container width, wrap to a new line.
        if(curX + child->width > x + width) {
            curX = x;
            curY += maxHeightInLine + verticalSpacing;
            maxHeightInLine = 0;
        }
        // Position the child relative to the container.
        child->x = curX;
        child->y = curY;
        child->render(renderer);
        curX += child->width + horizontalSpacing;
        if(child->height > maxHeightInLine)
            maxHeightInLine = child->height;
    }
}

void FlowContainer::handleEvent(const SDL_Event &e) {
    for(auto &child : children)
        child->handleEvent(e);
}

} // namespace ui
