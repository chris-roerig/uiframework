#include "uiframework/UI.h"
#include <iostream>

int main() {
    try {
        UI ui("TabbedPanel Demo", 600, 400);
        
        // Create tabbed panel
        auto tabbedPanel = ui.createTabbedPanel(50, 50, 500, 300);
        
        // Add tabs
        int tab1 = tabbedPanel->addTab("Settings");
        int tab2 = tabbedPanel->addTab("Controls");
        int tab3 = tabbedPanel->addTab("About");
        
        // Add content to Tab 1 (Settings)
        auto label1 = ui.createLabel("Volume Settings", 70, 100);
        auto slider1 = ui.createHSlider(70, 130, 200, 20, 0, 100, 50);
        auto checkbox1 = ui.createCheckBox(false, 70, 160, [](bool checked){
            std::cout << "Sound enabled: " << checked << std::endl;
        });
        tabbedPanel->addChildToTab(tab1, label1);
        tabbedPanel->addChildToTab(tab1, slider1);
        tabbedPanel->addChildToTab(tab1, checkbox1);
        
        // Add content to Tab 2 (Controls)
        auto label2 = ui.createLabel("Key Bindings", 70, 100);
        auto button1 = ui.createButton("Configure Keys", 70, 130, [](){
            std::cout << "Configure keys clicked!" << std::endl;
        });
        std::vector<std::string> options = {"WASD", "Arrow Keys", "Custom"};
        auto cycleList = ui.createCycleList(70, 170, 150, 30, options);
        tabbedPanel->addChildToTab(tab2, label2);
        tabbedPanel->addChildToTab(tab2, button1);
        tabbedPanel->addChildToTab(tab2, cycleList);
        
        // Add content to Tab 3 (About)
        auto label3 = ui.createLabel("UI Framework Demo", 70, 100);
        auto label4 = ui.createLabel("Version 1.0", 70, 130);
        auto label5 = ui.createLabel("Built with SDL2", 70, 160);
        tabbedPanel->addChildToTab(tab3, label3);
        tabbedPanel->addChildToTab(tab3, label4);
        tabbedPanel->addChildToTab(tab3, label5);
        
        // Set up tab change callback
        tabbedPanel->setOnTabChange([](int tabIndex) {
            std::cout << "Switched to tab: " << tabIndex << std::endl;
        });
        
        // Instructions
        ui.createLabel("Click tabs to switch between panels", 50, 20);
        
        ui.run();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    
    return 0;
}
