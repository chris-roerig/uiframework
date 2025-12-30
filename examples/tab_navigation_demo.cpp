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
        
        // Row 5: CycleList and TabbedPanel with child elements
        std::vector<std::string> priorities = {"Low", "Medium", "High", "Critical"};
        auto cycleList = ui.createCycleList(50, 250, 140, 30, priorities);
        
        auto tabbedPanel = ui.createTabbedPanel(200, 250, 300, 150);
        int tab1 = tabbedPanel->addTab("Personal");
        int tab2 = tabbedPanel->addTab("Contact");  
        int tab3 = tabbedPanel->addTab("Settings");
        
        // Add elements to each tab
        // Tab 1: Personal info
        auto nameField = ui.createTextBox("Full Name", 210, 280);
        nameField->setSize(120, 25);
        auto ageSlider = ui.createHSlider(210, 310, 120, 20, 18.0f, 100.0f, 25.0f);
        auto marriedCheck = ui.createCheckBox(false, 210, 340, [](bool checked) {
            std::cout << "Married: " << (checked ? "Yes" : "No") << std::endl;
        });
        tabbedPanel->addChildToTab(tab1, nameField);
        tabbedPanel->addChildToTab(tab1, ageSlider);
        tabbedPanel->addChildToTab(tab1, marriedCheck);
        
        // Tab 2: Contact info
        auto emailField = ui.createTextBox("Email Address", 210, 280);
        emailField->setSize(120, 25);
        auto phoneField = ui.createTextBox("Phone Number", 210, 310);
        phoneField->setSize(120, 25);
        auto notifyCheck = ui.createCheckBox(true, 210, 340, [](bool checked) {
            std::cout << "Notifications: " << (checked ? "On" : "Off") << std::endl;
        });
        tabbedPanel->addChildToTab(tab2, emailField);
        tabbedPanel->addChildToTab(tab2, phoneField);
        tabbedPanel->addChildToTab(tab2, notifyCheck);
        
        // Tab 3: Settings
        auto volumeSlider = ui.createHSlider(210, 280, 120, 20, 0.0f, 100.0f, 75.0f);
        auto themeList = ui.createCycleList(210, 310, 120, 25, {"Light", "Dark", "Auto"});
        auto autoSaveCheck = ui.createCheckBox(true, 210, 340, [](bool checked) {
            std::cout << "Auto-save: " << (checked ? "On" : "Off") << std::endl;
        });
        tabbedPanel->addChildToTab(tab3, volumeSlider);
        tabbedPanel->addChildToTab(tab3, themeList);
        tabbedPanel->addChildToTab(tab3, autoSaveCheck);
        
        // Labels
        ui.createLabel("Newsletter", 70, 150);
        ui.createLabel("Marketing", 220, 150);
        ui.createLabel("Accept Terms", 370, 150);
        ui.createLabel("Priority:", 50, 230);
        ui.createLabel("Volume:", 200, 180);
        ui.createLabel("Brightness:", 350, 180);
        
        // Tab content labels (these will be visible/hidden with tabs)
        auto marriedLabel = ui.createLabel("Married", 230, 340);
        auto notifyLabel = ui.createLabel("Notifications", 230, 340);
        auto autoSaveLabel = ui.createLabel("Auto-save", 230, 340);
        tabbedPanel->addChildToTab(tab1, marriedLabel);
        tabbedPanel->addChildToTab(tab2, notifyLabel);
        tabbedPanel->addChildToTab(tab3, autoSaveLabel);
        
        // Instructions
        ui.createLabel("=== TAB NAVIGATION DEMO ===", 50, 420);
        ui.createLabel("Press TAB to cycle forward through elements", 50, 440);
        ui.createLabel("Press SHIFT+TAB to cycle backward", 50, 460);
        ui.createLabel("Click any element to focus it directly", 50, 480);
        ui.createLabel("Switch tabs to see hidden elements become focusable", 50, 500);
        ui.createLabel("Yellow border indicates focused element", 50, 520);
        
        std::cout << "=== Tab Navigation Demo ===" << std::endl;
        std::cout << "Press TAB to cycle forward through elements" << std::endl;
        std::cout << "Press SHIFT+TAB to cycle backward" << std::endl;
        std::cout << "Click any element to focus it directly" << std::endl;
        std::cout << "Switch tabs to see hidden elements become focusable" << std::endl;
        std::cout << "Yellow border indicates focused element" << std::endl;
        std::cout << "=========================" << std::endl;
        
        ui.run();
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    
    return 0;
}
