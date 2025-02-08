#include "SplitContainer.h"
#include "../Theme/ThemeGlobals.h"
#include <SDL2/SDL.h>

namespace ui {

SplitContainer::SplitContainer(int x_, int y_, int w_, int h_, int dividerPos_)
    : UIElement(x_, y_, w_, h_), dividerPos(dividerPos_), dragging(false),
      leftPanel(nullptr), rightPanel(nullptr)
{
}

SplitContainer::~SplitContainer() {
    // Do not delete leftPanel/rightPanel; UI core owns them.
}

void SplitContainer::setLeftPanel(UIElement* panel) {
    leftPanel = panel;
}

void SplitContainer::setRightPanel(UIElement* panel) {
    rightPanel = panel;
}

void SplitContainer::render(SDL_Renderer* renderer) {
    ThemeableElementColors tc = g_currentTheme->containerColors();
    Color bg = tc.containerBackground;
    SDL_SetRenderDrawColor(renderer, bg.r, bg.g, bg.b, bg.a);
    SDL_Rect bgRect = { x, y, width, height };
    SDL_RenderFillRect(renderer, &bgRect);

    // Render left panel.
    if (leftPanel) {
        leftPanel->x = x;
        leftPanel->y = y;
        leftPanel->width = dividerPos;
        leftPanel->height = height;
        leftPanel->render(renderer);
    }

    // Render right panel.
    if (rightPanel) {
        rightPanel->x = x + dividerPos + 5; // 5px divider width.
        rightPanel->y = y;
        rightPanel->width = width - dividerPos - 5;
        rightPanel->height = height;
        rightPanel->render(renderer);
    }

    // Draw divider.
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
    SDL_Rect dividerRect = { x + dividerPos, y, 5, height };
    SDL_RenderFillRect(renderer, &dividerRect);
}

void SplitContainer::handleEvent(const SDL_Event &e) {
    if (e.type == SDL_MOUSEBUTTONDOWN) {
        int mouseX = e.button.x;
        // Check if click is within divider area.
        if (mouseX >= x + dividerPos && mouseX <= x + dividerPos + 5) {
            dragging = true;
        }
    } else if (e.type == SDL_MOUSEBUTTONUP) {
        dragging = false;
    } else if (e.type == SDL_MOUSEMOTION && dragging) {
        int newPos = e.motion.x - x;
        if (newPos < 50) newPos = 50;
        if (newPos > width - 50) newPos = width - 50;
        dividerPos = newPos;
    }
    if (leftPanel) leftPanel->handleEvent(e);
    if (rightPanel) rightPanel->handleEvent(e);
}

} // namespace ui

