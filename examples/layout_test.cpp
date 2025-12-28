#include "../src/UI.h"
#include <iostream>

int main() {
    try {
        UI ui("Layout Test", 600, 400);
        
        std::cout << "Creating buttons first..." << std::endl;
        auto btn1 = ui.createButton("Button 1", 10, 10, []() { std::cout << "Button 1" << std::endl; });
        auto btn2 = ui.createButton("Button 2", 10, 50, []() { std::cout << "Button 2" << std::endl; });
        
        std::cout << "Button 1 dimensions: " << btn1->width << "x" << btn1->height << " at (" << btn1->x << "," << btn1->y << ")" << std::endl;
        std::cout << "Button 2 dimensions: " << btn2->width << "x" << btn2->height << " at (" << btn2->x << "," << btn2->y << ")" << std::endl;
        
        std::cout << "Creating VBox layout..." << std::endl;
        auto vbox = ui.createVBoxLayout(50, 50, 200, 300, 10);
        
        std::cout << "VBox created successfully" << std::endl;
        
        std::cout << "Adding buttons to layout..." << std::endl;
        vbox->addElement(btn1);
        std::cout << "Button 1 added" << std::endl;
        vbox->addElement(btn2);
        std::cout << "Button 2 added" << std::endl;
        
        std::cout << "After layout - Button 1: " << btn1->width << "x" << btn1->height << " at (" << btn1->x << "," << btn1->y << ")" << std::endl;
        std::cout << "After layout - Button 2: " << btn2->width << "x" << btn2->height << " at (" << btn2->x << "," << btn2->y << ")" << std::endl;
        
        std::cout << "Layout setup complete - starting UI..." << std::endl;
        ui.run();
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    
    return 0;
}
