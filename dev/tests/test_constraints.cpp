#include <catch2/catch_test_macros.hpp>
#include "uiframework/UI.h"
#include "uiframework/Constraints/GridSnap.h"
#include "uiframework/Constraints/PercentageSize.h"

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

TEST_CASE("Percentage Sizing System", "[constraints][percentage]") {
    SECTION("PercentageSize utility functions work correctly") {
        // Test basic percentage calculations
        REQUIRE(ui::PercentageSize::calculateWidth(0.5f, 800) == 400);
        REQUIRE(ui::PercentageSize::calculateHeight(0.25f, 600) == 150);
        
        // Test edge cases
        REQUIRE(ui::PercentageSize::calculateWidth(0.0f, 800) == 0);
        REQUIRE(ui::PercentageSize::calculateWidth(1.0f, 800) == 800);
        
        // Test invalid percentages
        REQUIRE(ui::PercentageSize::calculateWidth(-0.1f, 800) == 0);
        REQUIRE(ui::PercentageSize::calculateWidth(1.1f, 800) == 0);
        
        // Test invalid parent dimensions
        REQUIRE(ui::PercentageSize::calculateWidth(0.5f, 0) == 0);
        REQUIRE(ui::PercentageSize::calculateWidth(0.5f, -100) == 0);
        
        // Test combined size calculation
        int width, height;
        ui::PercentageSize::calculateSize(0.3f, 0.4f, 1000, 800, width, height);
        REQUIRE(width == 300);
        REQUIRE(height == 320);
        
        // Test percentage validation
        REQUIRE(ui::PercentageSize::isValidPercentage(0.0f));
        REQUIRE(ui::PercentageSize::isValidPercentage(0.5f));
        REQUIRE(ui::PercentageSize::isValidPercentage(1.0f));
        REQUIRE_FALSE(ui::PercentageSize::isValidPercentage(-0.1f));
        REQUIRE_FALSE(ui::PercentageSize::isValidPercentage(1.1f));
    }
    
    SECTION("Element percentage sizing") {
        UI ui("Percentage Test", 800, 600);
        
        auto button = ui.createButton("Test", 100, 100, [](){});
        
        // Initially no relative sizing
        REQUIRE_FALSE(button->hasRelativeSize());
        
        // Set relative size to 50% width, 25% height
        button->setRelativeSize(0.5f, 0.25f);
        REQUIRE(button->hasRelativeSize());
        
        // Check calculated size (800 * 0.5 = 400, 600 * 0.25 = 150)
        REQUIRE(button->getWidth() == 400);
        REQUIRE(button->getHeight() == 150);
        
        // Clear relative sizing
        button->clearRelativeSize();
        REQUIRE_FALSE(button->hasRelativeSize());
    }
    
    SECTION("Invalid percentage values are rejected") {
        UI ui("Percentage Test", 800, 600);
        auto button = ui.createButton("Test", 100, 100, [](){});
        
        // Try invalid percentages
        button->setRelativeSize(-0.1f, 0.5f); // Negative width
        REQUIRE_FALSE(button->hasRelativeSize());
        
        button->setRelativeSize(0.5f, 1.1f); // Height > 100%
        REQUIRE_FALSE(button->hasRelativeSize());
        
        button->setRelativeSize(1.1f, -0.1f); // Both invalid
        REQUIRE_FALSE(button->hasRelativeSize());
    }
    
    SECTION("Percentage sizing with constraints") {
        UI ui("Percentage Test", 800, 600);
        
        // Create a percentage-sized panel
        auto panel = ui.createLabel("Panel", 100, 100);
        panel->setRelativeSize(0.5f, 0.3f); // 400x180
        
        // Anchor a button to the right of the panel
        auto button = ui.createButton("Anchored", 0, 0, [](){});
        button->setAnchor(panel, ui::AnchorType::Right, 10);
        
        // Button should be positioned relative to panel's calculated size
        REQUIRE(button->getX() == 100 + 400 + 10); // panel.x + panel.width + offset
        REQUIRE(button->getY() == 100); // Same Y as panel
    }
}

TEST_CASE("Integration & Optimization", "[constraints][integration]") {
    SECTION("Bulk constraint operations") {
        UI ui("Integration Test", 800, 600);
        
        // Create multiple elements
        auto target1 = ui.createLabel("Target1", 100, 100);
        auto target2 = ui.createLabel("Target2", 200, 200);
        auto elem1 = ui.createButton("Elem1", 0, 0, [](){});
        auto elem2 = ui.createButton("Elem2", 0, 0, [](){});
        
        // Bulk set anchors
        std::vector<std::string> elementIds = {elem1->getId(), elem2->getId()};
        std::vector<std::string> targetIds = {target1->getId(), target2->getId()};
        std::vector<ui::AnchorType> anchorTypes = {ui::AnchorType::Below, ui::AnchorType::Right};
        std::vector<int> offsets = {10, 15};
        
        ui.bulkSetAnchors(elementIds, targetIds, anchorTypes, offsets);
        
        // Verify anchoring worked
        REQUIRE(elem1->hasConstraints());
        REQUIRE(elem2->hasConstraints());
        REQUIRE(elem1->getX() == 100); // Same X as target1
        REQUIRE(elem1->getY() == 100 + target1->getHeight() + 10); // Below target1 + offset
        REQUIRE(elem2->getX() == 200 + target2->getWidth() + 15); // Right of target2 + offset
        REQUIRE(elem2->getY() == 200); // Same Y as target2
        
        // Bulk clear constraints
        ui.bulkClearConstraints(elementIds);
        REQUIRE_FALSE(elem1->hasConstraints());
        REQUIRE_FALSE(elem2->hasConstraints());
    }
    
    SECTION("Integrated widget creation") {
        UI ui("Integration Test", 800, 600);
        
        auto target = ui.createLabel("Target", 150, 150);
        
        // Create button with integrated anchoring
        auto anchoredButton = ui.createButtonAnchored("Anchored Button", target, 
                                                      ui::AnchorType::Below, 20, [](){});
        
        // Verify button is created and anchored correctly
        REQUIRE(anchoredButton != nullptr);
        REQUIRE(anchoredButton->hasConstraints());
        REQUIRE(anchoredButton->getX() == 150); // Same X as target
        REQUIRE(anchoredButton->getY() == 150 + target->getHeight() + 20); // Below target + offset
        
        // Create label with integrated anchoring
        auto anchoredLabel = ui.createLabelAnchored("Anchored Label", target, 
                                                    ui::AnchorType::Right, 10);
        
        // Verify label is created and anchored correctly
        REQUIRE(anchoredLabel != nullptr);
        REQUIRE(anchoredLabel->hasConstraints());
        REQUIRE(anchoredLabel->getX() == 150 + target->getWidth() + 10); // Right of target + offset
        REQUIRE(anchoredLabel->getY() == 150); // Same Y as target
    }
    
    SECTION("Bulk operations with mismatched vectors") {
        UI ui("Integration Test", 800, 600);
        
        auto elem = ui.createButton("Test", 0, 0, [](){});
        auto target = ui.createLabel("Target", 100, 100);
        
        // Test with mismatched vector sizes (should be ignored)
        std::vector<std::string> elementIds = {elem->getId()};
        std::vector<std::string> targetIds = {target->getId(), "nonexistent"};
        std::vector<ui::AnchorType> anchorTypes = {ui::AnchorType::Below};
        std::vector<int> offsets = {10};
        
        ui.bulkSetAnchors(elementIds, targetIds, anchorTypes, offsets);
        
        // Element should not have constraints due to mismatched vectors
        REQUIRE_FALSE(elem->hasConstraints());
    }
}
