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
        auto mainGrid = ui.createGridLayout(20, 80, 760, 500, 4, 2);  // 4 rows, 2 columns
        mainGrid->setBorderWidth(2);  // 2px borders for debugging
        
        // Left Column Content
        auto leftTitle = ui.createLabel("Left Column", 0, 0);
        auto leftBtn1 = ui.createButton("Left Button 1", 0, 0, []() {
            std::cout << "Left Button 1 clicked" << std::endl;
        });
        auto leftBtn2 = ui.createButton("Left Button 2", 0, 0, []() {
            std::cout << "Left Button 2 clicked" << std::endl;
        });
        auto leftInfo = ui.createLabel("Left column info", 0, 0);
        
        // Right Column Content
        auto rightTitle = ui.createLabel("Right Column", 0, 0);
        auto rightBtn1 = ui.createButton("Right Button 1", 0, 0, []() {
            std::cout << "Right Button 1 clicked" << std::endl;
        });
        auto rightBtn2 = ui.createButton("Right Button 2", 0, 0, []() {
            std::cout << "Right Button 2 clicked" << std::endl;
        });
        auto rightInfo = ui.createLabel("Right column info", 0, 0);
        
        // Add elements to grid (row, column)
        mainGrid->addElement(leftTitle, 0, 0);    // Row 0, Left column
        mainGrid->addElement(rightTitle, 0, 1);   // Row 0, Right column
        mainGrid->addElement(leftBtn1, 1, 0);     // Row 1, Left column
        mainGrid->addElement(rightBtn1, 1, 1);    // Row 1, Right column
        mainGrid->addElement(leftBtn2, 2, 0);     // Row 2, Left column
        mainGrid->addElement(rightBtn2, 2, 1);    // Row 2, Right column
        mainGrid->addElement(leftInfo, 3, 0);     // Row 3, Left column
        mainGrid->addElement(rightInfo, 3, 1);    // Row 3, Right column

        std::cout << "Context Menu + Grid Demo Started" << std::endl;
        std::cout << "Menu bar at top, 2-column grid layout below" << std::endl;
        
        ui.run();
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    
    return 0;
}
