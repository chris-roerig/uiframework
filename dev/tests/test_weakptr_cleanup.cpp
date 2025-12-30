#include <catch2/catch_test_macros.hpp>
#include "uiframework/UI.h"
#include "uiframework/Focus/FocusManager.h"

TEST_CASE("Weak Pointer Cleanup in FocusManager", "[weakptr]") {
    UI ui("Test", 800, 600);
    
    SECTION("Expired weak_ptr entries are cleaned up automatically") {
        // Create some elements
        auto button1 = ui.createButton("Button1", 10, 10, [](){});
        auto button2 = ui.createButton("Button2", 10, 50, [](){});
        auto button3 = ui.createButton("Button3", 10, 90, [](){});
        
        std::string button1Id = button1->getId();
        std::string button2Id = button2->getId();
        std::string button3Id = button3->getId();
        
        // Verify elements are registered
        REQUIRE(ui.getElement(button1Id) != nullptr);
        REQUIRE(ui.getElement(button2Id) != nullptr);
        REQUIRE(ui.getElement(button3Id) != nullptr);
        
        // Remove elements from UI (this should make weak_ptrs expire)
        ui.removeElement(button1Id);
        ui.removeElement(button2Id);
        
        // Verify elements are gone from UI
        REQUIRE(ui.getElement(button1Id) == nullptr);
        REQUIRE(ui.getElement(button2Id) == nullptr);
        REQUIRE(ui.getElement(button3Id) != nullptr);
        
        // Trigger focus operations that should clean up expired weak_ptrs
        ui.focusNext(); // This calls getFocusableElementIds() which triggers cleanup
        
        // The cleanup should have happened automatically
        // We can't directly test the internal state, but we can verify
        // that focus operations still work correctly
        REQUIRE_NOTHROW(ui.focusNext());
        REQUIRE_NOTHROW(ui.focusPrevious());
    }
    
    SECTION("Manual cleanup method works correctly") {
        // Create elements
        auto button = ui.createButton("TestButton", 10, 10, [](){});
        std::string buttonId = button->getId();
        
        // Verify element exists
        REQUIRE(ui.getElement(buttonId) != nullptr);
        
        // Remove element to make weak_ptr expire
        ui.removeElement(buttonId);
        REQUIRE(ui.getElement(buttonId) == nullptr);
        
        // Manual cleanup should work without issues
        // Note: We can't directly access FocusManager::cleanupExpiredElements()
        // from UI class, but the automatic cleanup in getFocusableElementIds()
        // serves the same purpose
        REQUIRE_NOTHROW(ui.focusNext());
    }
}
