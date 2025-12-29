#include <catch2/catch_test_macros.hpp>
#include "uiframework/UI.h"

TEST_CASE("Label creation", "[label]") {
    UI ui("Test Window", 800, 600);
    
    SECTION("Label creates successfully") {
        auto label = ui.createLabel("Test Label", 10, 20);
        REQUIRE(label != nullptr);
    }
    
    SECTION("Label with empty text") {
        auto label = ui.createLabel("", 0, 0);
        REQUIRE(label != nullptr);
    }
}

TEST_CASE("Label positioning", "[label]") {
    UI ui("Test Window", 800, 600);
    
    SECTION("Label at different positions") {
        auto label1 = ui.createLabel("Label 1", 0, 0);
        auto label2 = ui.createLabel("Label 2", 100, 200);
        
        REQUIRE(label1 != nullptr);
        REQUIRE(label2 != nullptr);
        REQUIRE(label1 != label2);
    }
}

TEST_CASE("Multiple labels", "[label]") {
    UI ui("Test Window", 800, 600);
    
    SECTION("Creating multiple labels") {
        auto label1 = ui.createLabel("First", 0, 0);
        auto label2 = ui.createLabel("Second", 50, 50);
        auto label3 = ui.createLabel("Third", 100, 100);
        
        REQUIRE(label1 != nullptr);
        REQUIRE(label2 != nullptr);
        REQUIRE(label3 != nullptr);
    }
}
