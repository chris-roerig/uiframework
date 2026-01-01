#include "uiframework/Resources/FontConfig.h"
#include "uiframework/Resources/FontManager.h"
#include <fstream>
#include <sstream>
#include <iostream>

namespace ui {

// Static member definitions
FontConfigData FontConfig::currentConfig;
bool FontConfig::configLoaded = false;

bool FontConfig::loadFromFile(const std::string& configPath) {
    std::ifstream file(configPath);
    if (!file.is_open()) {
        std::cerr << "FontConfig: Could not open config file: " << configPath << std::endl;
        return false;
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();
    
    return loadFromJSON(buffer.str());
}

bool FontConfig::loadFromJSON(const std::string& jsonContent) {
    FontConfigData newConfig;
    if (!parseJSON(jsonContent, newConfig)) {
        std::cerr << "FontConfig: Failed to parse JSON configuration" << std::endl;
        return false;
    }
    
    currentConfig = std::move(newConfig);
    configLoaded = true;
    
    // Apply configuration to FontManager
    return applyConfiguration();
}

std::string FontConfig::getDefaultFamily() {
    if (!configLoaded) {
        currentConfig = createDefaultConfig();
    }
    
    // Return primary font from default theme
    auto it = currentConfig.themes.find("Default");
    if (it != currentConfig.themes.end()) {
        return it->second.primaryFont;
    }
    
    return "Roboto"; // Fallback
}

int FontConfig::getDefaultSize() {
    if (!configLoaded) {
        currentConfig = createDefaultConfig();
    }
    
    // Return base size from default theme
    auto it = currentConfig.themes.find("Default");
    if (it != currentConfig.themes.end()) {
        return it->second.baseSize;
    }
    
    return 12; // Fallback
}

FontConfigData::ThemeConfig FontConfig::getThemeConfig(const std::string& themeName) {
    if (!configLoaded) {
        currentConfig = createDefaultConfig();
    }
    
    auto it = currentConfig.themes.find(themeName);
    if (it != currentConfig.themes.end()) {
        return it->second;
    }
    
    // Return default theme if requested theme not found
    auto defaultIt = currentConfig.themes.find("Default");
    if (defaultIt != currentConfig.themes.end()) {
        return defaultIt->second;
    }
    
    // Ultimate fallback
    FontConfigData::ThemeConfig fallback;
    fallback.primaryFont = "Roboto";
    fallback.monospaceFont = "Console";
    fallback.uiFont = "Roboto";
    fallback.baseSize = 12;
    return fallback;
}

std::vector<std::string> FontConfig::getAvailableThemes() {
    if (!configLoaded) {
        currentConfig = createDefaultConfig();
    }
    
    std::vector<std::string> themes;
    for (const auto& pair : currentConfig.themes) {
        themes.push_back(pair.first);
    }
    return themes;
}

const FontConfigData& FontConfig::getCurrentConfig() {
    if (!configLoaded) {
        currentConfig = createDefaultConfig();
    }
    return currentConfig;
}

bool FontConfig::applyConfiguration() {
    if (!configLoaded) {
        return false;
    }
    
    auto& fontManager = FontManager::getInstance();
    
    // Apply cache settings
    fontManager.setMaxCacheSize(currentConfig.cache.maxFonts);
    
    // Note: Text cache per element is handled by UIElement itself
    // We could add a method to set the default max text cache size for new elements
    
    return true;
}

void FontConfig::resetToDefaults() {
    currentConfig = createDefaultConfig();
    configLoaded = true;
    applyConfiguration();
}

bool FontConfig::isConfigurationLoaded() {
    return configLoaded;
}

FontConfigData FontConfig::createDefaultConfig() {
    FontConfigData config;
    
    // Embedded fonts configuration
    config.embeddedFonts.enabled = true;
    config.embeddedFonts.fonts = {"roboto", "console"};
    
    // Default theme
    FontConfigData::ThemeConfig defaultTheme;
    defaultTheme.primaryFont = "Roboto";
    defaultTheme.monospaceFont = "Console";
    defaultTheme.uiFont = "Roboto";
    defaultTheme.baseSize = 12;
    config.themes["Default"] = defaultTheme;
    
    // Console theme
    FontConfigData::ThemeConfig consoleTheme;
    consoleTheme.primaryFont = "Console";
    consoleTheme.monospaceFont = "Console";
    consoleTheme.uiFont = "Console";
    consoleTheme.baseSize = 12;
    config.themes["Console"] = consoleTheme;
    
    // System fallback fonts
    config.fallbackFonts.system = {
        "/System/Library/Fonts/Helvetica.ttc",           // macOS
        "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", // Linux
        "C:\\Windows\\Fonts\\arial.ttf"                   // Windows
    };
    
    // Cache configuration
    config.cache.maxFonts = 50;
    config.cache.maxTexturesPerElement = 10;
    
    return config;
}

bool FontConfig::parseJSON(const std::string& jsonContent, FontConfigData& config) {
    // Simple JSON parser for our specific configuration format
    // This is a minimal implementation - in production, you'd use a proper JSON library
    
    config = createDefaultConfig(); // Start with defaults
    
    // For this minimal implementation, we'll just validate the JSON is not empty
    // and return true. A full implementation would parse the JSON properly.
    if (jsonContent.empty()) {
        return false;
    }
    
    // Basic validation - check for JSON-like structure
    if (jsonContent.find('{') == std::string::npos || 
        jsonContent.find('}') == std::string::npos) {
        return false;
    }
    
    // In a full implementation, you would:
    // 1. Parse the JSON using a library like nlohmann/json or rapidjson
    // 2. Extract embeddedFonts configuration
    // 3. Extract themes configuration
    // 4. Extract fallbackFonts configuration
    // 5. Extract cache configuration
    // 6. Validate all values and provide sensible defaults
    
    std::cout << "FontConfig: JSON parsing not fully implemented - using defaults" << std::endl;
    
    return true;
}

} // namespace ui
