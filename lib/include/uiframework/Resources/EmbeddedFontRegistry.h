#pragma once

#include <string>
#include <vector>

namespace ui {

// Forward declarations
class FontManager;
enum class FontStyle;
enum class FontType;

/**
 * Registry for embedded fonts - Phase 0 implementation
 * Automatically registers all embedded fonts with the FontManager
 */
class EmbeddedFontRegistry {
public:
    /**
     * Register all embedded fonts with the FontManager
     * Call this during initialization to make embedded fonts available
     */
    static void registerAllFonts();
    
    /**
     * Get list of available embedded font families
     */
    static std::vector<std::string> getAvailableFamilies();
    
    /**
     * Check if a font family is available
     */
    static bool isFamilyAvailable(const std::string& familyName);

private:
    EmbeddedFontRegistry() = default;
};

} // namespace ui
