#include "uiframework/UI.h"
#include <iostream>

int main() {
    try {
        UI ui("Tooltip Demo", 800, 600);
        
        // Create elements with tooltips
        auto button1 = ui.createButton("Hover me!", 50, 50, []() {
            std::cout << "Button 1 clicked!" << std::endl;
        });
        button1->setTooltip("This is a button tooltip");
        
        auto button2 = ui.createButton("Me too!", 200, 50, []() {
            std::cout << "Button 2 clicked!" << std::endl;
        });
        button2->setTooltip("Another helpful tooltip");
        
        auto label = ui.createLabel("Hover over the buttons to see tooltips", 50, 150);
        label->setTooltip("This label also has a tooltip!");
        
        auto textbox = ui.createTextBox("Type here...", 50, 200, 200);
        textbox->setTooltip("Enter your text in this input field");
        
        std::cout << "=== Tooltip Demo ===" << std::endl;
        std::cout << "Hover over elements to see tooltips appear after 500ms" << std::endl;
        std::cout << "Tooltips automatically position to avoid screen edges" << std::endl;
        std::cout << "===================" << std::endl;
        
        ui.run();
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    
    return 0;
}
