#include <catch2/catch_test_macros.hpp>
#include "uiframework/UICore.h"
#include "uiframework/UIElements/UIElement.h"
#include <memory>
#include <SDL2/SDL.h>

// Mock UIElement for Tab navigation testing
class MockTabElement : public ui::UIElement {
private:
    bool interactive = true;
    
public:
    MockTabElement(const std::string& id, int x = 0, int y = 0, int w = 100, int h = 30) 
        : UIElement(x, y, w, h) {
        setId(id);
    }
    
    void render(SDL_Renderer*, TTF_Font*, std::shared_ptr<ui::Theme>) override {}
    void handleEvent(const SDL_Event&) override {}
    
    bool isInteractive() const override { return interactive; }
    
    void setInteractive(bool value) { interactive = value; }
};

TEST_CASE("Tab Key Navigation", "[focus][tab][keyboard]") {
    
    SECTION("Tab key cycles through visible elements only") {
        // Create mock elements
        auto elem1 = std::make_shared<MockTabElement>("elem1");
        auto elem2 = std::make_shared<MockTabElement>("elem2");
        auto elem3 = std::make_shared<MockTabElement>("elem3");
        
        // Hide middle element
        elem2->setVisible(false);
        
        ui::FocusManager focusManager;
        focusManager.registerElement("elem1", elem1);
        focusManager.registerElement("elem2", elem2);
        focusManager.registerElement("elem3", elem3);
        
        // Start with first element focused
        focusManager.setFocus("elem1");
        REQUIRE(focusManager.getFocusedElementId() == "elem1");
        
        // Tab should skip hidden element and go to elem3
        focusManager.focusNext();
        REQUIRE(focusManager.getFocusedElementId() == "elem3");
        
        // Tab again should wrap to elem1 (skipping hidden elem2)
        focusManager.focusNext();
        REQUIRE(focusManager.getFocusedElementId() == "elem1");
    }
    
    SECTION("Shift+Tab cycles backward through visible elements") {
        auto elem1 = std::make_shared<MockTabElement>("elem1");
        auto elem2 = std::make_shared<MockTabElement>("elem2");
        auto elem3 = std::make_shared<MockTabElement>("elem3");
        
        // Hide middle element
        elem2->setVisible(false);
        
        ui::FocusManager focusManager;
        focusManager.registerElement("elem1", elem1);
        focusManager.registerElement("elem2", elem2);
        focusManager.registerElement("elem3", elem3);
        
        // Start with elem3 focused
        focusManager.setFocus("elem3");
        REQUIRE(focusManager.getFocusedElementId() == "elem3");
        
        // Shift+Tab should skip hidden element and go to elem1
        focusManager.focusPrevious();
        REQUIRE(focusManager.getFocusedElementId() == "elem1");
        
        // Shift+Tab again should wrap to elem3 (skipping hidden elem2)
        focusManager.focusPrevious();
        REQUIRE(focusManager.getFocusedElementId() == "elem3");
    }
    
    SECTION("Tab navigation with custom focus order") {
        auto elem1 = std::make_shared<MockTabElement>("elem1");
        auto elem2 = std::make_shared<MockTabElement>("elem2");
        auto elem3 = std::make_shared<MockTabElement>("elem3");
        
        ui::FocusManager focusManager;
        focusManager.registerElement("elem1", elem1);
        focusManager.registerElement("elem2", elem2);
        focusManager.registerElement("elem3", elem3);
        
        // Set custom tab order: elem3 -> elem1 -> elem2
        std::vector<std::string> customOrder = {"elem3", "elem1", "elem2"};
        focusManager.setFocusOrder(customOrder);
        
        // Hide elem1 (middle in custom order)
        elem1->setVisible(false);
        
        focusManager.setFocus("elem3");
        REQUIRE(focusManager.getFocusedElementId() == "elem3");
        
        // Tab should skip hidden elem1 and go to elem2
        focusManager.focusNext();
        REQUIRE(focusManager.getFocusedElementId() == "elem2");
        
        // Tab should wrap back to elem3 (skipping hidden elem1)
        focusManager.focusNext();
        REQUIRE(focusManager.getFocusedElementId() == "elem3");
    }
    
    SECTION("No focusable elements available") {
        auto elem1 = std::make_shared<MockTabElement>("elem1");
        auto elem2 = std::make_shared<MockTabElement>("elem2");
        
        ui::FocusManager focusManager;
        focusManager.registerElement("elem1", elem1);
        focusManager.registerElement("elem2", elem2);
        
        // Hide all elements
        elem1->setVisible(false);
        elem2->setVisible(false);
        
        // Try to focus invisible element - should fail
        focusManager.setFocus("elem1");
        REQUIRE(focusManager.getFocusedElementId().empty());
        
        // Tab navigation should do nothing when no elements are focusable
        focusManager.focusNext();
        REQUIRE(focusManager.getFocusedElementId().empty());
        
        focusManager.focusPrevious();
        REQUIRE(focusManager.getFocusedElementId().empty());
    }
    
    SECTION("Single focusable element") {
        auto elem1 = std::make_shared<MockTabElement>("elem1");
        auto elem2 = std::make_shared<MockTabElement>("elem2");
        
        ui::FocusManager focusManager;
        focusManager.registerElement("elem1", elem1);
        focusManager.registerElement("elem2", elem2);
        
        // Hide one element, leaving only one focusable
        elem2->setVisible(false);
        
        focusManager.setFocus("elem1");
        REQUIRE(focusManager.getFocusedElementId() == "elem1");
        
        // Tab should stay on the same element
        focusManager.focusNext();
        REQUIRE(focusManager.getFocusedElementId() == "elem1");
        
        focusManager.focusPrevious();
        REQUIRE(focusManager.getFocusedElementId() == "elem1");
    }
}

TEST_CASE("Tab Navigation Edge Cases", "[focus][tab][edge]") {
    
    SECTION("Element becomes invisible while focused") {
        auto elem1 = std::make_shared<MockTabElement>("elem1");
        auto elem2 = std::make_shared<MockTabElement>("elem2");
        
        ui::FocusManager focusManager;
        focusManager.registerElement("elem1", elem1);
        focusManager.registerElement("elem2", elem2);
        
        focusManager.setFocus("elem1");
        REQUIRE(focusManager.getFocusedElementId() == "elem1");
        REQUIRE(elem1->hasFocus == true);
        
        // Make focused element invisible
        elem1->setVisible(false);
        
        // Current focus remains until explicitly changed
        REQUIRE(focusManager.getFocusedElementId() == "elem1");
        
        // But new focus attempts on invisible element should fail
        focusManager.clearFocus();
        focusManager.setFocus("elem1");
        REQUIRE(focusManager.getFocusedElementId().empty());
        
        // Tab navigation should focus the remaining visible element
        focusManager.focusNext();
        REQUIRE(focusManager.getFocusedElementId() == "elem2");
    }
    
    SECTION("All elements become invisible during navigation") {
        auto elem1 = std::make_shared<MockTabElement>("elem1");
        auto elem2 = std::make_shared<MockTabElement>("elem2");
        
        ui::FocusManager focusManager;
        focusManager.registerElement("elem1", elem1);
        focusManager.registerElement("elem2", elem2);
        
        focusManager.setFocus("elem1");
        REQUIRE(focusManager.getFocusedElementId() == "elem1");
        
        // Hide all elements
        elem1->setVisible(false);
        elem2->setVisible(false);
        
        // Tab navigation should not find any focusable elements, focus stays on current
        focusManager.focusNext();
        REQUIRE(focusManager.getFocusedElementId() == "elem1"); // Stays on current since no alternatives
        
        // But trying to explicitly set focus on invisible element should fail
        focusManager.clearFocus(); // Clear first
        focusManager.setFocus("elem1");
        REQUIRE(focusManager.getFocusedElementId().empty());
    }
}
