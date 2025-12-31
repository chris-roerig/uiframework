#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <memory>
#include "uiframework/Theme/Color.h"

namespace ui {

class Theme;

/**
 * @brief Utility class for rendering tooltips consistently across the framework
 * 
 * Provides standardized tooltip rendering with proper positioning, theming,
 * and delay management to avoid screen edge clipping.
 */
class TooltipRenderer {
public:
    /**
     * @brief Render a tooltip near the specified position
     * 
     * @param renderer SDL renderer to draw with
     * @param font Font to use for tooltip text
     * @param theme Theme for tooltip colors
     * @param text Tooltip text to display
     * @param mouseX Current mouse X position
     * @param mouseY Current mouse Y position
     * @param screenWidth Screen width for edge detection
     * @param screenHeight Screen height for edge detection
     */
    static void renderTooltip(SDL_Renderer* renderer, TTF_Font* font, 
                            std::shared_ptr<Theme> theme, const std::string& text,
                            int mouseX, int mouseY, int screenWidth, int screenHeight);

    /**
     * @brief Check if enough time has passed to show tooltip
     * 
     * @param hoverStartTime Time when hover started (SDL_GetTicks())
     * @param currentTime Current time (SDL_GetTicks())
     * @param delayMs Delay in milliseconds before showing tooltip (default: 500ms)
     * @return true if tooltip should be shown
     */
    static bool shouldShowTooltip(Uint32 hoverStartTime, Uint32 currentTime, Uint32 delayMs = 500);

private:
    /**
     * @brief Calculate optimal tooltip position to avoid screen edges
     * 
     * @param mouseX Mouse X position
     * @param mouseY Mouse Y position
     * @param tooltipWidth Tooltip width
     * @param tooltipHeight Tooltip height
     * @param screenWidth Screen width
     * @param screenHeight Screen height
     * @return SDL_Point with optimal tooltip position
     */
    static SDL_Point calculateTooltipPosition(int mouseX, int mouseY, 
                                            int tooltipWidth, int tooltipHeight,
                                            int screenWidth, int screenHeight);
};

} // namespace ui
