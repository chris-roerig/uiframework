#include <catch2/catch_test_macros.hpp>
#include "uiframework/Logger.h"
#include "uiframework/Constants.h"
#include "uiframework/Resources/TTFManager.h"

TEST_CASE("Phase 1 Improvements", "[phase1]") {
    
    SECTION("Logger functionality") {
        // Test that Logger works
        ui::Logger::setLevel(ui::LogLevel::DEBUG);
        REQUIRE(ui::Logger::getLevel() == ui::LogLevel::DEBUG);
        
        // Test logging (should not crash)
        ui::Logger::log(ui::LogLevel::INFO, "Test message");
        ui::Logger::log(ui::LogLevel::ERROR, "Test error");
        
        REQUIRE(true); // If we get here, logging works
    }
    
    SECTION("Constants are defined") {
        // Test that new constants exist
        REQUIRE(ui::Constants::BUTTON_PRESSED_DARKEN_FACTOR == 0.8f);
        REQUIRE(ui::Constants::FONT_CACHE_MAX_SIZE == 100);
        REQUIRE(ui::Constants::TEXT_CACHE_MAX_ENTRIES == 500);
        REQUIRE(std::string(ui::Constants::ELLIPSIS) == "...");
    }
    
    SECTION("TTFManager RAII") {
        // Test TTFManager initialization and cleanup
        {
            ui::TTFManager ttf1;
            REQUIRE(ttf1.isInitialized() == true);
            
            {
                ui::TTFManager ttf2;
                REQUIRE(ttf2.isInitialized() == true);
            } // ttf2 destroyed here
            
            REQUIRE(ttf1.isInitialized() == true);
        } // ttf1 destroyed here
        
        // Should be able to create new instance
        ui::TTFManager ttf3;
        REQUIRE(ttf3.isInitialized() == true);
    }
}
