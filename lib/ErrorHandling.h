#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <memory>

namespace ui {

// Forward declaration
class Theme;

/**
 * Standardized error handling utilities for UI elements
 */
namespace ErrorHandling {
    
    /**
     * Validates common render parameters
     * @param renderer SDL renderer (required)
     * @param theme Theme object (required)
     * @param font Font object (optional, can be null)
     * @return true if basic requirements are met for rendering
     */
    inline bool validateRenderParams(SDL_Renderer* renderer, std::shared_ptr<Theme> theme, TTF_Font* font = nullptr) {
        return renderer != nullptr && theme != nullptr;
    }
    
    /**
     * Validates text rendering parameters
     * @param renderer SDL renderer (required)
     * @param theme Theme object (required)
     * @param font Font object (required for text)
     * @param text Text content (required, non-empty)
     * @return true if all requirements are met for text rendering
     */
    inline bool validateTextRenderParams(SDL_Renderer* renderer, std::shared_ptr<Theme> theme, TTF_Font* font, const std::string& text) {
        return renderer != nullptr && theme != nullptr && font != nullptr && !text.empty();
    }
    
    /**
     * Validates SDL surface creation result
     * @param surface SDL surface pointer
     * @return true if surface is valid
     */
    inline bool validateSurface(SDL_Surface* surface) {
        return surface != nullptr;
    }
    
    /**
     * Validates SDL texture creation result
     * @param texture SDL texture pointer
     * @return true if texture is valid
     */
    inline bool validateTexture(SDL_Texture* texture) {
        return texture != nullptr;
    }
}

} // namespace ui
