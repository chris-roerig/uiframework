#include <catch2/catch_test_macros.hpp>
#include "../src/UI.h"

// Simple test data
const unsigned char test_anim_data[] = {0x89, 0x50, 0x4E, 0x47}; // PNG header
const size_t test_anim_size = sizeof(test_anim_data);

TEST_CASE("AnimatedSprite creation", "[animatedsprite]") {
    UI ui("Test Window", 800, 600);
    
    SECTION("AnimatedSprite creates successfully") {
        REQUIRE_NOTHROW(ui.createAnimatedSprite(test_anim_data, test_anim_size, 
                                               10, 20, 64, 64, 4, 250));
    }
    
    SECTION("AnimatedSprite with different frame counts") {
        REQUIRE_NOTHROW(ui.createAnimatedSprite(test_anim_data, test_anim_size, 
                                               0, 0, 64, 64, 2, 500));
        REQUIRE_NOTHROW(ui.createAnimatedSprite(test_anim_data, test_anim_size, 
                                               0, 70, 64, 64, 8, 100));
    }
}

TEST_CASE("AnimatedSprite parameters", "[animatedsprite]") {
    UI ui("Test Window", 800, 600);
    
    SECTION("AnimatedSprite with stretch parameter") {
        REQUIRE_NOTHROW(ui.createAnimatedSprite(test_anim_data, test_anim_size, 
                                               0, 0, 128, 128, 4, 250, true));
        REQUIRE_NOTHROW(ui.createAnimatedSprite(test_anim_data, test_anim_size, 
                                               0, 140, 64, 64, 4, 250, false));
    }
}
