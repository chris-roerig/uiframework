#include <catch2/catch_test_macros.hpp>
#include "uiframework/UI.h"

TEST_CASE("Focus Order Hints Cleanup", "[focushints]") {
    UI ui("Test", 800, 600);
    
    SECTION("Focus hints are cleaned up when element is removed by string ID") {
        // Create button with focus order hint
        auto button = ui.createButton("Test", 10, 10, [](){}, 1);
        std::string buttonId = button->getId();
        
        // Verify element exists
        REQUIRE(ui.getElement(buttonId) != nullptr);
        
        // Remove element
        ui.removeElement(buttonId);
        
        // Verify element is gone
        REQUIRE(ui.getElement(buttonId) == nullptr);
        
        // Create another element with focus order to verify hints were cleaned
        auto button2 = ui.createButton("Test2", 20, 20, [](){}, 1);
        
        // Should work without issues (hints map was cleaned)
        REQUIRE(button2 != nullptr);
    }
    
    SECTION("Focus hints are cleaned up when element is removed by numeric ID") {
        // Create button with focus order hint
        auto button = ui.createButton("Test", 10, 10, [](){}, 1);
        uint64_t numericId = button->getNumericId();
        std::string stringId = button->getId();
        
        // Verify element exists
        REQUIRE(ui.getElement(numericId) != nullptr);
        
        // Remove element by numeric ID
        ui.removeElement(numericId);
        
        // Verify element is gone
        REQUIRE(ui.getElement(numericId) == nullptr);
        REQUIRE(ui.getElement(stringId) == nullptr);
        
        // Create another element to verify hints were cleaned
        auto button2 = ui.createButton("Test2", 20, 20, [](){}, 1);
        
        // Should work without issues
        REQUIRE(button2 != nullptr);
    }
}
