#include "../src/UI.h"
#include <iostream>

int main() {
    try {
        UI ui("Context Menu + Grid Demo", 800, 600);
        ui.setTheme("SolarizedDark");

        // Create context menu (menu bar)
        std::vector<ui::TopMenuItem> menuItems = {
            {"File", {
                {"New File", []() { std::cout << "New File selected" << std::endl; }},
                {"Open File", []() { std::cout << "Open File selected" << std::endl; }},
                {"Save", []() { std::cout << "Save selected" << std::endl; }},
                {"Exit", []() { std::cout << "Exit selected" << std::endl; }}
            }},
            {"Edit", {
                {"Undo", []() { std::cout << "Undo selected" << std::endl; }},
                {"Redo", []() { std::cout << "Redo selected" << std::endl; }},
                {"Copy", []() { std::cout << "Copy selected" << std::endl; }},
                {"Paste", []() { std::cout << "Paste selected" << std::endl; }}
            }},
            {"View", {
                {"Zoom In", []() { std::cout << "Zoom In selected" << std::endl; }},
                {"Zoom Out", []() { std::cout << "Zoom Out selected" << std::endl; }}
            }}
        };
        
        auto contextMenu = ui.createContextMenu(menuItems);

        // Create 2-column grid layout below menu bar with borders
        auto mainGrid = ui.createGridLayout(20, 80, 760, 500, 3, 2);  // 3 rows, 2 columns
        mainGrid->setBorderWidth(2);  // 2px borders for debugging
        
        // Left Column Content
        auto leftTitle = ui.createLabel("Left Column", 0, 0);
        
        // Create a VBox layout inside the left column
        auto leftVBox = ui.createVBoxLayout(0, 0, 350, 200);
        auto vboxBtn1 = ui.createButton("VBox Btn 1", 0, 0, []() {
            std::cout << "VBox Button 1 clicked" << std::endl;
        });
        auto vboxBtn2 = ui.createButton("VBox Btn 2", 0, 0, []() {
            std::cout << "VBox Button 2 clicked" << std::endl;
        });
        leftVBox->addElement(vboxBtn1);
        leftVBox->addElement(vboxBtn2);
        
        auto leftInfo = ui.createLabel("Left column info", 0, 0);
        
        // Right Column Content  
        auto rightTitle = ui.createLabel("Right Column", 0, 0);
        
        // Create an HBox layout inside the right column
        auto rightHBox = ui.createHBoxLayout(0, 0, 350, 60);
        auto hboxBtn1 = ui.createButton("H1", 0, 0, []() {
            std::cout << "HBox Button 1 clicked" << std::endl;
        });
        auto hboxBtn2 = ui.createButton("H2", 0, 0, []() {
            std::cout << "HBox Button 2 clicked" << std::endl;
        });
        rightHBox->addElement(hboxBtn1);
        rightHBox->addElement(hboxBtn2);
        
        auto rightInfo = ui.createLabel("Right column info", 0, 0);
        
        // Add elements to grid (row, column)
        mainGrid->addElement(leftTitle, 0, 0);     // Row 0, Left column
        mainGrid->addElement(rightTitle, 0, 1);    // Row 0, Right column
        mainGrid->addElement(leftVBox, 1, 0);      // Row 1, Left column - VBox layout
        mainGrid->addElement(rightHBox, 1, 1);     // Row 1, Right column - HBox layout
        mainGrid->addElement(leftInfo, 2, 0);      // Row 2, Left column
        mainGrid->addElement(rightInfo, 2, 1);     // Row 2, Right column

        std::cout << "Context Menu + Grid Demo Started" << std::endl;
        std::cout << "Menu bar at top, 2-column grid layout below" << std::endl;
        std::cout << "Left column: VBox layout nested in grid cell" << std::endl;
        std::cout << "Right column: HBox layout nested in grid cell" << std::endl;
        
        ui.run();
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    
    return 0;
}
