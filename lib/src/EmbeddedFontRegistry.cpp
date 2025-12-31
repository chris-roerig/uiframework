#include "uiframework/Resources/EmbeddedFontRegistry.h"
#include "uiframework/Resources/FontManager.h"
#include "uiframework/Resources/EmbeddedFonts/Console.h"
#include "uiframework/Resources/EmbeddedFonts/RobotoRegular.h"
#include "uiframework/Logger.h"
#include <algorithm>

namespace ui {

// Global initialization of embedded fonts
namespace {
    struct EmbeddedFontInitializer {
        EmbeddedFontInitializer() {
            EmbeddedFontRegistry::registerAllFonts();
        }
    };
    static EmbeddedFontInitializer fontInit;
}

void EmbeddedFontRegistry::registerAllFonts() {
    static bool registered = false;
    if (registered) return;
    
    auto& fontManager = FontManager::getInstance();
    
    // Register Console font (monospace)
    fontManager.registerEmbeddedFont("Console", FontStyle::Regular, 
        embedded::Console_data, embedded::Console_size);
    
    // Register Roboto font (primary UI font)
    fontManager.registerEmbeddedFont("Roboto", FontStyle::Regular,
        embedded::RobotoRegular_data, embedded::RobotoRegular_size);
    
    Logger::log(LogLevel::INFO, "Embedded fonts registered: Console, Roboto");
    registered = true;
}

std::vector<std::string> EmbeddedFontRegistry::getAvailableFamilies() {
    return {"Console", "Roboto"};
}

bool EmbeddedFontRegistry::isFamilyAvailable(const std::string& familyName) {
    auto families = getAvailableFamilies();
    return std::find(families.begin(), families.end(), familyName) != families.end();
}

} // namespace ui
