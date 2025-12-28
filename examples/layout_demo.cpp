#include "../src/UI.h"
#include <iostream>

int main() {
    try {
        UI ui("Layout Demo", 800, 600);
        
        // Test VBox Layout
        std::cout << "Creating VBox layout with buttons..." << std::endl;
        auto vbox = ui.createVBoxLayout(50, 50, 200, 300, 10);
        
        auto btn1 = ui.createButton("VBox Button 1", 0, 0, []() { std::cout << "VBox Button 1 clicked!" << std::endl; });
        auto btn2 = ui.createButton("VBox Button 2", 0, 0, []() { std::cout << "VBox Button 2 clicked!" << std::endl; });
        auto btn3 = ui.createButton("VBox Button 3", 0, 0, []() { std::cout << "VBox Button 3 clicked!" << std::endl; });
        
        vbox->addElement(btn1);
        vbox->addElement(btn2);
        vbox->addElement(btn3);
        
        // Test HBox Layout
        std::cout << "Creating HBox layout with buttons..." << std::endl;
        auto hbox = ui.createHBoxLayout(300, 50, 400, 100, 15);
        
        auto hbtn1 = ui.createButton("HBox 1", 0, 0, []() { std::cout << "HBox Button 1 clicked!" << std::endl; });
        auto hbtn2 = ui.createButton("HBox 2", 0, 0, []() { std::cout << "HBox Button 2 clicked!" << std::endl; });
        auto hbtn3 = ui.createButton("HBox 3", 0, 0, []() { std::cout << "HBox Button 3 clicked!" << std::endl; });
        
        hbox->addElement(hbtn1);
        hbox->addElement(hbtn2);
        hbox->addElement(hbtn3);
        
        // Test Grid Layout
        std::cout << "Creating Grid layout..." << std::endl;
        auto grid = ui.createGridLayout(50, 400, 300, 150, 2, 3, 5);
        
        for (int i = 0; i < 6; i++) {
            std::string label = "Grid " + std::to_string(i + 1);
            auto gridBtn = ui.createButton(label, 0, 0, [i]() { 
                std::cout << "Grid Button " << (i + 1) << " clicked!" << std::endl; 
            });
            grid->addElement(gridBtn);
        }
        
        // Add some labels to show the layout system works
        auto title = ui.createLabel("Layout System Demo - All layouts working!", 50, 10);
        auto vboxLabel = ui.createLabel("VBox Layout:", 50, 30);
        auto hboxLabel = ui.createLabel("HBox Layout:", 300, 30);
        auto gridLabel = ui.createLabel("Grid Layout (2x3):", 50, 380);
        
        std::cout << "All layouts created successfully!" << std::endl;
        std::cout << "Starting UI - click buttons to test functionality..." << std::endl;
        
        ui.run();
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    
    return 0;
}
