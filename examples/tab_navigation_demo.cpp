#include "uiframework/UI.h"
#include <iostream>

int main() {
    try {
        UI ui("Tab Navigation Demo", 800, 600);
        
        // Row 1: TextBoxes
        auto textbox1 = ui.createTextBox("First Name", 50, 50);
        textbox1->setSize(140, 30);
        auto textbox2 = ui.createTextBox("Last Name", 200, 50);
        textbox2->setSize(140, 30);
        auto textbox3 = ui.createTextBox("Email", 350, 50);
        textbox3->setSize(140, 30);
        
        // Row 2: More TextBoxes
        auto textbox4 = ui.createTextBox("Phone", 50, 100);
        textbox4->setSize(140, 30);
        auto textbox5 = ui.createTextBox("Address", 200, 100);
        textbox5->setSize(140, 30);
        auto textbox6 = ui.createTextBox("City", 350, 100);
        textbox6->setSize(140, 30);
        
        // Row 3: Checkboxes
        auto checkbox1 = ui.createCheckBox(false, 50, 150, [](bool checked) {
            std::cout << "Newsletter: " << (checked ? "Yes" : "No") << std::endl;
        });
        auto checkbox2 = ui.createCheckBox(false, 200, 150, [](bool checked) {
            std::cout << "Marketing: " << (checked ? "Yes" : "No") << std::endl;
        });
        auto checkbox3 = ui.createCheckBox(false, 350, 150, [](bool checked) {
            std::cout << "Terms: " << (checked ? "Yes" : "No") << std::endl;
        });
        
        // Row 4: Sliders
        auto slider1 = ui.createHSlider(50, 200, 140, 20, 0.0f, 100.0f, 25.0f);
        auto slider2 = ui.createHSlider(200, 200, 140, 20, 0.0f, 100.0f, 50.0f);
        auto slider3 = ui.createHSlider(350, 200, 140, 20, 0.0f, 100.0f, 75.0f);
        
        // Row 5: CycleList and TabbedPanel
        std::vector<std::string> priorities = {"Low", "Medium", "High", "Critical"};
        auto cycleList = ui.createCycleList(50, 250, 140, 30, priorities);
        
        auto tabbedPanel = ui.createTabbedPanel(200, 250, 300, 150);
        tabbedPanel->addTab("Personal");
        tabbedPanel->addTab("Contact");
        tabbedPanel->addTab("Preferences");
        
        // Labels
        ui.createLabel("Newsletter", 70, 150);
        ui.createLabel("Marketing", 220, 150);
        ui.createLabel("Accept Terms", 370, 150);
        ui.createLabel("Priority:", 50, 230);
        ui.createLabel("Volume:", 200, 180);
        ui.createLabel("Brightness:", 350, 180);
        
        // Instructions
        ui.createLabel("=== TAB NAVIGATION DEMO ===", 50, 420);
        ui.createLabel("Press TAB to cycle forward through elements", 50, 440);
        ui.createLabel("Press SHIFT+TAB to cycle backward", 50, 460);
        ui.createLabel("Click any element to focus it directly", 50, 480);
        ui.createLabel("Yellow border indicates focused element", 50, 500);
        
        std::cout << "=== Tab Navigation Demo ===" << std::endl;
        std::cout << "Press TAB to cycle forward through elements" << std::endl;
        std::cout << "Press SHIFT+TAB to cycle backward" << std::endl;
        std::cout << "Click any element to focus it directly" << std::endl;
        std::cout << "Yellow border indicates focused element" << std::endl;
        std::cout << "=========================" << std::endl;
        
        ui.run();
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    
    return 0;
}
