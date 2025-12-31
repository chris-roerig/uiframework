#include "uiframework/UI.h"
#include "uiframework/Resources/FontManager.h"
#include "uiframework/Resources/EmbeddedFontRegistry.h"
#include <iostream>

int main() {
    try {
        UI ui("Phase 0 Font Demo", 800, 600);
        
        // Test embedded font registry
        auto families = ui::EmbeddedFontRegistry::getAvailableFamilies();
        std::cout << "Available embedded fonts: ";
        for (const auto& family : families) {
            std::cout << family << " ";
        }
        std::cout << std::endl;
        
        // Create labels with different fonts
        auto robotoLabel = ui.createLabel("Roboto Font (Primary UI)", 50, 50);
        auto consoleLabel = ui.createLabel("Console Font (Monospace)", 50, 100);
        
        // Test theme-based font selection
        auto& fontManager = ui::FontManager::getInstance();
        
        // Get theme fonts
        auto primaryFont = fontManager.getThemeFont(ui::FontType::Primary, 14);
        auto monospaceFont = fontManager.getThemeFont(ui::FontType::Monospace, 12);
        
        if (primaryFont) {
            std::cout << "Primary theme font loaded successfully" << std::endl;
        }
        if (monospaceFont) {
            std::cout << "Monospace theme font loaded successfully" << std::endl;
        }
        
        // Create buttons to test different font families
        auto robotoButton = ui.createButton("Roboto Button", 50, 200, [](){
            std::cout << "Roboto button clicked!" << std::endl;
        });
        
        auto consoleButton = ui.createButton("Console Button", 50, 250, [](){
            std::cout << "Console button clicked!" << std::endl;
        });
        
        // Add tooltips to demonstrate font usage
        robotoLabel->setTooltip("This uses the Roboto embedded font");
        consoleLabel->setTooltip("This uses the Console embedded font");
        robotoButton->setTooltip("Button with Roboto font");
        consoleButton->setTooltip("Button with Console font");
        
        std::cout << "Phase 0 Font Demo started. Available fonts: " << families.size() << std::endl;
        std::cout << "Press ESC to exit" << std::endl;
        
        ui.run();
        
    } catch (const ui::UIException& e) {
        std::cerr << "UI Error: " << e.what() << std::endl;
        return -1;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    
    return 0;
}
