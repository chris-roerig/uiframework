#include "../src/UI.h"
#include <iostream>

int main() {
    try {
        UI ui("Layout Test - Multiple Widgets", 800, 600);
        
        // Test VBox with multiple buttons
        std::cout << "Testing VBox with 3 buttons..." << std::endl;
        auto vbox = ui.createVBoxLayout(50, 50, 200, 300, 10);
        
        auto btn1 = ui.createButton("Button 1", 0, 0, []() {});
        auto btn2 = ui.createButton("Button 2", 0, 0, []() {});
        auto btn3 = ui.createButton("Button 3", 0, 0, []() {});
        
        vbox->addElement(btn1);
        vbox->addElement(btn2);
        vbox->addElement(btn3);
        
        std::cout << "After VBox layout:" << std::endl;
        std::cout << "Button 1: (" << btn1->x << "," << btn1->y << ") size " << btn1->width << "x" << btn1->height << std::endl;
        std::cout << "Button 2: (" << btn2->x << "," << btn2->y << ") size " << btn2->width << "x" << btn2->height << std::endl;
        std::cout << "Button 3: (" << btn3->x << "," << btn3->y << ") size " << btn3->width << "x" << btn3->height << std::endl;
        
        // Test HBox with multiple buttons
        std::cout << "\nTesting HBox with 3 buttons..." << std::endl;
        auto hbox = ui.createHBoxLayout(300, 50, 400, 100, 10);
        
        auto hbtn1 = ui.createButton("H1", 0, 0, []() {});
        auto hbtn2 = ui.createButton("H2", 0, 0, []() {});
        auto hbtn3 = ui.createButton("H3", 0, 0, []() {});
        
        hbox->addElement(hbtn1);
        hbox->addElement(hbtn2);
        hbox->addElement(hbtn3);
        
        std::cout << "After HBox layout:" << std::endl;
        std::cout << "H Button 1: (" << hbtn1->x << "," << hbtn1->y << ") size " << hbtn1->width << "x" << hbtn1->height << std::endl;
        std::cout << "H Button 2: (" << hbtn2->x << "," << hbtn2->y << ") size " << hbtn2->width << "x" << hbtn2->height << std::endl;
        std::cout << "H Button 3: (" << hbtn3->x << "," << hbtn3->y << ") size " << hbtn3->width << "x" << hbtn3->height << std::endl;
        
        ui.run();
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    
    return 0;
}
