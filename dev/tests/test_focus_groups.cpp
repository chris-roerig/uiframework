#include <catch2/catch_test_macros.hpp>
#include "uiframework/UI.h"
#include <memory>

TEST_CASE("Focus Groups - Phase 4", "[focusmanager][groups]") {
    
    SECTION("Focus group creation and activation") {
        UI ui("Test", 800, 600);
        
        auto btn1 = ui.createButton("Button 1", 10, 10, [](){});
        auto btn2 = ui.createButton("Button 2", 10, 50, [](){});
        auto btn3 = ui.createButton("Button 3", 10, 90, [](){});
        auto btn4 = ui.createButton("Button 4", 10, 130, [](){});
        
        // Create focus groups
        ui.createFocusGroup("group1", {btn1->getId(), btn2->getId()});
        ui.createFocusGroup("group2", {btn3->getId(), btn4->getId()});
        
        // Test group activation
        ui.setActiveFocusGroup("group1");
        ui.setFocus(btn1->getId());
        
        // Tab should cycle within group1 only
        ui.focusNext();
        REQUIRE(ui.getFocusedElementId() == btn2->getId());
        
        ui.focusNext();
        REQUIRE(ui.getFocusedElementId() == btn1->getId()); // Wrap around
        
        // Switch to group2
        ui.setActiveFocusGroup("group2");
        ui.focusNext();
        REQUIRE(ui.getFocusedElementId() == btn3->getId());
        
        ui.focusNext();
        REQUIRE(ui.getFocusedElementId() == btn4->getId());
    }
    
    SECTION("Focus trapping") {
        UI ui("Test", 800, 600);
        
        auto mainBtn = ui.createButton("Main", 10, 10, [](){});
        auto modalBtn1 = ui.createButton("Modal 1", 200, 10, [](){});
        auto modalBtn2 = ui.createButton("Modal 2", 200, 50, [](){});
        
        ui.createFocusGroup("main", {mainBtn->getId()});
        ui.createFocusGroup("modal", {modalBtn1->getId(), modalBtn2->getId()});
        
        // Start with main focused
        ui.setFocus(mainBtn->getId());
        REQUIRE(ui.getFocusedElementId() == mainBtn->getId());
        
        // Trap focus to modal
        ui.trapFocus("modal");
        
        // Focus should move to modal group
        ui.focusNext();
        std::string focused = ui.getFocusedElementId();
        REQUIRE((focused == modalBtn1->getId() || focused == modalBtn2->getId()));
        
        // Should cycle within modal only
        ui.focusNext();
        ui.focusNext();
        focused = ui.getFocusedElementId();
        REQUIRE((focused == modalBtn1->getId() || focused == modalBtn2->getId()));
        
        // Release trap
        ui.releaseFocusTrap();
        ui.clearActiveFocusGroup();
    }
    
    SECTION("Clear active focus group") {
        UI ui("Test", 800, 600);
        
        auto btn1 = ui.createButton("Button 1", 10, 10, [](){});
        auto btn2 = ui.createButton("Button 2", 10, 50, [](){});
        auto btn3 = ui.createButton("Button 3", 10, 90, [](){});
        
        ui.createFocusGroup("limited", {btn1->getId(), btn2->getId()});
        ui.setActiveFocusGroup("limited");
        
        ui.setFocus(btn1->getId());
        ui.focusNext();
        REQUIRE(ui.getFocusedElementId() == btn2->getId());
        
        // Clear group - should allow access to all elements
        ui.clearActiveFocusGroup();
        ui.focusNext();
        std::string focused = ui.getFocusedElementId();
        REQUIRE((focused == btn1->getId() || focused == btn3->getId()));
    }
}
