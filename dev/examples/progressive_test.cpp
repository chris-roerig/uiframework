#include "uiframework/UI.h"
#include <iostream>

int main() {
    try {
        UI ui("UI Framework Demo", 800, 600);
        
        // Create demo widgets
        auto label = ui.createLabel("UI Framework Demo", 10, 10);
        auto button = ui.createButton("Click Me!", 10, 50, []() {
            std::cout << "Button clicked!" << std::endl;
        });
        auto checkbox = ui.createCheckBox(false, 10, 90, [](bool state) {
            std::cout << "Checkbox: " << (state ? "checked" : "unchecked") << std::endl;
        });
        
        std::cout << "Demo started. Close window to exit." << std::endl;
        ui.run();
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    
    return 0;
}
