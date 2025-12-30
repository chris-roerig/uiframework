#include <catch2/catch_test_macros.hpp>
#include "uiframework/UI.h"

TEST_CASE("TabbedPanel creation", "[tabbedpanel]") {
    UI ui("Test Window", 800, 600);
    
    SECTION("TabbedPanel creates successfully") {
        auto tabbedPanel = ui.createTabbedPanel(10, 20, 400, 300);
        
        REQUIRE(tabbedPanel != nullptr);
        REQUIRE(tabbedPanel->getActiveTab() == -1); // No tabs initially
    }
    
    SECTION("TabbedPanel with initial size") {
        auto tabbedPanel = ui.createTabbedPanel(0, 0, 200, 150);
        REQUIRE(tabbedPanel != nullptr);
    }
}

TEST_CASE("TabbedPanel tab management", "[tabbedpanel]") {
    UI ui("Test Window", 800, 600);
    auto tabbedPanel = ui.createTabbedPanel(10, 20, 400, 300);
    
    SECTION("Add single tab") {
        int tabIndex = tabbedPanel->addTab("First Tab");
        
        REQUIRE(tabIndex == 0);
        REQUIRE(tabbedPanel->getActiveTab() == 0);
    }
    
    SECTION("Add multiple tabs") {
        int tab1 = tabbedPanel->addTab("Tab 1");
        int tab2 = tabbedPanel->addTab("Tab 2");
        int tab3 = tabbedPanel->addTab("Tab 3");
        
        REQUIRE(tab1 == 0);
        REQUIRE(tab2 == 1);
        REQUIRE(tab3 == 2);
        REQUIRE(tabbedPanel->getActiveTab() == 0); // First tab active by default
    }
    
    SECTION("Set active tab") {
        tabbedPanel->addTab("Tab 1");
        tabbedPanel->addTab("Tab 2");
        tabbedPanel->addTab("Tab 3");
        
        tabbedPanel->setActiveTab(1);
        REQUIRE(tabbedPanel->getActiveTab() == 1);
        
        tabbedPanel->setActiveTab(2);
        REQUIRE(tabbedPanel->getActiveTab() == 2);
    }
    
    SECTION("Invalid tab index handling") {
        tabbedPanel->addTab("Tab 1");
        
        tabbedPanel->setActiveTab(-1);
        REQUIRE(tabbedPanel->getActiveTab() == 0); // Should remain unchanged
        
        tabbedPanel->setActiveTab(10);
        REQUIRE(tabbedPanel->getActiveTab() == 0); // Should remain unchanged
    }
}

TEST_CASE("TabbedPanel child management", "[tabbedpanel]") {
    UI ui("Test Window", 800, 600);
    auto tabbedPanel = ui.createTabbedPanel(10, 20, 400, 300);
    
    SECTION("Add children to tabs") {
        int tab1 = tabbedPanel->addTab("Tab 1");
        int tab2 = tabbedPanel->addTab("Tab 2");
        
        auto button1 = ui.createButton("Button 1", 50, 100, [](){});
        auto button2 = ui.createButton("Button 2", 50, 150, [](){});
        auto label1 = ui.createLabel("Label 1", 50, 200);
        
        tabbedPanel->addChildToTab(tab1, button1);
        tabbedPanel->addChildToTab(tab1, label1);
        tabbedPanel->addChildToTab(tab2, button2);
        
        // Children should be added successfully (no exceptions)
        REQUIRE(true);
    }
    
    SECTION("Child visibility management") {
        int tab1 = tabbedPanel->addTab("Tab 1");
        int tab2 = tabbedPanel->addTab("Tab 2");
        
        auto button1 = ui.createButton("Button 1", 50, 100, [](){});
        auto button2 = ui.createButton("Button 2", 50, 150, [](){});
        
        tabbedPanel->addChildToTab(tab1, button1);
        tabbedPanel->addChildToTab(tab2, button2);
        
        // Tab 1 is active by default, so button1 should be visible
        REQUIRE(button1->isVisible() == true);
        REQUIRE(button2->isVisible() == false);
        
        // Switch to tab 2
        tabbedPanel->setActiveTab(1);
        REQUIRE(button1->isVisible() == false);
        REQUIRE(button2->isVisible() == true);
        
        // Switch back to tab 1
        tabbedPanel->setActiveTab(0);
        REQUIRE(button1->isVisible() == true);
        REQUIRE(button2->isVisible() == false);
    }
    
    SECTION("Add child to invalid tab") {
        tabbedPanel->addTab("Tab 1");
        auto button = ui.createButton("Button", 50, 100, [](){});
        
        // Should handle invalid tab index gracefully
        tabbedPanel->addChildToTab(-1, button);
        tabbedPanel->addChildToTab(10, button);
        
        REQUIRE(true); // No crash expected
    }
}

TEST_CASE("TabbedPanel callback", "[tabbedpanel]") {
    UI ui("Test Window", 800, 600);
    auto tabbedPanel = ui.createTabbedPanel(10, 20, 400, 300);
    
    SECTION("Tab change callback fires") {
        int callbackTabIndex = -1;
        
        tabbedPanel->setOnTabChange([&](int tabIndex) {
            callbackTabIndex = tabIndex;
        });
        
        tabbedPanel->addTab("Tab 1");
        tabbedPanel->addTab("Tab 2");
        tabbedPanel->addTab("Tab 3");
        
        tabbedPanel->setActiveTab(1);
        REQUIRE(callbackTabIndex == 1);
        
        tabbedPanel->setActiveTab(2);
        REQUIRE(callbackTabIndex == 2);
        
        tabbedPanel->setActiveTab(0);
        REQUIRE(callbackTabIndex == 0);
    }
}
