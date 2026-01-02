#include "uiframework/UI.h"
#include <iostream>

int main() {
    try {
        UI ui("Grid System Demo", 800, 600);
        
        // Enable 20px grid
        ui.setGridSize(20);
        
        // Create title
        auto title = ui.createLabel("Grid System Demo - 20px Grid", 20, 20);
        title->setTooltip("All elements below snap to a 20px grid");
        
        // Create elements that will snap to grid
        auto button1 = ui.createButton("Grid Button 1", 47, 73, [](){
            std::cout << "Grid Button 1 clicked!" << std::endl;
        });
        button1->setGridSnapping(true);
        button1->snapToGrid(); // 47,73 -> 40,80
        button1->setTooltip("Snapped from (47,73) to grid");
        
        auto button2 = ui.createButton("Grid Button 2", 183, 127, [](){
            std::cout << "Grid Button 2 clicked!" << std::endl;
        });
        button2->setGridSnapping(true);
        button2->snapToGrid(); // 183,127 -> 180,120
        button2->setTooltip("Snapped from (183,127) to grid");
        
        // Create anchor element
        auto anchor = ui.createLabel("Anchor Element", 100, 200);
        anchor->setTooltip("Other elements anchor to this");
        
        // Create element with constraint + grid snapping
        auto constrainedButton = ui.createButton("Constrained+Grid", 0, 0, [](){
            std::cout << "Constrained+Grid button clicked!" << std::endl;
        });
        constrainedButton->setAnchor(anchor, ui::AnchorType::Below, 15);
        constrainedButton->setTooltip("Anchored below + auto grid-snapped");
        
        // Create percentage-sized element
        auto percentPanel = ui.createLabel("25% x 15% Panel", 0, 0);
        percentPanel->setRelativeSize(0.25f, 0.15f); // 200x90
        percentPanel->setPosition(300, 300);
        percentPanel->setTooltip("25% width x 15% height of window");
        
        // Integrated creation with anchoring
        auto integratedButton = ui.createButtonAnchored("Integrated", percentPanel,
                                                        ui::AnchorType::Right, 10, [](){
            std::cout << "Integrated button clicked!" << std::endl;
        });
        integratedButton->setTooltip("Created with createButtonAnchored()");
        
        std::cout << "Grid System Demo Started!" << std::endl;
        std::cout << "Grid size: " << ui.getGridSize() << "px" << std::endl;
        std::cout << "Button 1 snapped to: (" << button1->getX() << "," << button1->getY() << ")" << std::endl;
        std::cout << "Button 2 snapped to: (" << button2->getX() << "," << button2->getY() << ")" << std::endl;
        std::cout << "Constrained button at: (" << constrainedButton->getX() << "," << constrainedButton->getY() << ")" << std::endl;
        std::cout << "Percentage panel size: " << percentPanel->getWidth() << "x" << percentPanel->getHeight() << std::endl;
        std::cout << "Integrated button at: (" << integratedButton->getX() << "," << integratedButton->getY() << ")" << std::endl;
        
        ui.run();
        
    } catch (const ui::UIException& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    
    return 0;
}
