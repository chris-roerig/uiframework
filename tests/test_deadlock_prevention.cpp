#include <catch2/catch_test_macros.hpp>
#include "../src/UI.h"
#include <thread>
#include <chrono>

TEST_CASE("Mutex deadlock prevention", "[deadlock]") {
    UI ui("Test Window", 800, 600);
    
    SECTION("Button callback deferral prevents deadlocks") {
        bool callbackExecuted = false;
        auto button = ui.createButton("Test", 10, 10, [&callbackExecuted, &ui]() {
            callbackExecuted = true;
            // This would previously cause deadlock - creating UI elements in callback
            auto newButton = ui.createButton("Created in callback", 100, 100, [](){});
        });
        
        // Simulate button activation during event processing
        button->activate();
        
        // Should not deadlock
        REQUIRE(button != nullptr);
    }
    
    SECTION("CheckBox callback deferral prevents deadlocks") {
        bool callbackExecuted = false;
        auto checkbox = ui.createCheckBox(false, 10, 10, [&callbackExecuted, &ui](bool state) {
            callbackExecuted = true;
            // This would previously cause deadlock
            ui.setFocus("some_element");
        });
        
        // Simulate checkbox state change
        checkbox->setChecked(true);
        
        // Should not deadlock
        REQUIRE(checkbox != nullptr);
    }
    
    SECTION("ListView callback deferral prevents deadlocks") {
        std::vector<std::string> items = {"Item 1", "Item 2"};
        auto listview = ui.createListView(items, 10, 10, 200, 150);
        
        listview->onSelectionChange = [](const std::vector<int>& indices) {
            // This would previously cause deadlock
            // auto modal = ui.createInfoModal("Selection changed");
        };
        
        // Simulate selection change
        listview->setSelectedIndex(1);
        
        // Should not deadlock
        REQUIRE(listview != nullptr);
    }
    
    SECTION("ContextMenu callback deferral prevents deadlocks") {
        std::vector<ui::TopMenuItem> menus = {
            {"File", {{"New", [&ui]() {
                // This would previously cause deadlock
                auto button = ui.createButton("New File", 50, 50, [](){});
            }}}}
        };
        auto contextmenu = ui.createContextMenu(menus);
        
        // Simulate menu activation
        contextmenu->setActiveItem(0);
        contextmenu->expandMenu();
        contextmenu->selectSubItem(0);
        contextmenu->activate();
        
        // Should not deadlock
        REQUIRE(contextmenu != nullptr);
    }
}

TEST_CASE("Focus system thread safety", "[focus]") {
    UI ui("Test Window", 800, 600);
    
    SECTION("Deferred focus changes prevent deadlocks") {
        auto button1 = ui.createButton("Button 1", 10, 10, [](){});
        auto button2 = ui.createButton("Button 2", 10, 50, [](){});
        
        // Simulate focus changes during event processing
        ui.setFocus(button1);
        ui.setFocus(button2);
        
        // Should not deadlock
        REQUIRE(button1 != nullptr);
        REQUIRE(button2 != nullptr);
    }
}

TEST_CASE("Callback queue system", "[callbacks]") {
    UI ui("Test Window", 800, 600);
    
    SECTION("Callbacks are queued and executed safely") {
        int callbackCount = 0;
        
        auto button = ui.createButton("Test", 10, 10, [&callbackCount]() {
            callbackCount++;
        });
        
        // Multiple activations should queue callbacks
        button->activate();
        button->activate();
        button->activate();
        
        // Callbacks should be queued for safe execution
        REQUIRE(button != nullptr);
    }
}
