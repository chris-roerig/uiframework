#include <catch2/catch_test_macros.hpp>
#include "../src/UI.h"

TEST_CASE("Theme switching", "[theme]") {
    UI ui("Test Window", 800, 600);
    
    SECTION("Switch to different themes") {
        REQUIRE_NOTHROW(ui.setTheme("SolarizedDark"));
        REQUIRE_NOTHROW(ui.setTheme("SolarizedLight"));
        REQUIRE_NOTHROW(ui.setTheme("Molokai"));
        REQUIRE_NOTHROW(ui.setTheme("Default"));
    }
}

TEST_CASE("Theme with elements", "[theme]") {
    UI ui("Test Window", 800, 600);
    
    SECTION("Theme switching with existing elements") {
        auto button = ui.createButton("Test", 0, 0, [](){});
        auto label = ui.createLabel("Test", 0, 30);
        
        REQUIRE(button != nullptr);
        REQUIRE(label != nullptr);
        
        REQUIRE_NOTHROW(ui.setTheme("SolarizedDark"));
        REQUIRE_NOTHROW(ui.setTheme("Default"));
    }
}

TEST_CASE("Invalid theme handling", "[theme]") {
    UI ui("Test Window", 800, 600);
    
    SECTION("Invalid theme name") {
        // Should not crash with invalid theme name
        REQUIRE_NOTHROW(ui.setTheme("NonExistentTheme"));
    }
}
