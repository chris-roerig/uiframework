#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>
#include "uiframework/UI.h"

TEST_CASE("Coordinate Validation Optimization", "[coordinates]") {
    UI ui("Test", 800, 600);
    
    SECTION("Valid coordinates pass quickly") {
        // These should all pass without exceptions
        REQUIRE_NOTHROW(ui.createLabel("Test1", 10, 10));
        REQUIRE_NOTHROW(ui.createLabel("Test2", 100, 100));
        REQUIRE_NOTHROW(ui.createLabel("Test3", 700, 500));
    }
    
    SECTION("Invalid coordinates still throw proper exceptions") {
        // Negative coordinates
        REQUIRE_THROWS_WITH(ui.createLabel("Test", -1, 10), 
                           Catch::Matchers::ContainsSubstring("coordinates cannot be negative"));
        
        // Out of bounds
        REQUIRE_THROWS_WITH(ui.createLabel("Test", 900, 10), 
                           Catch::Matchers::ContainsSubstring("extends beyond window boundaries"));
    }
}

TEST_CASE("Coordinate Validation Performance", "[coordinates][benchmark]") {
    UI ui("Test", 800, 600);
    
    BENCHMARK("Valid coordinate validation (1000 widgets)") {
        for (int i = 0; i < 1000; ++i) {
            ui.createLabel("Test" + std::to_string(i), 10 + (i % 100), 10 + (i % 100));
        }
        return 1000;
    };
}
