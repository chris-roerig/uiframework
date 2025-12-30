#include "uiframework/UI.h"
#include <iostream>

int main() {
    try {
        UI ui("Working Focus Demo", 700, 500);
        
        // Create elements that DO auto-focus on click
        auto textbox1 = ui.createTextBox("Click me - I auto-focus", 50, 50);
        textbox1->setSize(250, 30);
        
        auto textbox2 = ui.createTextBox("Click me too", 50, 100);
        textbox2->setSize(250, 30);
        
        auto checkbox1 = ui.createCheckBox(false, 50, 150, [](bool checked) {
            std::cout << "Checkbox 1: " << (checked ? "checked" : "unchecked") << std::endl;
        });
        
        auto checkbox2 = ui.createCheckBox(false, 50, 180, [](bool checked) {
            std::cout << "Checkbox 2: " << (checked ? "checked" : "unchecked") << std::endl;
        });
        
        auto slider1 = ui.createHSlider(50, 220, 200, 20, 0.0f, 100.0f, 25.0f);
        auto slider2 = ui.createHSlider(50, 250, 200, 20, 0.0f, 100.0f, 75.0f);
        
        std::vector<std::string> cycleItems = {"Option A", "Option B", "Option C"};
        auto cycleList = ui.createCycleList(50, 290, 200, 30, cycleItems);
        
        auto tabbedPanel = ui.createTabbedPanel(350, 50, 300, 200);
        tabbedPanel->addTab("Tab 1");
        tabbedPanel->addTab("Tab 2");
        tabbedPanel->addTab("Tab 3");
        
        // Add labels
        ui.createLabel("Click any element to see focus change (yellow border)", 50, 350);
        ui.createLabel("TextBoxes: Type to see input", 50, 370);
        ui.createLabel("Checkboxes: Click to toggle", 50, 390);
        ui.createLabel("Sliders: Drag to change value", 50, 410);
        ui.createLabel("CycleList: Use UP/DOWN arrows when focused", 50, 430);
        ui.createLabel("TabbedPanel: Click tabs to switch", 50, 450);
        
        // Add checkbox labels
        ui.createLabel("Check 1", 70, 150);
        ui.createLabel("Check 2", 70, 180);
        
        std::cout << "=== Working Focus Demo ===" << std::endl;
        std::cout << "All elements shown auto-focus on mouse click" << std::endl;
        std::cout << "Look for YELLOW focus borders around clicked elements" << std::endl;
        std::cout << "Try typing in textboxes, using arrow keys on CycleList" << std::endl;
        std::cout << "=========================" << std::endl;
        
        // Set initial focus to first textbox
        ui.setFocus(textbox1);
        std::cout << "Initial focus set to first textbox (should see yellow border)" << std::endl;
        
        ui.run();
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    
    return 0;
}
