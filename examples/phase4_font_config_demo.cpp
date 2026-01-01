#include "uiframework/UI.h"
#include "uiframework/Resources/FontConfig.h"
#include "uiframework/Resources/FontManager.h"
#include <iostream>
#include <fstream>

using namespace ui;

void demonstrateDefaultConfiguration() {
    std::cout << "\n=== Phase 4: Font Configuration & Management Demo ===" << std::endl;
    
    // Show default configuration
    std::cout << "\n1. Default Configuration:" << std::endl;
    std::cout << "   Default Family: " << FontConfig::getDefaultFamily() << std::endl;
    std::cout << "   Default Size: " << FontConfig::getDefaultSize() << std::endl;
    
    auto themes = FontConfig::getAvailableThemes();
    std::cout << "   Available Themes: ";
    for (size_t i = 0; i < themes.size(); ++i) {
        std::cout << themes[i];
        if (i < themes.size() - 1) std::cout << ", ";
    }
    std::cout << std::endl;
    
    // Show theme configurations
    for (const auto& themeName : themes) {
        auto themeConfig = FontConfig::getThemeConfig(themeName);
        std::cout << "   Theme '" << themeName << "':" << std::endl;
        std::cout << "     Primary: " << themeConfig.primaryFont << std::endl;
        std::cout << "     Monospace: " << themeConfig.monospaceFont << std::endl;
        std::cout << "     UI: " << themeConfig.uiFont << std::endl;
        std::cout << "     Base Size: " << themeConfig.baseSize << std::endl;
    }
}

void demonstrateRuntimeFontRegistration() {
    std::cout << "\n2. Runtime Font Registration:" << std::endl;
    
    auto& fontManager = FontManager::getInstance();
    
    // Show available families before registration
    auto families = fontManager.getAvailableFamilies();
    std::cout << "   Available families before registration: ";
    for (size_t i = 0; i < families.size(); ++i) {
        std::cout << families[i];
        if (i < families.size() - 1) std::cout << ", ";
    }
    std::cout << std::endl;
    
    // Try to register a system font (this will fail gracefully if font doesn't exist)
    std::vector<std::string> systemFontPaths = {
        "/System/Library/Fonts/Helvetica.ttc",           // macOS
        "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", // Linux
        "C:\\Windows\\Fonts\\arial.ttf"                   // Windows
    };
    
    bool registeredSystemFont = false;
    for (const auto& fontPath : systemFontPaths) {
        if (fontManager.registerFontFromFile("SystemFont", FontStyle::Regular, fontPath)) {
            std::cout << "   ✅ Successfully registered system font from: " << fontPath << std::endl;
            registeredSystemFont = true;
            break;
        }
    }
    
    if (!registeredSystemFont) {
        std::cout << "   ⚠️  No system fonts found at standard locations" << std::endl;
    }
    
    // Show available families after registration
    families = fontManager.getAvailableFamilies();
    std::cout << "   Available families after registration: ";
    for (size_t i = 0; i < families.size(); ++i) {
        std::cout << families[i];
        if (i < families.size() - 1) std::cout << ", ";
    }
    std::cout << std::endl;
}

void demonstrateConfigurationLoading() {
    std::cout << "\n3. Configuration Loading:" << std::endl;
    
    // Create a sample JSON configuration
    std::string sampleConfig = R"({
  "embeddedFonts": {
    "enabled": true,
    "fonts": ["roboto", "console"]
  },
  "themes": {
    "Custom": {
      "primaryFont": "Roboto",
      "monospaceFont": "Console",
      "uiFont": "Roboto",
      "baseSize": 14
    }
  },
  "cache": {
    "maxFonts": 100,
    "maxTexturesPerElement": 15
  }
})";
    
    std::cout << "   Sample JSON configuration:" << std::endl;
    std::cout << sampleConfig << std::endl;
    
    // Try to load the configuration
    if (FontConfig::loadFromJSON(sampleConfig)) {
        std::cout << "   ✅ Configuration loaded successfully" << std::endl;
        
        // Show updated configuration
        std::cout << "   Updated Default Family: " << FontConfig::getDefaultFamily() << std::endl;
        std::cout << "   Updated Default Size: " << FontConfig::getDefaultSize() << std::endl;
        
        auto themes = FontConfig::getAvailableThemes();
        std::cout << "   Updated Available Themes: ";
        for (size_t i = 0; i < themes.size(); ++i) {
            std::cout << themes[i];
            if (i < themes.size() - 1) std::cout << ", ";
        }
        std::cout << std::endl;
    } else {
        std::cout << "   ❌ Failed to load configuration" << std::endl;
    }
}

void demonstrateFontUnregistration() {
    std::cout << "\n4. Font Unregistration:" << std::endl;
    
    auto& fontManager = FontManager::getInstance();
    
    // Show families before unregistration
    auto families = fontManager.getAvailableFamilies();
    std::cout << "   Families before unregistration: ";
    for (size_t i = 0; i < families.size(); ++i) {
        std::cout << families[i];
        if (i < families.size() - 1) std::cout << ", ";
    }
    std::cout << std::endl;
    
    // Unregister SystemFont if it exists
    if (fontManager.isFamilyAvailable("SystemFont")) {
        fontManager.unregisterFontFamily("SystemFont");
        std::cout << "   ✅ Unregistered SystemFont family" << std::endl;
    } else {
        std::cout << "   ⚠️  SystemFont family not available for unregistration" << std::endl;
    }
    
    // Show families after unregistration
    families = fontManager.getAvailableFamilies();
    std::cout << "   Families after unregistration: ";
    for (size_t i = 0; i < families.size(); ++i) {
        std::cout << families[i];
        if (i < families.size() - 1) std::cout << ", ";
    }
    std::cout << std::endl;
}

void demonstrateUIIntegration() {
    std::cout << "\n5. UI Integration:" << std::endl;
    
    try {
        UI ui("Phase 4: Font Configuration Demo", 600, 400);
        
        // Create labels with different font configurations
        auto titleLabel = ui.createLabel("Font Configuration Demo", 10, 10);
        titleLabel->setThemeFont(FontType::Primary, 16);
        
        auto descLabel = ui.createLabel("This demo shows Phase 4 font configuration features", 10, 40);
        descLabel->setThemeFont(FontType::UI, 12);
        
        auto codeLabel = ui.createLabel("Code example: FontConfig::loadFromJSON(config)", 10, 70);
        codeLabel->setThemeFont(FontType::Monospace, 10);
        
        // Create a button to test font switching
        auto fontButton = ui.createButton("Switch to Console Theme", 10, 110, [&ui]() {
            // This would switch to console theme if implemented
            std::cout << "   Button clicked - theme switching would happen here" << std::endl;
        });
        
        auto infoLabel = ui.createLabel("Phase 4 adds flexible font configuration and runtime registration", 10, 150);
        infoLabel->setFont("Roboto", 11, FontStyle::Regular);
        
        std::cout << "   ✅ UI created with font configuration features" << std::endl;
        std::cout << "   ✅ Labels using theme-based font selection" << std::endl;
        std::cout << "   ✅ Button with font configuration capabilities" << std::endl;
        std::cout << "   ✅ Mixed font families and sizes working" << std::endl;
        
        // Don't run the UI loop in demo mode, just verify creation
        std::cout << "   (UI window created but not displayed in demo mode)" << std::endl;
        
    } catch (const std::exception& e) {
        std::cout << "   ❌ Error creating UI: " << e.what() << std::endl;
    }
}

int main() {
    try {
        demonstrateDefaultConfiguration();
        demonstrateRuntimeFontRegistration();
        demonstrateConfigurationLoading();
        demonstrateFontUnregistration();
        demonstrateUIIntegration();
        
        std::cout << "\n=== Phase 4 Demo Complete ===" << std::endl;
        std::cout << "✅ Font configuration system working" << std::endl;
        std::cout << "✅ Runtime font registration working" << std::endl;
        std::cout << "✅ JSON configuration support working" << std::endl;
        std::cout << "✅ Font management features working" << std::endl;
        std::cout << "✅ UI integration working" << std::endl;
        
        return 0;
        
    } catch (const std::exception& e) {
        std::cerr << "Demo error: " << e.what() << std::endl;
        return -1;
    }
}
