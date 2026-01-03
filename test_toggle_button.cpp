#include "uiframework/UI.h"
#include <iostream>

int main() {
    try {
        UI ui("ToggleButton Test", 400, 300);
        
        bool toggleState = false;
        
        // Create a toggle button
        auto toggleButton = ui.createToggleButton("Toggle Me", 50, 50, [&](bool state) {
            toggleState = state;
            std::cout << "Toggle button state: " << (state ? "ON" : "OFF") << std::endl;
        });
        
        // Create a regular button to test the toggle state
        auto testButton = ui.createButton("Test State", 50, 100, [&]() {
            std::cout << "Current toggle state: " << (toggleState ? "ON" : "OFF") << std::endl;
            std::cout << "ToggleButton.getToggled(): " << (toggleButton->getToggled() ? "ON" : "OFF") << std::endl;
        });
        
        // Create a button to programmatically set toggle state
        auto setButton = ui.createButton("Set ON", 50, 150, [&]() {
            toggleButton->setToggled(true);
            std::cout << "Programmatically set toggle to ON" << std::endl;
        });
        
        // Create a button to clear toggle state
        auto clearButton = ui.createButton("Set OFF", 150, 150, [&]() {
            toggleButton->setToggled(false);
            std::cout << "Programmatically set toggle to OFF" << std::endl;
        });
        
        // Add some instructions
        auto label = ui.createLabel("Click 'Toggle Me' to toggle state", 50, 10);
        auto label2 = ui.createLabel("Use other buttons to test functionality", 50, 200);
        
        std::cout << "ToggleButton test started. Click buttons to test functionality." << std::endl;
        std::cout << "Press ESC or close window to exit." << std::endl;
        
        ui.run();
        
    } catch (const ui::UIException& e) {
        std::cerr << "UI Error: " << e.what() << std::endl;
        return -1;
    }
    
    return 0;
}
