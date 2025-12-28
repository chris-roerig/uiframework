#include <catch2/catch_test_macros.hpp>
#include "../src/UI.h"

TEST_CASE("Button creation", "[button]") {
    UI ui("Test Window", 800, 600);
    
    SECTION("Button creates successfully") {
        bool clicked = false;
        auto button = ui.createButton("Test Button", 10, 20, [&clicked](){ clicked = true; });
        
        REQUIRE(button != nullptr);
    }
    
    SECTION("Button with empty text") {
        auto button = ui.createButton("", 0, 0, [](){});
        REQUIRE(button != nullptr);
    }
}

TEST_CASE("Button properties", "[button]") {
    UI ui("Test Window", 800, 600);
    
    SECTION("Button positioning") {
        auto button = ui.createButton("Test", 100, 200, [](){});
        
        // Test that button was created at expected position
        // Note: We'd need getter methods to fully test this
        REQUIRE(button != nullptr);
    }
}

TEST_CASE("Button interactions", "[button]") {
    UI ui("Test Window", 800, 600);
    
    SECTION("Button callback execution") {
        bool clicked = false;
        auto button = ui.createButton("Click Me", 0, 0, [&clicked](){ 
            clicked = true; 
        });
        
        // Note: We'd need a way to trigger the callback to fully test this
        // For now, test that button with callback creates successfully
        REQUIRE(button != nullptr);
        REQUIRE(clicked == false); // Callback not triggered yet
    }
    
    SECTION("Button with null callback") {
        // Test that button works with empty callback
        auto button = ui.createButton("No Callback", 0, 0, [](){});
        REQUIRE(button != nullptr);
    }
}

TEST_CASE("Button element management", "[button]") {
    UI ui("Test Window", 800, 600);
    
    SECTION("Multiple buttons") {
        auto button1 = ui.createButton("Button 1", 0, 0, [](){});
        auto button2 = ui.createButton("Button 2", 50, 50, [](){});
        
        REQUIRE(button1 != nullptr);
        REQUIRE(button2 != nullptr);
        REQUIRE(button1 != button2); // Different instances
    }
}
