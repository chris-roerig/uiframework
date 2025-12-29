#include "uiframework/UI.h"
#include <iostream>
#include <vector>

int main() {
    try {
        std::cout << "Creating UI..." << std::endl;
        UI ui("Theme Test", 400, 300);
        
        std::cout << "Creating theme selector..." << std::endl;
        std::vector<std::string> themes = {"Default", "SolarizedDark", "SolarizedLight", "Molokai"};
        int currentTheme = 0;
        
        auto themeSelect = ui.createOptionSelect(currentTheme, themes, 10, 10, [&](int index) {
            std::cout << "Theme selected: " << themes[index] << std::endl;
            ui.setTheme(themes[index]);
            currentTheme = index;
        });
        
        std::cout << "Starting main loop..." << std::endl;
        ui.run();
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    
    return 0;
}
