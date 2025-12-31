#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <memory>
#include "uiframework/Theme/ThemeBase.h"

namespace ui {

class UIElement;

/**
 * @brief Unified rendering context that eliminates boilerplate from render methods
 * 
 * Consolidates renderer, font, theme, and validation into a single context object
 * that can be passed to element render implementations.
 */
struct RenderContext {
    SDL_Renderer* renderer;
    TTF_Font* font;
    std::shared_ptr<Theme> theme;
    
    /**
     * @brief Check if the render context is valid for rendering
     * @return true if all required parameters are non-null
     */
    bool isValid() const;
    
    /**
     * @brief Create a validated render context
     * @param renderer SDL renderer
     * @param font TTF font
     * @param theme Theme instance
     * @return RenderContext instance, check isValid() before use
     */
    static RenderContext create(SDL_Renderer* renderer, TTF_Font* font, std::shared_ptr<Theme> theme);
    
    /**
     * @brief Get theme colors for buttons
     */
    ThemeableElementColors buttonColors() const;
    
    /**
     * @brief Get theme colors for checkboxes
     */
    ThemeableElementColors checkboxColors() const;
    
    /**
     * @brief Get theme colors for text inputs
     */
    ThemeableElementColors textInputColors() const;
    
    /**
     * @brief Get theme colors for option selects
     */
    ThemeableElementColors optionSelectColors() const;
    
    /**
     * @brief Get theme colors for labels
     */
    ThemeableElementColors labelColors() const;
    
    /**
     * @brief Get theme colors for progress bars
     */
    ThemeableElementColors progressBarColors() const;
    
    /**
     * @brief Get theme colors for sliders
     */
    ThemeableElementColors sliderColors() const;
    
    /**
     * @brief Get theme colors for list views
     */
    ThemeableElementColors listViewColors() const;
    
    /**
     * @brief Get theme colors for modals
     */
    ThemeableElementColors modalColors() const;
    
    /**
     * @brief Get theme colors for context menus
     */
    ThemeableElementColors contextMenuColors() const;
    
    /**
     * @brief Get theme colors for tabbed panels
     */
    ThemeableElementColors tabbedPanelColors() const;
    
    /**
     * @brief Get theme colors for cycle lists
     */
    ThemeableElementColors cycleListColors() const;
};

} // namespace ui
