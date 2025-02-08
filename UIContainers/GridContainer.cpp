#include "GridContainer.h"
#include "../Theme/ThemeGlobals.h"
#include <SDL2/SDL.h>
#include <cmath>

namespace ui {

GridContainer::GridContainer(int x_, int y_, int w_, int h_, int columns_)
    : UIElement(x_, y_, w_, h_), columns(columns_), horizontalSpacing(5), verticalSpacing(5)
{
}

GridContainer::~GridContainer() {}

void GridContainer::addChild(std::shared_ptr<UIElement> child) {
    children.push_back(child);
}

void GridContainer::setSpacing(int horizontal, int vertical) {
    horizontalSpacing = horizontal;
    verticalSpacing = vertical;
}

void GridContainer::render(SDL_Renderer* renderer) {
    ThemeableElementColors tc = g_currentTheme->containerColors();
    Color bg = tc.containerBackground;
    SDL_SetRenderDrawColor(renderer, bg.r, bg.g, bg.b, bg.a);
    SDL_Rect bgRect = { x, y, width, height };
    SDL_RenderFillRect(renderer, &bgRect);

    int numRows = std::ceil(static_cast<float>(children.size()) / columns);
    // Calculate cell width and height (cells are evenly distributed).
    int cellWidth = (width - (columns - 1) * horizontalSpacing) / columns;
    int cellHeight = (height - (numRows - 1) * verticalSpacing) / numRows;

    for (size_t i = 0; i < children.size(); ++i) {
        int row = i / columns;
        int col = i % columns;
        int cellX = x + col * (cellWidth + horizontalSpacing);
        int cellY = y + row * (cellHeight + verticalSpacing);
        // Center the child within the cell.
        auto child = children[i];
        child->x = cellX + (cellWidth - child->width) / 2;
        child->y = cellY + (cellHeight - child->height) / 2;
        child->render(renderer);
    }
}

void GridContainer::handleEvent(const SDL_Event &e) {
    for(auto &child : children)
        child->handleEvent(e);
}

} // namespace ui
