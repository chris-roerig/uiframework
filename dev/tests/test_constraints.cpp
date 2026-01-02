#include <catch2/catch_test_macros.hpp>
#include "uiframework/UI.h"
#include "uiframework/Constraints/GridSnap.h"

TEST_CASE("Constraint System - Basic Anchoring", "[constraints]") {
    UI ui("Constraint Test", 800, 600);
    
    SECTION("Element can anchor below another element") {
        auto label = ui.createLabel("Target", 100, 100);
        auto button = ui.createButton("Anchored", 0, 0, [](){});
        
        // Anchor button below label with 10px offset
        button->setAnchor(label, ui::AnchorType::Below, 10);
        
        // Check that button is positioned correctly
        REQUIRE(button->getX() == 100); // Same X as label
        REQUIRE(button->getY() == 100 + label->getHeight() + 10); // Below label + offset
    }
    
    SECTION("Element can anchor to the right of another element") {
        auto label = ui.createLabel("Target", 50, 50);
        auto button = ui.createButton("Right", 0, 0, [](){});
        
        // Anchor button to the right of label with 5px offset
        button->setAnchor(label, ui::AnchorType::Right, 5);
        
        // Check positioning
        REQUIRE(button->getX() == 50 + label->getWidth() + 5); // Right of label + offset
        REQUIRE(button->getY() == 50); // Same Y as label
    }
    
    SECTION("Element can be centered on another element") {
        auto panel = ui.createLabel("Panel", 100, 100);
        panel->setSize(200, 100); // Make it larger
        
        auto button = ui.createButton("Centered", 0, 0, [](){});
        
        // Center button on panel
        button->setAnchor(panel, ui::AnchorType::Center, 0);
        
        // Check centering
        int expectedX = 100 + (200 - button->getWidth()) / 2;
        int expectedY = 100 + (100 - button->getHeight()) / 2;
        
        REQUIRE(button->getX() == expectedX);
        REQUIRE(button->getY() == expectedY);
    }
    
    SECTION("Constraint system reports correct state") {
        auto label = ui.createLabel("Target", 100, 100);
        auto button = ui.createButton("Test", 0, 0, [](){});
        
        // Initially no constraints
        REQUIRE_FALSE(button->hasConstraints());
        
        // Add constraint
        button->setAnchor(label, ui::AnchorType::Below, 10);
        REQUIRE(button->hasConstraints());
        
        // Clear constraints
        button->clearConstraints();
        REQUIRE_FALSE(button->hasConstraints());
    }
}

TEST_CASE("Grid Snapping System", "[constraints][grid]") {
    SECTION("GridSnap utility functions work correctly") {
        // Test basic snapping
        REQUIRE(ui::GridSnap::snapToGrid(7, 10) == 10);  // 7 -> 10
        REQUIRE(ui::GridSnap::snapToGrid(13, 10) == 10); // 13 -> 10
        REQUIRE(ui::GridSnap::snapToGrid(15, 10) == 20); // 15 -> 20
        REQUIRE(ui::GridSnap::snapToGrid(18, 10) == 20); // 18 -> 20
        
        // Test edge cases
        REQUIRE(ui::GridSnap::snapToGrid(0, 10) == 0);   // 0 -> 0
        REQUIRE(ui::GridSnap::snapToGrid(5, 10) == 10);  // 5 -> 10 (round up)
        
        // Test invalid grid size
        REQUIRE(ui::GridSnap::snapToGrid(7, 0) == 7);    // No change with grid size 0
        REQUIRE(ui::GridSnap::snapToGrid(7, -5) == 7);   // No change with negative grid
        
        // Test position snapping
        int x = 17, y = 23;
        ui::GridSnap::snapPosition(x, y, 10);
        REQUIRE(x == 20);
        REQUIRE(y == 20);
    }
    
    SECTION("UI grid size management") {
        UI ui("Grid Test", 800, 600);
        
        // Default grid size should be 0 (disabled)
        REQUIRE(ui.getGridSize() == 0);
        
        // Set grid size
        ui.setGridSize(20);
        REQUIRE(ui.getGridSize() == 20);
        
        // Grid size should propagate to constraint manager
        auto* constraintManager = ui.getConstraintManager();
        REQUIRE(constraintManager->getGridSize() == 20);
    }
    
    SECTION("Element grid snapping") {
        UI ui("Grid Test", 800, 600);
        ui.setGridSize(10);
        
        auto button = ui.createButton("Test", 17, 23, [](){});
        
        // Initially grid snapping is disabled
        REQUIRE_FALSE(button->isGridSnappingEnabled());
        
        // Enable grid snapping
        button->setGridSnapping(true);
        REQUIRE(button->isGridSnappingEnabled());
        
        // Snap to grid
        button->snapToGrid();
        REQUIRE(button->getX() == 20); // 17 -> 20
        REQUIRE(button->getY() == 20); // 23 -> 20
    }
    
    SECTION("Constraint positioning with grid snapping") {
        UI ui("Grid Test", 800, 600);
        ui.setGridSize(10);
        
        auto label = ui.createLabel("Target", 15, 25); // Will be snapped
        auto button = ui.createButton("Anchored", 0, 0, [](){});
        
        // Anchor button below label - should snap to grid
        button->setAnchor(label, ui::AnchorType::Below, 5);
        
        // Position should be grid-snapped
        int expectedY = 25 + label->getHeight() + 5; // Calculate expected position
        int snappedY = ((expectedY + 5) / 10) * 10;  // Snap to grid
        
        REQUIRE(button->getX() == 20); // 15 snapped to 20
        REQUIRE(button->getY() == snappedY);
    }
}
