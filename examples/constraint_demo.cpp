#include "uiframework/UI.h"
#include <iostream>

int main() {
    try {
        UI ui("Constraint System Demo", 800, 600);
        
        // Create a title label at the top
        auto title = ui.createLabel("Constraint-Based Positioning Demo", 50, 50);
        title->setTooltip("This demonstrates the new constraint system");
        
        // Create a reference button
        auto refButton = ui.createButton("Reference Button", 100, 150, [](){
            std::cout << "Reference button clicked!" << std::endl;
        });
        refButton->setTooltip("Other elements will anchor to this button");
        
        // Anchor a button below the reference button
        auto belowButton = ui.createButton("Below", 0, 0, [](){
            std::cout << "Below button clicked!" << std::endl;
        });
        belowButton->setAnchor(refButton, ui::AnchorType::Below, 10);
        belowButton->setTooltip("Anchored 10px below the reference button");
        
        // Anchor a button to the right of the reference button
        auto rightButton = ui.createButton("Right", 0, 0, [](){
            std::cout << "Right button clicked!" << std::endl;
        });
        rightButton->setAnchor(refButton, ui::AnchorType::Right, 15);
        rightButton->setTooltip("Anchored 15px to the right of the reference button");
        
        // Create a panel and center a button on it
        auto panel = ui.createLabel("Panel Background", 400, 200);
        panel->setSize(200, 100);
        panel->setTooltip("Background panel for centering demonstration");
        
        auto centeredButton = ui.createButton("Centered", 0, 0, [](){
            std::cout << "Centered button clicked!" << std::endl;
        });
        centeredButton->setAnchor(panel, ui::AnchorType::Center, 0);
        centeredButton->setTooltip("Centered on the panel");
        
        // Create a chain of anchored elements
        auto chain1 = ui.createButton("Chain 1", 100, 350, [](){
            std::cout << "Chain 1 clicked!" << std::endl;
        });
        
        auto chain2 = ui.createButton("Chain 2", 0, 0, [](){
            std::cout << "Chain 2 clicked!" << std::endl;
        });
        chain2->setAnchor(chain1, ui::AnchorType::Right, 5);
        
        auto chain3 = ui.createButton("Chain 3", 0, 0, [](){
            std::cout << "Chain 3 clicked!" << std::endl;
        });
        chain3->setAnchor(chain2, ui::AnchorType::Right, 5);
        
        // Add instructions
        auto instructions = ui.createLabel("Click buttons to see console output. Hover for tooltips.", 50, 500);
        instructions->setTooltip("This label shows how to use the constraint system");
        
        // === GRID SNAPPING DEMONSTRATION ===
        
        // Enable grid snapping with 20px grid
        ui.setGridSize(20);
        
        auto gridTitle = ui.createLabel("Grid Snapping Demo (20px grid):", 50, 400);
        gridTitle->setTooltip("Elements below demonstrate grid snapping");
        
        // Create elements that will snap to grid
        auto gridButton1 = ui.createButton("Grid Snap", 157, 437, [](){
            std::cout << "Grid snapped button clicked!" << std::endl;
        });
        gridButton1->setGridSnapping(true);
        gridButton1->snapToGrid(); // Should snap 157,437 -> 160,440
        gridButton1->setTooltip("Position snapped from (157,437) to grid");
        
        // Create an element with constraints that will be grid-snapped
        auto gridAnchor = ui.createButton("Grid Anchor", 203, 463, [](){
            std::cout << "Grid anchor clicked!" << std::endl;
        });
        gridAnchor->setTooltip("Anchor element for grid-snapped constraint");
        
        auto gridConstrained = ui.createButton("Grid+Constraint", 0, 0, [](){
            std::cout << "Grid constrained button clicked!" << std::endl;
        });
        gridConstrained->setAnchor(gridAnchor, ui::AnchorType::Right, 7);
        gridConstrained->setTooltip("Anchored to right + grid snapped automatically");
        
        // === PERCENTAGE SIZING DEMONSTRATION ===
        
        auto percentTitle = ui.createLabel("Percentage Sizing Demo:", 400, 400);
        percentTitle->setTooltip("Elements below demonstrate percentage-based sizing");
        
        // Create a panel that takes 30% of window width, 20% of height
        auto percentPanel = ui.createLabel("30% x 20% Panel", 400, 430);
        percentPanel->setRelativeSize(0.3f, 0.2f); // 30% width, 20% height
        percentPanel->setTooltip("Panel sized to 30% width x 20% height of window");
        
        // Create a button that takes 25% width, 10% height
        auto percentButton = ui.createButton("25% x 10%", 400, 550, [](){
            std::cout << "Percentage sized button clicked!" << std::endl;
        });
        percentButton->setRelativeSize(0.25f, 0.1f);
        percentButton->setTooltip("Button sized to 25% width x 10% height of window");
        
        std::cout << "Constraint System Demo Started!" << std::endl;
        std::cout << "- Reference button at (100, 150)" << std::endl;
        std::cout << "- Below button anchored 10px below reference" << std::endl;
        std::cout << "- Right button anchored 15px to the right of reference" << std::endl;
        std::cout << "- Centered button anchored to center of panel" << std::endl;
        std::cout << "- Chain of buttons anchored to each other" << std::endl;
        std::cout << "- Grid snapping enabled with 20px grid size" << std::endl;
        std::cout << "- Grid button snapped from (157,437) to (" << gridButton1->getX() << "," << gridButton1->getY() << ")" << std::endl;
        std::cout << "- Constrained element with grid snapping at (" << gridConstrained->getX() << "," << gridConstrained->getY() << ")" << std::endl;
        std::cout << "- Percentage panel sized to " << percentPanel->getWidth() << "x" << percentPanel->getHeight() << " (30% x 20%)" << std::endl;
        std::cout << "- Percentage button sized to " << percentButton->getWidth() << "x" << percentButton->getHeight() << " (25% x 10%)" << std::endl;
        
        ui.run();
        
    } catch (const ui::UIException& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    
    return 0;
}
