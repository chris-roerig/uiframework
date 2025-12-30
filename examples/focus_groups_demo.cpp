#include "uiframework/UI.h"
#include <iostream>

int main() {
    try {
        UI ui("Focus Groups Demo", 800, 600);
        
        // Main form elements
        auto mainLabel = ui.createLabel("Main Form", 50, 50);
        auto mainButton1 = ui.createButton("Main Button 1", 50, 80, [](){
            std::cout << "Main Button 1 clicked!" << std::endl;
        });
        auto mainTextBox = ui.createTextBox("Main text", 50, 120);
        auto mainButton2 = ui.createButton("Main Button 2", 50, 160, [](){
            std::cout << "Main Button 2 clicked!" << std::endl;
        });
        
        // Modal elements
        auto modalLabel = ui.createLabel("Modal Dialog", 300, 200);
        auto modalButton1 = ui.createButton("OK", 300, 230, [](){
            std::cout << "Modal OK clicked!" << std::endl;
        });
        auto modalTextBox = ui.createTextBox("Modal input", 300, 270);
        auto modalButton2 = ui.createButton("Cancel", 300, 310, [](){
            std::cout << "Modal Cancel clicked!" << std::endl;
        });
        
        // Sidebar elements
        auto sidebarLabel = ui.createLabel("Sidebar", 500, 50);
        auto sidebarButton1 = ui.createButton("Side 1", 500, 80, [](){
            std::cout << "Sidebar Button 1 clicked!" << std::endl;
        });
        auto sidebarButton2 = ui.createButton("Side 2", 500, 120, [](){
            std::cout << "Sidebar Button 2 clicked!" << std::endl;
        });
        
        // Create focus groups
        ui.createFocusGroup("main", {
            mainButton1->getId(), 
            mainTextBox->getId(), 
            mainButton2->getId()
        });
        
        ui.createFocusGroup("modal", {
            modalButton1->getId(), 
            modalTextBox->getId(), 
            modalButton2->getId()
        });
        
        ui.createFocusGroup("sidebar", {
            sidebarButton1->getId(), 
            sidebarButton2->getId()
        });
        
        // Instructions
        auto instructions = ui.createLabel("Keys: 1=Main, 2=Modal, 3=Sidebar, 4=All, T=Trap Modal, R=Release", 50, 400);
        
        // Hotkeys for switching focus groups
        ui.assignHotKey("", "1", [&ui]() {
            ui.setActiveFocusGroup("main");
            std::cout << "Switched to Main focus group" << std::endl;
        });
        
        ui.assignHotKey("", "2", [&ui]() {
            ui.setActiveFocusGroup("modal");
            std::cout << "Switched to Modal focus group" << std::endl;
        });
        
        ui.assignHotKey("", "3", [&ui]() {
            ui.setActiveFocusGroup("sidebar");
            std::cout << "Switched to Sidebar focus group" << std::endl;
        });
        
        ui.assignHotKey("", "4", [&ui]() {
            ui.clearActiveFocusGroup();
            std::cout << "Cleared focus group - all elements accessible" << std::endl;
        });
        
        ui.assignHotKey("", "t", [&ui]() {
            ui.trapFocus("modal");
            std::cout << "Focus trapped to Modal group" << std::endl;
        });
        
        ui.assignHotKey("", "r", [&ui]() {
            ui.releaseFocusTrap();
            std::cout << "Focus trap released" << std::endl;
        });
        
        std::cout << "Focus Groups Demo Started!" << std::endl;
        std::cout << "Use Tab/Shift+Tab to navigate within active group" << std::endl;
        std::cout << "Press 1/2/3 to switch groups, 4 for all, T to trap modal, R to release" << std::endl;
        
        ui.run();
        
    } catch (const ui::UIException& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    
    return 0;
}
