#include <catch2/catch_test_macros.hpp>
#include "uiframework/UI.h"

// Simple test data
const unsigned char test_sprite_data[] = {0x89, 0x50, 0x4E, 0x47}; // PNG header
const size_t test_sprite_size = sizeof(test_sprite_data);

TEST_CASE("Sprite creation", "[sprite]") {
    UI ui("Test Window", 800, 600);
    
    SECTION("Sprite creates with test data") {
        REQUIRE_NOTHROW(ui.createSprite(test_sprite_data, test_sprite_size, 10, 20, 64, 64, false));
    }
    
    SECTION("Sprite with different dimensions") {
        REQUIRE_NOTHROW(ui.createSprite(test_sprite_data, test_sprite_size, 0, 0, 32, 32, false));
        REQUIRE_NOTHROW(ui.createSprite(test_sprite_data, test_sprite_size, 0, 40, 128, 128, true));
    }
}

TEST_CASE("Multiple sprites", "[sprite]") {
    UI ui("Test Window", 800, 600);
    
    SECTION("Creating multiple sprites") {
        REQUIRE_NOTHROW(ui.createSprite(test_sprite_data, test_sprite_size, 0, 0, 64, 64, false));
        REQUIRE_NOTHROW(ui.createSprite(test_sprite_data, test_sprite_size, 70, 0, 64, 64, false));
    }
}
