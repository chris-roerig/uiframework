#include "uiframework/UI.h"
#include <iostream>

int main() {
    try {
        UI ui("Layout Examples", 900, 600);
        ui.setTheme("SolarizedLight");

        // Example 1: VBox Layout (Left third)
        auto vboxPanel = ui.createVBoxLayout(10, 10, 280, 580);
        auto vboxTitle = ui.createLabel("VBox Layout", 0, 0);
        auto vboxBtn1 = ui.createButton("Button 1", 0, 0, []() { std::cout << "VBox Button 1" << std::endl; });
        auto vboxBtn2 = ui.createButton("Button 2", 0, 0, []() { std::cout << "VBox Button 2" << std::endl; });
        auto vboxBtn3 = ui.createButton("Button 3", 0, 0, []() { std::cout << "VBox Button 3" << std::endl; });
        auto vboxLabel = ui.createLabel("Stacked Vertically", 0, 0);
        
        vboxPanel->addElement(vboxTitle);
        vboxPanel->addElement(vboxBtn1);
        vboxPanel->addElement(vboxBtn2);
        vboxPanel->addElement(vboxBtn3);
        vboxPanel->addElement(vboxLabel);

        // Example 2: Grid Layout (Middle third)
        auto gridPanel = ui.createGridLayout(310, 10, 280, 580, 3, 2);
        auto gridTitle = ui.createLabel("Grid Layout", 0, 0);
        auto gridBtn1 = ui.createButton("A1", 0, 0, []() { std::cout << "Grid A1" << std::endl; });
        auto gridBtn2 = ui.createButton("A2", 0, 0, []() { std::cout << "Grid A2" << std::endl; });
        auto gridBtn3 = ui.createButton("B1", 0, 0, []() { std::cout << "Grid B1" << std::endl; });
        auto gridBtn4 = ui.createButton("B2", 0, 0, []() { std::cout << "Grid B2" << std::endl; });
        auto gridLabel = ui.createLabel("2D Grid", 0, 0);
        
        gridPanel->addElement(gridTitle, 0, 0, 1, 2);  // Span 2 columns
        gridPanel->addElement(gridBtn1, 1, 0);
        gridPanel->addElement(gridBtn2, 1, 1);
        gridPanel->addElement(gridBtn3, 2, 0);
        gridPanel->addElement(gridBtn4, 2, 1);
        gridPanel->addElement(gridLabel, 0, 0);  // Auto-position

        // Example 3: Combined Layout (Right third)
        auto combinedPanel = ui.createVBoxLayout(610, 10, 280, 580);
        auto combinedTitle = ui.createLabel("Combined Layout", 0, 0);
        
        // Top section - HBox for horizontal buttons
        auto topHBox = ui.createHBoxLayout(0, 0, 260, 60);
        auto hBtn1 = ui.createButton("H1", 0, 0, []() { std::cout << "Horizontal 1" << std::endl; });
        auto hBtn2 = ui.createButton("H2", 0, 0, []() { std::cout << "Horizontal 2" << std::endl; });
        topHBox->addElement(hBtn1);
        topHBox->addElement(hBtn2);
        
        // Middle section - 2x2 Grid
        auto miniGrid = ui.createGridLayout(0, 0, 260, 120, 2, 2);
        auto g1 = ui.createButton("G1", 0, 0, []() { std::cout << "Grid 1" << std::endl; });
        auto g2 = ui.createButton("G2", 0, 0, []() { std::cout << "Grid 2" << std::endl; });
        auto g3 = ui.createButton("G3", 0, 0, []() { std::cout << "Grid 3" << std::endl; });
        auto g4 = ui.createButton("G4", 0, 0, []() { std::cout << "Grid 4" << std::endl; });
        miniGrid->addElement(g1, 0, 0);
        miniGrid->addElement(g2, 0, 1);
        miniGrid->addElement(g3, 1, 0);
        miniGrid->addElement(g4, 1, 1);
        
        // Bottom section - More vertical buttons
        auto bottomBtn1 = ui.createButton("Bottom 1", 0, 0, []() { std::cout << "Bottom 1" << std::endl; });
        auto bottomBtn2 = ui.createButton("Bottom 2", 0, 0, []() { std::cout << "Bottom 2" << std::endl; });
        auto combinedLabel = ui.createLabel("VBox + HBox + Grid", 0, 0);
        
        combinedPanel->addElement(combinedTitle);
        combinedPanel->addElement(topHBox);
        combinedPanel->addElement(miniGrid);
        combinedPanel->addElement(bottomBtn1);
        combinedPanel->addElement(bottomBtn2);
        combinedPanel->addElement(combinedLabel);

        std::cout << "Layout Examples Started:" << std::endl;
        std::cout << "Left: VBox Layout - vertical stacking" << std::endl;
        std::cout << "Middle: Grid Layout - 2D positioning" << std::endl;
        std::cout << "Right: Combined - nested layouts" << std::endl;
        
        ui.run();
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    
    return 0;
}
