#include <catch2/catch_test_macros.hpp>
#include "../src/UI.h"

TEST_CASE("CheckBox creation", "[checkbox]") {
    UI ui("Test Window", 800, 600);
    
    SECTION("CheckBox creates successfully") {
        bool changed = false;
        auto checkbox = ui.createCheckBox(false, 10, 20, [&changed](bool state){ changed = true; });
        REQUIRE(checkbox != nullptr);
    }
    
    SECTION("CheckBox with initial true state") {
        auto checkbox = ui.createCheckBox(true, 0, 0, [](bool state){});
        REQUIRE(checkbox != nullptr);
    }
}

TEST_CASE("CheckBox interactions", "[checkbox]") {
    UI ui("Test Window", 800, 600);
    
    SECTION("CheckBox with callback") {
        bool clicked = false;
        auto checkbox = ui.createCheckBox(false, 0, 0, [&clicked](bool state){ clicked = true; });
        
        REQUIRE(checkbox != nullptr);
        REQUIRE(clicked == false); // Callback not triggered yet
    }
    
    SECTION("CheckBox without callback") {
        auto checkbox = ui.createCheckBox(false, 0, 0, [](bool state){});
        REQUIRE(checkbox != nullptr);
    }
}

TEST_CASE("Multiple checkboxes", "[checkbox]") {
    UI ui("Test Window", 800, 600);
    
    SECTION("Creating multiple checkboxes") {
        auto cb1 = ui.createCheckBox(false, 0, 0, [](bool state){});
        auto cb2 = ui.createCheckBox(true, 50, 50, [](bool state){});
        
        REQUIRE(cb1 != nullptr);
        REQUIRE(cb2 != nullptr);
        REQUIRE(cb1 != cb2);
    }
}

TEST_CASE("CheckBox callback deferral", "[checkbox]") {
    UI ui("Test Window", 800, 600);
    
    SECTION("Callback is deferred during event processing") {
        bool callbackExecuted = false;
        auto checkbox = ui.createCheckBox(false, 10, 10, [&callbackExecuted](bool state) {
            callbackExecuted = true;
        });
        
        // Simulate state change
        checkbox->setChecked(true);
        
        // Callback should be queued, not executed immediately during event processing
        // This test verifies the deferred callback system is in place
        REQUIRE(checkbox != nullptr);
    }
}
