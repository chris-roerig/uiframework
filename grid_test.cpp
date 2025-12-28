#include "../src/UI.h"
#include <iostream>

int main() {
    try {
        UI ui("4x4 Grid Test", 600, 500);
        
        // Create a 4x4 grid layout
        auto grid = ui.createGridLayout(50, 50, 500, 400, 4, 4, 10);
        
        std::cout << "Grid container position: (" << grid->x << "," << grid->y << ") size: " << grid->width << "x" << grid->height << std::endl;
        
        // Add 16 labels to fill the 4x4 grid
        std::vector<std::shared_ptr<ui::Label>> labels;
        for (int i = 0; i < 16; i++) {
            std::string labelText = "Cell " + std::to_string(i + 1);
            auto label = ui.createLabel(labelText, 0, 0);
            labels.push_back(label);
            grid->addElement(label);
        }
        
        // Check positions after all labels are added
        std::cout << "\nLabel positions after grid layout:" << std::endl;
        for (int i = 0; i < 16; i++) {
            auto& label = labels[i];
            int expectedRow = i / 4;
            int expectedCol = i % 4;
            std::cout << "Cell " << (i+1) << " (expected row " << expectedRow << ", col " << expectedCol << "): ";
            std::cout << "(" << label->x << "," << label->y << ") size " << label->width << "x" << label->height << std::endl;
        }
        
        std::cout << "\nIf grid is working correctly, you should see:" << std::endl;
        std::cout << "- Row 0: Cells 1-4 at same Y, different X values" << std::endl;
        std::cout << "- Row 1: Cells 5-8 at same Y, different X values" << std::endl;
        std::cout << "- Row 2: Cells 9-12 at same Y, different X values" << std::endl;
        std::cout << "- Row 3: Cells 13-16 at same Y, different X values" << std::endl;
        
        ui.run();
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    
    return 0;
}
