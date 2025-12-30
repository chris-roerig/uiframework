#include <catch2/catch_test_macros.hpp>
#include "uiframework/Focus/FocusManager.h"
#include "uiframework/UIElements/UIElement.h"
#include <memory>

// Mock UIElement for testing
class MockUIElement : public ui::UIElement {
public:
    MockUIElement(int x, int y, int w, int h) : UIElement(x, y, w, h) {}
    
    void render(SDL_Renderer*, TTF_Font*, std::shared_ptr<ui::Theme>) override {}
    bool isInteractive() const override { return true; }
};

TEST_CASE("FocusManager basic functionality", "[focusmanager]") {
    ui::FocusManager focusManager;
    
    SECTION("Element registration and focus") {
        auto element1 = std::make_shared<MockUIElement>(0, 0, 100, 30);
        auto element2 = std::make_shared<MockUIElement>(0, 50, 100, 30);
        
        element1->setId("element1");
        element2->setId("element2");
        
        focusManager.registerElement("element1", element1);
        focusManager.registerElement("element2", element2);
        
        // Test basic focus setting
        focusManager.setFocus("element1");
        REQUIRE(focusManager.getFocusedElementId() == "element1");
        REQUIRE(element1->hasFocus == true);
        REQUIRE(element2->hasFocus == false);
        
        // Test focus change
        focusManager.setFocus("element2");
        REQUIRE(focusManager.getFocusedElementId() == "element2");
        REQUIRE(element1->hasFocus == false);
        REQUIRE(element2->hasFocus == true);
    }
    
    SECTION("Focus cycling with default order") {
        auto element1 = std::make_shared<MockUIElement>(0, 0, 100, 30);
        auto element2 = std::make_shared<MockUIElement>(0, 50, 100, 30);
        auto element3 = std::make_shared<MockUIElement>(0, 100, 100, 30);
        
        element1->setId("element1");
        element2->setId("element2");
        element3->setId("element3");
        
        focusManager.registerElement("element1", element1);
        focusManager.registerElement("element2", element2);
        focusManager.registerElement("element3", element3);
        
        // Set explicit focus order to make test deterministic
        std::vector<std::string> order = {"element1", "element2", "element3"};
        focusManager.setFocusOrder(order);
        
        // Set initial focus
        focusManager.setFocus("element1");
        REQUIRE(focusManager.getFocusedElementId() == "element1");
        
        // Test forward cycling
        focusManager.focusNext();
        REQUIRE(focusManager.getFocusedElementId() == "element2");
        
        focusManager.focusNext();
        REQUIRE(focusManager.getFocusedElementId() == "element3");
        
        // Should wrap around
        focusManager.focusNext();
        REQUIRE(focusManager.getFocusedElementId() == "element1");
        
        // Test backward cycling
        focusManager.focusPrevious();
        REQUIRE(focusManager.getFocusedElementId() == "element3");
    }
    
    SECTION("Custom focus order") {
        auto element1 = std::make_shared<MockUIElement>(0, 0, 100, 30);
        auto element2 = std::make_shared<MockUIElement>(0, 50, 100, 30);
        auto element3 = std::make_shared<MockUIElement>(0, 100, 100, 30);
        
        element1->setId("element1");
        element2->setId("element2");
        element3->setId("element3");
        
        focusManager.registerElement("element1", element1);
        focusManager.registerElement("element2", element2);
        focusManager.registerElement("element3", element3);
        
        // Set custom focus order
        std::vector<std::string> customOrder = {"element3", "element1", "element2"};
        focusManager.setFocusOrder(customOrder);
        
        // Set initial focus
        focusManager.setFocus("element3");
        REQUIRE(focusManager.getFocusedElementId() == "element3");
        
        // Test cycling follows custom order
        focusManager.focusNext();
        REQUIRE(focusManager.getFocusedElementId() == "element1");
        
        focusManager.focusNext();
        REQUIRE(focusManager.getFocusedElementId() == "element2");
        
        // Should wrap around to first in custom order
        focusManager.focusNext();
        REQUIRE(focusManager.getFocusedElementId() == "element3");
    }
    
    SECTION("Element unregistration") {
        auto element1 = std::make_shared<MockUIElement>(0, 0, 100, 30);
        auto element2 = std::make_shared<MockUIElement>(0, 50, 100, 30);
        
        element1->setId("element1");
        element2->setId("element2");
        
        focusManager.registerElement("element1", element1);
        focusManager.registerElement("element2", element2);
        
        // Focus first element
        focusManager.setFocus("element1");
        REQUIRE(focusManager.getFocusedElementId() == "element1");
        
        // Unregister focused element
        focusManager.unregisterElement("element1");
        
        // Focus should be cleared
        REQUIRE(focusManager.getFocusedElementId().empty());
        REQUIRE(element1->hasFocus == false);
        
        // Should be able to focus remaining element
        focusManager.setFocus("element2");
        REQUIRE(focusManager.getFocusedElementId() == "element2");
    }
}
