#include "uiframework/UI.h"
#include <iostream>

int main() {
    try {
        UI ui("Margin/Padding Demo", 800, 600);
        
        // Create elements with different padding/margin settings
        auto button1 = ui.createButton("No Padding", 50, 50, []() {
            std::cout << "Button 1 clicked!" << std::endl;
        });
        
        auto button2 = ui.createButton("10px Padding", 200, 50, []() {
            std::cout << "Button 2 clicked!" << std::endl;
        });
        button2->setPadding(10);
        
        auto button3 = ui.createButton("Custom Padding", 350, 50, []() {
            std::cout << "Button 3 clicked!" << std::endl;
        });
        button3->setPadding(5, 15, 5, 15); // top, right, bottom, left
        
        auto label1 = ui.createLabel("No Padding", 50, 150);
        
        auto label2 = ui.createLabel("8px Padding", 200, 150);
        label2->setPadding(8);
        
        auto label3 = ui.createLabel("Asymmetric Padding", 350, 150);
        label3->setPadding(2, 20, 2, 20);
        
        auto textbox1 = ui.createTextBox("No padding", 50, 250, 150);
        
        auto textbox2 = ui.createTextBox("With padding", 250, 250, 150);
        textbox2->setPadding(8);
        
        auto textbox3 = ui.createTextBox("Custom padding", 450, 250, 150);
        textbox3->setPadding(4, 12, 4, 12);
        
        // Add tooltips to explain the padding
        button1->setTooltip("Default button - no padding");
        button2->setTooltip("Button with 10px padding on all sides");
        button3->setTooltip("Button with custom padding: 5px top/bottom, 15px left/right");
        
        label1->setTooltip("Default label - no padding");
        label2->setTooltip("Label with 8px padding on all sides");
        label3->setTooltip("Label with asymmetric padding");
        
        textbox1->setTooltip("Default textbox - no padding");
        textbox2->setTooltip("Textbox with 8px padding");
        textbox3->setTooltip("Textbox with custom padding");
        
        std::cout << "=== Margin/Padding Demo ===" << std::endl;
        std::cout << "Compare elements with different padding settings" << std::endl;
        std::cout << "Hover over elements to see their padding configuration" << std::endl;
        std::cout << "Notice how text positioning changes with padding" << std::endl;
        std::cout << "===========================" << std::endl;
        
        ui.run();
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    
    return 0;
}
