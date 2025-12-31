#pragma once

#include <SDL2/SDL.h>
#include "uiframework/Theme/Color.h"

namespace ui {

/**
 * @brief Utility class for rendering consistent borders across UI elements
 * 
 * Provides standardized border rendering methods to eliminate duplicated
 * border drawing code across CheckBox, TextBox, OptionSelect and other elements.
 */
class BorderRenderer {
public:
    /**
     * @brief Draw a 3D border effect with light and dark edges
     * 
     * Creates a raised/inset appearance by drawing light colors on top/left
     * and dark colors on bottom/right edges.
     * 
     * @param renderer SDL renderer to draw with
     * @param rect Rectangle to draw border around
     * @param lightColor Color for top and left edges (highlight)
     * @param darkColor Color for bottom and right edges (shadow)
     * @param thickness Border thickness in pixels (default: 1)
     */
    static void draw3DBorder(SDL_Renderer* renderer, const SDL_Rect& rect,
                           const Color& lightColor, const Color& darkColor,
                           int thickness = 1);

    /**
     * @brief Draw a flat border around a rectangle
     * 
     * Draws a simple rectangular border using SDL_RenderDrawRect.
     * 
     * @param renderer SDL renderer to draw with
     * @param rect Rectangle to draw border around
     * @param color Border color
     * @param thickness Border thickness in pixels (default: 1)
     */
    static void drawFlatBorder(SDL_Renderer* renderer, const SDL_Rect& rect,
                             const Color& color, int thickness = 1);

    /**
     * @brief Draw a focus border for keyboard navigation
     * 
     * Draws a distinctive border to indicate keyboard focus.
     * Uses a thicker line and potentially different style.
     * 
     * @param renderer SDL renderer to draw with
     * @param rect Rectangle to draw focus border around
     * @param color Focus border color
     * @param thickness Focus border thickness in pixels (default: 2)
     */
    static void drawFocusBorder(SDL_Renderer* renderer, const SDL_Rect& rect,
                              const Color& color, int thickness = 2);

private:
    /**
     * @brief Helper to draw a thick line by drawing multiple 1-pixel lines
     * 
     * @param renderer SDL renderer to draw with
     * @param x1 Start X coordinate
     * @param y1 Start Y coordinate
     * @param x2 End X coordinate
     * @param y2 End Y coordinate
     * @param thickness Line thickness in pixels
     */
    static void drawThickLine(SDL_Renderer* renderer, int x1, int y1, int x2, int y2, int thickness);
};

} // namespace ui
