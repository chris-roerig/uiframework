#pragma once

#include <string>
#include <vector>
#include <map>
#include <memory>

namespace ui {

/**
 * Font configuration structure for JSON-based font management
 */
struct FontConfigData {
    struct EmbeddedFonts {
        bool enabled = true;
        std::vector<std::string> fonts;
    } embeddedFonts;
    
    struct ThemeConfig {
        std::string primaryFont;
        std::string monospaceFont;
        std::string uiFont;
        int baseSize = 12;
    };
    std::map<std::string, ThemeConfig> themes;
    
    struct FallbackFonts {
        std::vector<std::string> system;
    } fallbackFonts;
    
    struct CacheConfig {
        size_t maxFonts = 50;
        size_t maxTexturesPerElement = 10;
    } cache;
};

/**
 * Font configuration loader and manager
 * Provides JSON-based font configuration with runtime font registration
 */
class FontConfig {
public:
    /**
     * Load font configuration from JSON file
     * @param configPath Path to JSON configuration file
     * @return true if loaded successfully, false otherwise
     */
    static bool loadFromFile(const std::string& configPath);
    
    /**
     * Load font configuration from JSON string
     * @param jsonContent JSON configuration content
     * @return true if parsed successfully, false otherwise
     */
    static bool loadFromJSON(const std::string& jsonContent);
    
    /**
     * Get default font family from current configuration
     * @return Default font family name
     */
    static std::string getDefaultFamily();
    
    /**
     * Get default font size from current configuration
     * @return Default font size in pixels
     */
    static int getDefaultSize();
    
    /**
     * Get theme configuration by name
     * @param themeName Name of the theme
     * @return Theme configuration, or default if not found
     */
    static FontConfigData::ThemeConfig getThemeConfig(const std::string& themeName);
    
    /**
     * Get list of available theme names
     * @return Vector of theme names
     */
    static std::vector<std::string> getAvailableThemes();
    
    /**
     * Get current configuration data
     * @return Current font configuration
     */
    static const FontConfigData& getCurrentConfig();
    
    /**
     * Apply configuration to FontManager
     * Updates cache settings and registers configured fonts
     * @return true if applied successfully, false otherwise
     */
    static bool applyConfiguration();
    
    /**
     * Reset to default configuration
     */
    static void resetToDefaults();
    
    /**
     * Check if configuration has been loaded
     * @return true if configuration is loaded, false if using defaults
     */
    static bool isConfigurationLoaded();

private:
    static FontConfigData currentConfig;
    static bool configLoaded;
    
    static FontConfigData createDefaultConfig();
    static bool parseJSON(const std::string& jsonContent, FontConfigData& config);
};

} // namespace ui
