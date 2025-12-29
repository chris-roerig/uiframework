#include "uiframework/UI.h"
#include <iostream>

/**
 * @brief Font debugging test
 * 
 * Tests font loading and text rendering to identify the issue.
 */
int main() {
    try {
        std::cout << "Testing font loading..." << std::endl;
        
        UI ui("Font Test", 600, 400);
        
        std::cout << "UI created successfully" << std::endl;
        
        // Test different text elements
        auto label1 = ui.createLabel("Test Label 1", 20, 20);
        std::cout << "Label 1 created" << std::endl;
        
        auto button1 = ui.createButton("Test Button", 20, 60, []() {
            std::cout << "Button clicked - text should be visible!" << std::endl;
        });
        std::cout << "Button 1 created" << std::endl;
        
        auto label2 = ui.createLabel("Another Label", 20, 100);
        std::cout << "Label 2 created" << std::endl;
        
        auto textbox = ui.createTextBox("Type here", 20, 140);
        std::cout << "TextBox created" << std::endl;
        
        // Test with explicit text setting
        label1->setText("Updated Label Text");
        button1->setText("Updated Button");
        
        std::cout << "Text updated, starting UI..." << std::endl;
        
        ui.run();
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    
    return 0;
}
