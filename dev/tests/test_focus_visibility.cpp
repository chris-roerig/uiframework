#include <catch2/catch_test_macros.hpp>
#include "uiframework/Focus/FocusManager.h"
#include "uiframework/UIElements/UIElement.h"
#include <memory>
#include <vector>

// Mock UIElement for testing
class MockFocusElement : public ui::UIElement {
private:
    bool interactive = true;
    
public:
    MockFocusElement(const std::string& id, int x = 0, int y = 0, int w = 100, int h = 30) 
        : UIElement(x, y, w, h) {
        setId(id);
    }
    
    void render(SDL_Renderer*, TTF_Font*, std::shared_ptr<ui::Theme>) override {}
    void handleEvent(const SDL_Event&) override {}
    
    bool isInteractive() const override { return interactive; }
    
    void setInteractive(bool value) { interactive = value; }
};

TEST_CASE("Focus System Visibility Tests", "[focus][visibility]") {
    ui::FocusManager focusManager;
    
    SECTION("Hidden elements are not focusable") {
        auto element1 = std::make_shared<MockFocusElement>("elem1");
        auto element2 = std::make_shared<MockFocusElement>("elem2");
        auto element3 = std::make_shared<MockFocusElement>("elem3");
        
        focusManager.registerElement("elem1", element1);
        focusManager.registerElement("elem2", element2);
        focusManager.registerElement("elem3", element3);
        
        // Hide element2
        element2->setVisible(false);
        
        // Focus cycling should skip hidden element
        focusManager.setFocus("elem1");
        REQUIRE(focusManager.getFocusedElementId() == "elem1");
        
        focusManager.focusNext();
        REQUIRE(focusManager.getFocusedElementId() == "elem3"); // Skipped elem2
        
        focusManager.focusPrevious();
        REQUIRE(focusManager.getFocusedElementId() == "elem1"); // Skipped elem2
    }
    
    SECTION("Making hidden element visible makes it focusable") {
        auto element1 = std::make_shared<MockFocusElement>("elem1");
        auto element2 = std::make_shared<MockFocusElement>("elem2");
        
        focusManager.registerElement("elem1", element1);
        focusManager.registerElement("elem2", element2);
        
        // Hide element2
        element2->setVisible(false);
        
        focusManager.setFocus("elem1");
        focusManager.focusNext();
        REQUIRE(focusManager.getFocusedElementId() == "elem1"); // Wrapped around, skipped elem2
        
        // Make element2 visible
        element2->setVisible(true);
        
        focusManager.focusNext();
        REQUIRE(focusManager.getFocusedElementId() == "elem2"); // Now includes elem2
    }
    
    SECTION("Non-interactive elements are not focusable even if visible") {
        auto element1 = std::make_shared<MockFocusElement>("elem1");
        auto element2 = std::make_shared<MockFocusElement>("elem2");
        
        focusManager.registerElement("elem1", element1);
        focusManager.registerElement("elem2", element2);
        
        // Make element2 non-interactive
        element2->setInteractive(false);
        
        focusManager.setFocus("elem1");
        focusManager.focusNext();
        REQUIRE(focusManager.getFocusedElementId() == "elem1"); // Wrapped around, skipped elem2
    }
    
    SECTION("Custom focus order respects visibility") {
        auto element1 = std::make_shared<MockFocusElement>("elem1");
        auto element2 = std::make_shared<MockFocusElement>("elem2");
        auto element3 = std::make_shared<MockFocusElement>("elem3");
        
        focusManager.registerElement("elem1", element1);
        focusManager.registerElement("elem2", element2);
        focusManager.registerElement("elem3", element3);
        
        // Set custom focus order
        std::vector<std::string> customOrder = {"elem3", "elem1", "elem2"};
        focusManager.setFocusOrder(customOrder);
        
        // Hide middle element in custom order
        element1->setVisible(false);
        
        focusManager.setFocus("elem3");
        focusManager.focusNext();
        REQUIRE(focusManager.getFocusedElementId() == "elem2"); // Skipped hidden elem1
        
        focusManager.focusPrevious();
        REQUIRE(focusManager.getFocusedElementId() == "elem3"); // Skipped hidden elem1
    }
}

TEST_CASE("Tab Navigation Integration Tests", "[focus][tab]") {
    ui::FocusManager focusManager;
    
    SECTION("Tab cycling with mixed visibility") {
        auto elem1 = std::make_shared<MockFocusElement>("elem1");
        auto elem2 = std::make_shared<MockFocusElement>("elem2");
        auto elem3 = std::make_shared<MockFocusElement>("elem3");
        
        focusManager.registerElement("elem1", elem1);
        focusManager.registerElement("elem2", elem2);
        focusManager.registerElement("elem3", elem3);
        
        // Hide middle element
        elem2->setVisible(false);
        
        // Test forward cycling: elem1 -> elem3 (skip elem2) -> elem1
        focusManager.setFocus("elem1");
        focusManager.focusNext();
        REQUIRE(focusManager.getFocusedElementId() == "elem3"); // Skip hidden elem2
        
        focusManager.focusNext();
        REQUIRE(focusManager.getFocusedElementId() == "elem1"); // Wrap around
        
        // Test backward cycling: elem1 -> elem3 (skip elem2) -> elem1
        focusManager.focusPrevious();
        REQUIRE(focusManager.getFocusedElementId() == "elem3"); // Skip hidden elem2
        
        focusManager.focusPrevious();
        REQUIRE(focusManager.getFocusedElementId() == "elem1"); // Wrap around
    }
    
    SECTION("Dynamic visibility changes during navigation") {
        auto element1 = std::make_shared<MockFocusElement>("elem1");
        auto element2 = std::make_shared<MockFocusElement>("elem2");
        auto element3 = std::make_shared<MockFocusElement>("elem3");
        
        focusManager.registerElement("elem1", element1);
        focusManager.registerElement("elem2", element2);
        focusManager.registerElement("elem3", element3);
        
        focusManager.setFocus("elem1");
        
        // Hide element2 while navigating
        element2->setVisible(false);
        
        focusManager.focusNext();
        REQUIRE(focusManager.getFocusedElementId() == "elem3"); // Skipped hidden elem2
        
        // Show element2 again
        element2->setVisible(true);
        
        focusManager.focusPrevious();
        REQUIRE(focusManager.getFocusedElementId() == "elem1"); // Goes to elem1 (previous in order)
    }
}

TEST_CASE("Focus State Management", "[focus][state]") {
    ui::FocusManager focusManager;
    
    SECTION("Focus callbacks are triggered correctly") {
        auto element1 = std::make_shared<MockFocusElement>("elem1");
        auto element2 = std::make_shared<MockFocusElement>("elem2");
        
        focusManager.registerElement("elem1", element1);
        focusManager.registerElement("elem2", element2);
        
        // Test focus gained/lost states
        focusManager.setFocus("elem1");
        REQUIRE(element1->hasFocus == true);
        REQUIRE(element2->hasFocus == false);
        
        focusManager.setFocus("elem2");
        REQUIRE(element1->hasFocus == false);
        REQUIRE(element2->hasFocus == true);
        
        focusManager.clearFocus();
        REQUIRE(element1->hasFocus == false);
        REQUIRE(element2->hasFocus == false);
        REQUIRE(focusManager.getFocusedElementId().empty());
    }
    
    SECTION("Focus setting fails for non-focusable elements") {
        auto element = std::make_shared<MockFocusElement>("elem");
        focusManager.registerElement("elem", element);
        
        focusManager.setFocus("elem");
        REQUIRE(focusManager.getFocusedElementId() == "elem");
        REQUIRE(element->hasFocus == true);
        
        // Make element non-interactive
        element->setInteractive(false);
        
        // Current focus remains, but new focus attempts fail
        REQUIRE(focusManager.getFocusedElementId() == "elem"); // Still focused
        
        // Clear focus and try to set again - should fail
        focusManager.clearFocus();
        focusManager.setFocus("elem");
        REQUIRE(focusManager.getFocusedElementId().empty());
    }
    
    SECTION("Focus setting fails for invisible elements") {
        auto element = std::make_shared<MockFocusElement>("elem");
        focusManager.registerElement("elem", element);
        
        focusManager.setFocus("elem");
        REQUIRE(focusManager.getFocusedElementId() == "elem");
        REQUIRE(element->hasFocus == true);
        
        // Make element invisible
        element->setVisible(false);
        
        // Current focus remains, but new focus attempts fail
        REQUIRE(focusManager.getFocusedElementId() == "elem"); // Still focused
        
        // Clear focus and try to set again - should fail
        focusManager.clearFocus();
        focusManager.setFocus("elem");
        REQUIRE(focusManager.getFocusedElementId().empty());
    }
}
