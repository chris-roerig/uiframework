#include "uiframework/UI.h"
#include <iostream>

/**
 * @brief Button color test
 */
int main() {
    try {
        UI ui("Button Color Test", 400, 300);
        
        // Test button with default colors
        auto button1 = ui.createButton("Default Button", 20, 20, []() {
            std::cout << "Default button clicked!" << std::endl;
        });
        
        // Test label (which works)
        auto label1 = ui.createLabel("This label text should be visible", 20, 70);
        
        // Test textbox (which works)
        auto textbox1 = ui.createTextBox("TextBox text visible", 20, 110);
        
        std::cout << "Button color test - check if button text is visible" << std::endl;
        
        ui.run();
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    
    return 0;
}
