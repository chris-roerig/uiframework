#include <catch2/catch_test_macros.hpp>
#include "../src/UI.h"

TEST_CASE("Modal creation", "[modal]") {
    UI ui("Test Window", 800, 600);
    
    SECTION("Modal creates successfully") {
        auto modal = ui.createModal("Test Modal", "OK", false);
        REQUIRE(modal != nullptr);
    }
    
    SECTION("Modal with cancel button") {
        auto modal = ui.createModal("Confirm Action", "Yes", true);
        REQUIRE(modal != nullptr);
    }
}

TEST_CASE("Modal types", "[modal]") {
    UI ui("Test Window", 800, 600);
    
    SECTION("Info modal") {
        auto info = ui.createInfoModal("Information message");
        REQUIRE(info != nullptr);
    }
    
    SECTION("Confirm modal") {
        auto confirm = ui.createConfirmModal("Are you sure?");
        REQUIRE(confirm != nullptr);
    }
}

TEST_CASE("Modal with callbacks", "[modal]") {
    UI ui("Test Window", 800, 600);
    
    SECTION("Modal with close callback") {
        bool closed = false;
        auto modal = ui.createModal("Test", "OK", false, [&closed](){ closed = true; });
        
        REQUIRE(modal != nullptr);
        REQUIRE(closed == false); // Callback not triggered yet
    }
}
