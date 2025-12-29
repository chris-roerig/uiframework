#include <catch2/catch_test_macros.hpp>
#include "uiframework/UI.h"

TEST_CASE("Canvas creation", "[canvas]") {
    UI ui("Test Window", 800, 600);
    
    SECTION("Canvas creates successfully") {
        auto canvas = ui.createCanvas(10, 20, 200, 150);
        REQUIRE(canvas != nullptr);
    }
    
    SECTION("Canvas with different dimensions") {
        auto small = ui.createCanvas(0, 0, 50, 50);
        auto large = ui.createCanvas(0, 60, 400, 300);
        
        REQUIRE(small != nullptr);
        REQUIRE(large != nullptr);
        REQUIRE(small != large);
    }
}

TEST_CASE("Canvas positioning", "[canvas]") {
    UI ui("Test Window", 800, 600);
    
    SECTION("Canvas at different positions") {
        auto canvas1 = ui.createCanvas(0, 0, 100, 100);
        auto canvas2 = ui.createCanvas(150, 150, 100, 100);
        
        REQUIRE(canvas1 != nullptr);
        REQUIRE(canvas2 != nullptr);
        REQUIRE(canvas1 != canvas2);
    }
}

TEST_CASE("Multiple canvases", "[canvas]") {
    UI ui("Test Window", 800, 600);
    
    SECTION("Creating multiple canvases") {
        auto canvas1 = ui.createCanvas(0, 0, 100, 100);
        auto canvas2 = ui.createCanvas(110, 0, 100, 100);
        auto canvas3 = ui.createCanvas(220, 0, 100, 100);
        
        REQUIRE(canvas1 != nullptr);
        REQUIRE(canvas2 != nullptr);
        REQUIRE(canvas3 != nullptr);
    }
}
