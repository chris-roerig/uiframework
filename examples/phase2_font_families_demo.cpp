#include "uiframework/UI.h"
#include "uiframework/Resources/FontManager.h"
#include "uiframework/Resources/EmbeddedFontRegistry.h"
#include <iostream>

int main() {
    try {
        UI ui("Phase 2: Font Families & Styles Demo", 800, 600);
        
        // Get font manager instance
        auto& fontManager = ui::FontManager::getInstance();
        
        // Display available font families
        auto families = fontManager.getAvailableFamilies();
        std::cout << "Available font families: ";
        for (const auto& family : families) {
            std::cout << family << " ";
        }
        std::cout << std::endl;
        
        // Create labels with different font families and styles
        auto titleLabel = ui.createLabel("Phase 2: Font Families & Styles", 50, 50);
        titleLabel->setFont("Roboto", 18, ui::FontStyle::Regular);
        
        auto primaryLabel = ui.createLabel("Primary Font (Roboto Regular)", 50, 100);
        primaryLabel->setFont("Roboto", 14, ui::FontStyle::Regular);
        
        auto monospaceLabel = ui.createLabel("Monospace Font (Console)", 50, 150);
        monospaceLabel->setFont("Console", 14, ui::FontStyle::Regular);
        
        // Test theme-based font selection
        auto themeLabel = ui.createLabel("Theme-based Primary Font", 50, 200);
        themeLabel->setThemeFont(ui::FontType::Primary, 14);
        
        auto themeMonoLabel = ui.createLabel("Theme-based Monospace Font", 50, 250);
        themeMonoLabel->setThemeFont(ui::FontType::Monospace, 14);
        
        // Test font fallback (bold style falls back to regular)
        auto fallbackLabel = ui.createLabel("Bold Style (fallback to regular)", 50, 300);
        fallbackLabel->setFont("Roboto", 14, ui::FontStyle::Bold);
        
        // Test different sizes
        auto smallLabel = ui.createLabel("Small font (10px)", 50, 350);
        smallLabel->setFont("Roboto", 10, ui::FontStyle::Regular);
        
        auto largeLabel = ui.createLabel("Large font (20px)", 50, 400);
        largeLabel->setFont("Roboto", 20, ui::FontStyle::Regular);
        
        // Display font cache information
        auto cacheInfoLabel = ui.createLabel("", 50, 450);
        
        // Create buttons to test font switching
        auto switchToRobotoBtn = ui.createButton("Switch to Roboto", 50, 500, [&]() {
            primaryLabel->setFont("Roboto", 14, ui::FontStyle::Regular);
            std::cout << "Switched to Roboto font" << std::endl;
        });
        
        auto switchToConsoleBtn = ui.createButton("Switch to Console", 200, 500, [&]() {
            primaryLabel->setFont("Console", 14, ui::FontStyle::Regular);
            std::cout << "Switched to Console font" << std::endl;
        });
        
        auto logMemoryBtn = ui.createButton("Log Memory Usage", 350, 500, [&]() {
            fontManager.logMemoryUsage();
            auto stats = fontManager.getMemoryStats();
            cacheInfoLabel->setText("Cache: " + std::to_string(stats.currentFontCacheSize) + 
                                   "/" + std::to_string(stats.maxFontCacheSize) + " fonts");
        });
        
        // Initial cache info display
        auto stats = fontManager.getMemoryStats();
        cacheInfoLabel->setText("Cache: " + std::to_string(stats.currentFontCacheSize) + 
                               "/" + std::to_string(stats.maxFontCacheSize) + " fonts");
        
        std::cout << "Phase 2 Demo Features:" << std::endl;
        std::cout << "- Multiple embedded font families (Roboto, Console)" << std::endl;
        std::cout << "- Font style support (with fallback to regular)" << std::endl;
        std::cout << "- Theme-based font selection" << std::endl;
        std::cout << "- Font family management" << std::endl;
        std::cout << "- Cache management from Phase 1" << std::endl;
        
        ui.run();
        
    } catch (const ui::UIException& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    
    return 0;
}
