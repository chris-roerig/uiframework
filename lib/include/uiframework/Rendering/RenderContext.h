#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <memory>

namespace ui {

// Forward declaration
class Theme;

/**
 * @brief Unified rendering context for all UI elements
 * 
 * Eliminates boilerplate code by providing validated rendering parameters
 * in a single structure passed to all render methods.
 */
struct RenderContext {
    SDL_Renderer* renderer;
    TTF_Font* font;
    std::shared_ptr<Theme> theme;
    
    /**
     * @brief Check if all rendering parameters are valid
     * @return true if renderer, font, and theme are all non-null
     */
    bool isValid() const {
        return renderer != nullptr && font != nullptr && theme != nullptr;
    }
    
    /**
     * @brief Create a validated RenderContext
     * @param r SDL renderer
     * @param f TTF font
     * @param t Theme instance
     * @return RenderContext with validated parameters
     */
    static RenderContext create(SDL_Renderer* r, TTF_Font* f, std::shared_ptr<Theme> t) {
        RenderContext ctx;
        ctx.renderer = r;
        ctx.font = f;
        ctx.theme = t;
        return ctx;
    }
};

} // namespace ui
