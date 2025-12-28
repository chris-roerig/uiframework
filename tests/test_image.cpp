#include <catch2/catch_test_macros.hpp>
#include "../src/UI.h"

// Simple test data
const unsigned char test_image_data[] = {0x89, 0x50, 0x4E, 0x47}; // PNG header
const size_t test_image_size = sizeof(test_image_data);

TEST_CASE("Image creation", "[image]") {
    UI ui("Test Window", 800, 600);
    
    SECTION("Image creates with test data") {
        // Test basic creation - may fail but shouldn't crash
        REQUIRE_NOTHROW(ui.createImage(test_image_data, test_image_size, 10, 20, 50, 50, false));
    }
    
    SECTION("Image with different dimensions") {
        REQUIRE_NOTHROW(ui.createImage(test_image_data, test_image_size, 0, 0, 32, 32, false));
        REQUIRE_NOTHROW(ui.createImage(test_image_data, test_image_size, 0, 40, 100, 100, true));
    }
}

TEST_CASE("Image positioning", "[image]") {
    UI ui("Test Window", 800, 600);
    
    SECTION("Images at different positions") {
        REQUIRE_NOTHROW(ui.createImage(test_image_data, test_image_size, 0, 0, 50, 50, false));
        REQUIRE_NOTHROW(ui.createImage(test_image_data, test_image_size, 100, 100, 50, 50, false));
    }
}
