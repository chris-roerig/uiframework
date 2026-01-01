#pragma once

#include <string>
#include <vector>
#include <utility>
#include <SDL2/SDL_ttf.h>
#include "uiframework/Resources/FontManager.h"

namespace ui {

/**
 * @brief Utility class for text operations and measurements
 * 
 * Provides reusable text processing functions to eliminate code duplication
 * across UI elements that handle text rendering and layout.
 */
class TextUtils {
public:
    /**
     * @brief Truncate text with ellipsis to fit within specified width
     * @param text Original text to truncate
     * @param font TTF font for measurement
     * @param maxWidth Maximum width in pixels
     * @return Truncated text with ellipsis, or empty string if impossible to fit
     */
    static std::string truncateWithEllipsis(const std::string& text, TTF_Font* font, int maxWidth);
    
    /**
     * @brief Get text dimensions in pixels
     * @param text Text to measure
     * @param font TTF font for measurement
     * @return Pair of (width, height) in pixels, or (0, 0) on error
     */
    static std::pair<int, int> getTextSize(const std::string& text, TTF_Font* font);
    
    /**
     * @brief Check if text fits within specified width
     * @param text Text to check
     * @param font TTF font for measurement
     * @param width Maximum width in pixels
     * @return true if text fits, false otherwise
     */
    static bool textFitsWidth(const std::string& text, TTF_Font* font, int width);
    
    /**
     * @brief Wrap text to multiple lines within specified width
     * @param text Text to wrap
     * @param font TTF font for measurement
     * @param maxWidth Maximum width per line in pixels
     * @return Vector of text lines that fit within maxWidth
     */
    static std::vector<std::string> wrapText(const std::string& text, TTF_Font* font, int maxWidth);

    // Phase 3: Font-aware text operations using FontManager
    /**
     * @brief Get text dimensions using font family and style
     * @param text Text to measure
     * @param familyName Font family name
     * @param size Font size
     * @param style Font style
     * @return Pair of (width, height) in pixels
     */
    static std::pair<int, int> getTextSizeAdvanced(const std::string& text, const std::string& familyName, 
                                                  int size, FontStyle style = FontStyle::Regular);
    
    /**
     * @brief Wrap text with font-aware measurements
     * @param text Text to wrap
     * @param familyName Font family name
     * @param size Font size
     * @param maxWidth Maximum width per line in pixels
     * @param style Font style
     * @return Vector of text lines that fit within maxWidth
     */
    static std::vector<std::string> wrapTextAdvanced(const std::string& text, const std::string& familyName,
                                                    int size, int maxWidth, FontStyle style = FontStyle::Regular);
    
    /**
     * @brief Truncate text with ellipsis using font family and style
     * @param text Original text to truncate
     * @param familyName Font family name
     * @param size Font size
     * @param maxWidth Maximum width in pixels
     * @param style Font style
     * @return Truncated text with ellipsis
     */
    static std::string truncateWithEllipsisAdvanced(const std::string& text, const std::string& familyName,
                                                   int size, int maxWidth, FontStyle style = FontStyle::Regular);

private:
    static constexpr const char* ELLIPSIS = "...";
};

} // namespace ui
