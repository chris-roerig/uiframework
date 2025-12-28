#include <catch2/catch_test_macros.hpp>
#include "../src/UI.h"

TEST_CASE("ProgressBar creation", "[progressbar]") {
    UI ui("Test Window", 800, 600);
    
    SECTION("ProgressBar creates successfully") {
        auto progressbar = ui.createProgressBar(10, 20, 200, 30);
        REQUIRE(progressbar != nullptr);
    }
    
    SECTION("ProgressBar with different initial values") {
        auto pb1 = ui.createProgressBar(0, 0, 100, 20, 0.5f);
        auto pb2 = ui.createProgressBar(0, 30, 100, 20, 1.0f);
        
        REQUIRE(pb1 != nullptr);
        REQUIRE(pb2 != nullptr);
    }
}

TEST_CASE("ProgressBar dimensions", "[progressbar]") {
    UI ui("Test Window", 800, 600);
    
    SECTION("ProgressBar with different sizes") {
        auto small = ui.createProgressBar(0, 0, 50, 10);
        auto large = ui.createProgressBar(0, 20, 300, 50);
        
        REQUIRE(small != nullptr);
        REQUIRE(large != nullptr);
        REQUIRE(small != large);
    }
}

TEST_CASE("ProgressBar values", "[progressbar]") {
    UI ui("Test Window", 800, 600);
    
    SECTION("ProgressBar with boundary values") {
        auto empty = ui.createProgressBar(0, 0, 100, 20, 0.0f);
        auto full = ui.createProgressBar(0, 30, 100, 20, 1.0f);
        auto half = ui.createProgressBar(0, 60, 100, 20, 0.5f);
        
        REQUIRE(empty != nullptr);
        REQUIRE(full != nullptr);
        REQUIRE(half != nullptr);
    }
}
