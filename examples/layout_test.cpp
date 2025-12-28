#include "../src/UI.h"
#include <iostream>

int main() {
    try {
        UI ui("Layout Test", 600, 400);
        
        // Test layout creation
        std::cout << "Creating VBox layout..." << std::endl;
        auto vbox = ui.createVBoxLayout(50, 50, 200, 300, 10);
        
        std::cout << "Creating buttons..." << std::endl;
        auto btn1 = ui.createButton("Button 1", 0, 0, []() { std::cout << "Button 1" << std::endl; });
        auto btn2 = ui.createButton("Button 2", 0, 0, []() { std::cout << "Button 2" << std::endl; });
        
        std::cout << "Adding buttons to layout..." << std::endl;
        vbox->addElement(btn1);
        vbox->addElement(btn2);
        
        std::cout << "Starting UI..." << std::endl;
        ui.run();
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    
    return 0;
}
