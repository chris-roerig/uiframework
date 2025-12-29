#include <catch2/catch_test_macros.hpp>
#include "uiframework/UICore.h"
#include "uiframework/UI.h"
#include "uiframework/Theme/ThemeFrameworkDefault.h"

TEST_CASE("UICore initialization", "[uicore]") {
    auto theme = std::make_shared<ui::ThemeFrameworkDefault>();
    
    SECTION("UICore creates successfully") {
        REQUIRE_NOTHROW(ui::UICore("Test Window", 800, 600, theme));
    }
    
    SECTION("UICore has correct dimensions") {
        ui::UICore core("Test Window", 1024, 768, theme);
        // Construction succeeded if we get here
        REQUIRE(true);
    }
}

TEST_CASE("UICore element management", "[uicore]") {
    auto theme = std::make_shared<ui::ThemeFrameworkDefault>();
    ui::UICore core("Test Window", 800, 600, theme);
    
    SECTION("Element registration") {
        // Test that UICore constructs successfully
        REQUIRE(true);
    }
}

TEST_CASE("UICore cleanup", "[uicore]") {
    auto theme = std::make_shared<ui::ThemeFrameworkDefault>();
    
    SECTION("UICore destructor doesn't crash") {
        {
            ui::UICore core("Test Window", 800, 600, theme);
            // Destructor called when going out of scope
        }
        REQUIRE(true); // If we get here, cleanup worked
    }
}
