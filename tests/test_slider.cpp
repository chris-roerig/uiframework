#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include "../src/UI.h"
#include "../lib/UIElements/Slider.h"

TEST_CASE("Slider Base Class", "[slider]") {
    UI ui("Test", 800, 600);
    
    SECTION("HSlider Creation and Basic Properties") {
        auto hslider = ui.createHSlider(10, 10, 200, 30, 0.0f, 100.0f, 50.0f);
        
        REQUIRE(hslider != nullptr);
        REQUIRE(hslider->getValue() == 50.0f);
    }
    
    SECTION("VSlider Creation and Basic Properties") {
        auto vslider = ui.createVSlider(10, 10, 30, 200, 0.0f, 100.0f, 25.0f);
        
        REQUIRE(vslider != nullptr);
        REQUIRE(vslider->getValue() == 25.0f);
    }
    
    SECTION("KnobSlider Creation and Basic Properties") {
        auto knobslider = ui.createKnobSlider(10, 10, 60, 0.0f, 100.0f, 75.0f);
        
        REQUIRE(knobslider != nullptr);
        REQUIRE(knobslider->getValue() == 75.0f);
    }
}

TEST_CASE("Slider Value Management", "[slider]") {
    UI ui("Test", 800, 600);
    
    SECTION("HSlider Value Setting and Clamping") {
        auto hslider = ui.createHSlider(10, 10, 200, 30, 0.0f, 100.0f, 50.0f);
        
        // Test normal value setting
        hslider->setValue(75.0f);
        REQUIRE(hslider->getValue() == 75.0f);
        
        // Test clamping to minimum
        hslider->setValue(-10.0f);
        REQUIRE(hslider->getValue() == 0.0f);
        
        // Test clamping to maximum
        hslider->setValue(150.0f);
        REQUIRE(hslider->getValue() == 100.0f);
    }
    
    SECTION("VSlider Value Setting and Clamping") {
        auto vslider = ui.createVSlider(10, 10, 30, 200, -50.0f, 50.0f, 0.0f);
        
        // Test normal value setting
        vslider->setValue(25.0f);
        REQUIRE(vslider->getValue() == 25.0f);
        
        // Test clamping to minimum
        vslider->setValue(-100.0f);
        REQUIRE(vslider->getValue() == -50.0f);
        
        // Test clamping to maximum
        vslider->setValue(100.0f);
        REQUIRE(vslider->getValue() == 50.0f);
    }
    
    SECTION("KnobSlider Value Setting and Clamping") {
        auto knobslider = ui.createKnobSlider(10, 10, 60, 10.0f, 90.0f, 50.0f);
        
        // Test normal value setting
        knobslider->setValue(70.0f);
        REQUIRE(knobslider->getValue() == 70.0f);
        
        // Test clamping to minimum
        knobslider->setValue(5.0f);
        REQUIRE(knobslider->getValue() == 10.0f);
        
        // Test clamping to maximum
        knobslider->setValue(95.0f);
        REQUIRE(knobslider->getValue() == 90.0f);
    }
}

TEST_CASE("Slider Callbacks", "[slider]") {
    UI ui("Test", 800, 600);
    
    SECTION("HSlider Callback Setting") {
        auto hslider = ui.createHSlider(10, 10, 200, 30, 0.0f, 100.0f, 50.0f);
        
        bool callbackSet = false;
        
        // Test that callback can be set without error
        hslider->setOnChange([&](float value) {
            callbackSet = true;
        });
        
        // Test that setValue works (callback execution is deferred)
        hslider->setValue(80.0f);
        REQUIRE(hslider->getValue() == 80.0f);
    }
    
    SECTION("VSlider Callback Setting") {
        auto vslider = ui.createVSlider(10, 10, 30, 200, 0.0f, 100.0f, 25.0f);
        
        // Test that callback can be set without error
        vslider->setOnChange([&](float value) {
            // Callback logic would execute during UI event processing
        });
        
        vslider->setValue(60.0f);
        REQUIRE(vslider->getValue() == 60.0f);
    }
    
    SECTION("KnobSlider Callback Setting") {
        auto knobslider = ui.createKnobSlider(10, 10, 60, 0.0f, 100.0f, 75.0f);
        
        // Test that callback can be set without error
        knobslider->setOnChange([&](float value) {
            // Callback logic would execute during UI event processing
        });
        
        knobslider->setValue(40.0f);
        REQUIRE(knobslider->getValue() == 40.0f);
    }
}

TEST_CASE("Slider Range Management", "[slider]") {
    UI ui("Test", 800, 600);
    
    SECTION("Range Setting") {
        auto hslider = ui.createHSlider(10, 10, 200, 30, 0.0f, 100.0f, 50.0f);
        
        // Change range
        hslider->setRange(10.0f, 90.0f);
        
        // Value should be clamped to new range
        hslider->setValue(5.0f);
        REQUIRE(hslider->getValue() == 10.0f);
        
        hslider->setValue(95.0f);
        REQUIRE(hslider->getValue() == 90.0f);
        
        hslider->setValue(50.0f);
        REQUIRE(hslider->getValue() == 50.0f);
    }
    
    SECTION("Zero Range Slider") {
        auto hslider = ui.createHSlider(10, 10, 200, 30, 50.0f, 50.0f, 50.0f);
        
        REQUIRE(hslider->getValue() == 50.0f);
        
        // Setting any value should clamp to the single valid value
        hslider->setValue(100.0f);
        REQUIRE(hslider->getValue() == 50.0f);
    }
    
    SECTION("Negative Range Slider") {
        auto vslider = ui.createVSlider(10, 10, 30, 200, -100.0f, -10.0f, -50.0f);
        
        REQUIRE(vslider->getValue() == -50.0f);
        
        vslider->setValue(-75.0f);
        REQUIRE(vslider->getValue() == -75.0f);
    }
}

TEST_CASE("Slider Focus and Interaction", "[slider]") {
    UI ui("Test", 800, 600);
    
    SECTION("Slider Focus Management") {
        auto hslider = ui.createHSlider(10, 10, 200, 30, 0.0f, 100.0f, 50.0f);
        auto vslider = ui.createVSlider(250, 10, 30, 200, 0.0f, 100.0f, 25.0f);
        
        // Test focus setting
        ui.setFocus(hslider);
        REQUIRE(ui.getFocusedElementId() == hslider->getId());
        
        ui.setFocus(vslider);
        REQUIRE(ui.getFocusedElementId() == vslider->getId());
    }
    
    SECTION("Multiple Sliders Independence") {
        auto hslider1 = ui.createHSlider(10, 10, 200, 30, 0.0f, 100.0f, 30.0f);
        auto hslider2 = ui.createHSlider(10, 50, 200, 30, 0.0f, 100.0f, 70.0f);
        
        // Verify initial values
        REQUIRE(hslider1->getValue() == 30.0f);
        REQUIRE(hslider2->getValue() == 70.0f);
        
        // Change one slider, verify the other is unaffected
        hslider1->setValue(80.0f);
        REQUIRE(hslider1->getValue() == 80.0f);
        REQUIRE(hslider2->getValue() == 70.0f);
        
        hslider2->setValue(20.0f);
        REQUIRE(hslider1->getValue() == 80.0f);
        REQUIRE(hslider2->getValue() == 20.0f);
    }
    
    SECTION("Slider Activation") {
        auto hslider = ui.createHSlider(10, 10, 200, 30, 0.0f, 100.0f, 50.0f);
        
        // Test that sliders are interactive
        REQUIRE(hslider->isInteractive() == true);
        
        // Test activation (should not crash)
        hslider->activate();
        REQUIRE(hslider->getValue() == 50.0f); // Value should remain unchanged
    }
}
