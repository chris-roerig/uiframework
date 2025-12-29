#include <catch2/catch_test_macros.hpp>
#include "uiframework/UI.h"

TEST_CASE("VBoxLayout creation", "[layout][vbox]") {
    UI ui("Test Window", 800, 600);
    
    SECTION("VBoxLayout creates successfully") {
        auto vbox = ui.createVBoxLayout(10, 20, 200, 300);
        REQUIRE(vbox != nullptr);
    }
    
    SECTION("VBoxLayout with different dimensions") {
        auto small = ui.createVBoxLayout(10, 10, 100, 150);
        auto large = ui.createVBoxLayout(10, 170, 300, 400);
        
        REQUIRE(small != nullptr);
        REQUIRE(large != nullptr);
        REQUIRE(small != large);
    }
}

TEST_CASE("HBoxLayout creation", "[layout][hbox]") {
    UI ui("Test Window", 800, 600);
    
    SECTION("HBoxLayout creates successfully") {
        auto hbox = ui.createHBoxLayout(10, 20, 300, 100);
        REQUIRE(hbox != nullptr);
    }
    
    SECTION("HBoxLayout with different dimensions") {
        auto narrow = ui.createHBoxLayout(10, 10, 200, 50);
        auto wide = ui.createHBoxLayout(10, 70, 500, 80);
        
        REQUIRE(narrow != nullptr);
        REQUIRE(wide != nullptr);
        REQUIRE(narrow != wide);
    }
}

TEST_CASE("GridLayout creation", "[layout][grid]") {
    UI ui("Test Window", 800, 600);
    
    SECTION("GridLayout creates successfully") {
        auto grid = ui.createGridLayout(10, 20, 200, 200, 3, 3);
        REQUIRE(grid != nullptr);
    }
    
    SECTION("GridLayout with different grid sizes") {
        auto small_grid = ui.createGridLayout(10, 10, 150, 150, 2, 2);
        auto large_grid = ui.createGridLayout(10, 170, 300, 300, 4, 4);
        
        REQUIRE(small_grid != nullptr);
        REQUIRE(large_grid != nullptr);
        REQUIRE(small_grid != large_grid);
    }
}

TEST_CASE("Layout positioning", "[layout]") {
    UI ui("Test Window", 800, 600);
    
    SECTION("Different layout types at different positions") {
        auto vbox = ui.createVBoxLayout(10, 10, 100, 200);
        auto hbox = ui.createHBoxLayout(120, 10, 200, 100);
        auto grid = ui.createGridLayout(330, 10, 150, 150, 3, 3);
        
        REQUIRE(vbox != nullptr);
        REQUIRE(hbox != nullptr);
        REQUIRE(grid != nullptr);
    }
}
