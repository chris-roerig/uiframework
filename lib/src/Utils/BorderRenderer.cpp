#include "uiframework/Utils/BorderRenderer.h"

namespace ui {

void BorderRenderer::draw3DBorder(SDL_Renderer* renderer, const SDL_Rect& rect,
                                 const Color& lightColor, const Color& darkColor,
                                 int thickness) {
    if (!renderer || thickness <= 0) return;

    // Draw light edges (top and left)
    SDL_SetRenderDrawColor(renderer, lightColor.r, lightColor.g, lightColor.b, lightColor.a);
    
    // Top edge
    drawThickLine(renderer, rect.x, rect.y, rect.x + rect.w - 1, rect.y, thickness);
    
    // Left edge  
    drawThickLine(renderer, rect.x, rect.y, rect.x, rect.y + rect.h - 1, thickness);

    // Draw dark edges (bottom and right)
    SDL_SetRenderDrawColor(renderer, darkColor.r, darkColor.g, darkColor.b, darkColor.a);
    
    // Bottom edge
    drawThickLine(renderer, rect.x, rect.y + rect.h - 1, rect.x + rect.w - 1, rect.y + rect.h - 1, thickness);
    
    // Right edge
    drawThickLine(renderer, rect.x + rect.w - 1, rect.y, rect.x + rect.w - 1, rect.y + rect.h - 1, thickness);
}

void BorderRenderer::drawFlatBorder(SDL_Renderer* renderer, const SDL_Rect& rect,
                                  const Color& color, int thickness) {
    if (!renderer || thickness <= 0) return;

    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    
    if (thickness == 1) {
        // Optimized single-pixel border
        SDL_RenderDrawRect(renderer, &rect);
    } else {
        // Multi-pixel border - draw multiple rectangles
        for (int i = 0; i < thickness; ++i) {
            SDL_Rect borderRect = {
                rect.x - i,
                rect.y - i,
                rect.w + 2 * i,
                rect.h + 2 * i
            };
            SDL_RenderDrawRect(renderer, &borderRect);
        }
    }
}

void BorderRenderer::drawFocusBorder(SDL_Renderer* renderer, const SDL_Rect& rect,
                                   const Color& color, int thickness) {
    if (!renderer || thickness <= 0) return;

    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    
    // Draw focus border as multiple rectangles for thickness
    for (int i = 0; i < thickness; ++i) {
        SDL_Rect focusRect = {
            rect.x - i - 1,  // Offset outside the element
            rect.y - i - 1,
            rect.w + 2 * (i + 1),
            rect.h + 2 * (i + 1)
        };
        SDL_RenderDrawRect(renderer, &focusRect);
    }
}

void BorderRenderer::drawThickLine(SDL_Renderer* renderer, int x1, int y1, int x2, int y2, int thickness) {
    if (thickness == 1) {
        SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
        return;
    }

    // Determine if line is horizontal or vertical
    if (y1 == y2) {
        // Horizontal line - expand vertically
        for (int i = 0; i < thickness; ++i) {
            SDL_RenderDrawLine(renderer, x1, y1 + i, x2, y2 + i);
        }
    } else if (x1 == x2) {
        // Vertical line - expand horizontally
        for (int i = 0; i < thickness; ++i) {
            SDL_RenderDrawLine(renderer, x1 + i, y1, x2 + i, y2);
        }
    } else {
        // Diagonal line - just draw single line (thickness not well-defined for diagonals)
        SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
    }
}

} // namespace ui
