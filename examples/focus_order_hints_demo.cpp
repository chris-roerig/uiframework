#include "uiframework/UI.h"
#include <iostream>

int main() {
    try {
        UI ui("Focus Order Hints Demo", 600, 400);
        
        // Create elements with focus order hints (intentionally out of visual order)
        auto field3 = ui.createTextBox("Field 3", 50, 100, true, 3);
        field3->setSize(120, 30);
        
        auto field1 = ui.createTextBox("Field 1", 50, 50, true, 1);
        field1->setSize(120, 30);
        
        auto field4 = ui.createTextBox("Field 4", 200, 100, true, 4);
        field4->setSize(120, 30);
        
        auto field2 = ui.createTextBox("Field 2", 200, 50, true, 2);
        field2->setSize(120, 30);
        
        auto button2 = ui.createButton("Second", 50, 150, []() {
            std::cout << "Second button clicked!" << std::endl;
        }, 6);
        
        auto button1 = ui.createButton("First", 200, 150, []() {
            std::cout << "First button clicked!" << std::endl;
        }, 5);
        
        // Apply focus order based on hints
        ui.applyFocusOrderHints();
        
        // Labels
        ui.createLabel("Focus Order Hints Demo", 50, 20);
        ui.createLabel("Elements created out of order but Tab follows hints:", 50, 200);
        ui.createLabel("Expected Tab order: Field1 → Field2 → Field3 → Field4 → First → Second", 50, 220);
        ui.createLabel("Press TAB to test the focus order", 50, 250);
        
        std::cout << "=== Focus Order Hints Demo ===" << std::endl;
        std::cout << "Elements were created in this order: Field3, Field1, Field4, Field2, Second, First" << std::endl;
        std::cout << "But focus order hints make Tab navigate: Field1 → Field2 → Field3 → Field4 → First → Second" << std::endl;
        std::cout << "Press TAB to test!" << std::endl;
        std::cout << "=========================" << std::endl;
        
        ui.run();
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    
    return 0;
}
