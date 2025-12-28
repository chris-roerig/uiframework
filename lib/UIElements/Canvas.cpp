#include "Canvas.h"
#include "../../lib/Theme/ThemeBase.h"
#include "../../src/Helpers.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>

namespace ui {

void Canvas::render(SDL_Renderer* renderer, TTF_Font* font, std::shared_ptr<Theme> theme) {
    if (!renderer || !theme) {
        return;
    }
    
    // Set clipping rectangle to canvas bounds
    SDL_Rect clipRect = { x, y, width, height };
    SDL_RenderSetClipRect(renderer, &clipRect);
    
    // Draw canvas background
    auto colors = theme->canvasColors();
    drawFilledRect(renderer, clipRect, colors.canvasBackground);
    
    // Execute all draw commands
    for (auto& cmd : drawCommands) {
        if (cmd) {
            try {
                cmd(renderer);
            } catch (const std::exception& e) {
                std::cerr << "Error executing canvas draw command: " << e.what() << std::endl;
            }
        }
    }
    
    // Reset clipping
    SDL_RenderSetClipRect(renderer, nullptr);
    
    // Draw border if focused
    if (hasFocus && theme) {
        auto colors = theme->focusColors();
        SDL_Rect focusRect = getFocusRect();
        SDL_SetRenderDrawColor(renderer, colors.focusBorder.r, colors.focusBorder.g, colors.focusBorder.b, colors.focusBorder.a);
        SDL_RenderDrawRect(renderer, &focusRect);
    }
}

void Canvas::rect(const SDL_Rect &r, const Color &color) {
    drawCommands.push_back([=](SDL_Renderer* renderer) {
        SDL_Rect rect = { x + r.x, y + r.y, r.w, r.h };
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        SDL_RenderDrawRect(renderer, &rect);
    });
}

void Canvas::filledRect(const SDL_Rect &r, const Color &color) {
    drawCommands.push_back([=](SDL_Renderer* renderer) {
        SDL_Rect rect = { x + r.x, y + r.y, r.w, r.h };
        drawFilledRect(renderer, rect, color);
    });
}

void Canvas::line(int x1, int y1, int x2, int y2, const Color &color) {
    drawCommands.push_back([=](SDL_Renderer* renderer) {
        drawLine(renderer, x + x1, y + y1, x + x2, y + y2, color);
    });
}

void Canvas::point(int px, int py, const Color &color) {
    drawCommands.push_back([=](SDL_Renderer* renderer) {
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        SDL_RenderDrawPoint(renderer, x + px, y + py);
    });
}

void Canvas::clear() {
    drawCommands.clear();
}

// Convenience methods with relative coordinates
void Canvas::rectRel(int relX, int relY, int w, int h, const Color &color) {
    rect({relX, relY, w, h}, color);
}

void Canvas::filledRectRel(int relX, int relY, int w, int h, const Color &color) {
    filledRect({relX, relY, w, h}, color);
}

void Canvas::lineRel(int x1, int y1, int x2, int y2, const Color &color) {
    line(x1, y1, x2, y2, color);
}

void Canvas::pointRel(int relX, int relY, const Color &color) {
    point(relX, relY, color);
}

} // namespace ui
