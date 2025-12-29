#include <catch2/catch_test_macros.hpp>
#include "uiframework/UI.h"

TEST_CASE("VirtualKeyboard creation", "[virtualkeyboard]") {
    UI ui("Test Window", 800, 600);
    
    SECTION("VirtualKeyboard creates successfully") {
        bool callbackCalled = false;
        char receivedChar = '\0';
        
        auto keyboard = ui.createVirtualKeyboard(10, 20, 300, 100, [&](char c) {
            callbackCalled = true;
            receivedChar = c;
        });
        
        REQUIRE(keyboard != nullptr);
        REQUIRE(keyboard->getMode() == ui::KeyboardMode::LOWERCASE);
        REQUIRE(keyboard->getSelectedIndex() == 0);
    }
}

TEST_CASE("VirtualKeyboard mode cycling", "[virtualkeyboard]") {
    UI ui("Test Window", 800, 600);
    
    auto keyboard = ui.createVirtualKeyboard(10, 20, 300, 100, [](char c) {});
    
    SECTION("Mode cycles correctly") {
        REQUIRE(keyboard->getMode() == ui::KeyboardMode::LOWERCASE);
        
        keyboard->setMode(ui::KeyboardMode::UPPERCASE);
        REQUIRE(keyboard->getMode() == ui::KeyboardMode::UPPERCASE);
        
        keyboard->setMode(ui::KeyboardMode::NUMBERS);
        REQUIRE(keyboard->getMode() == ui::KeyboardMode::NUMBERS);
        
        keyboard->setMode(ui::KeyboardMode::SPECIAL);
        REQUIRE(keyboard->getMode() == ui::KeyboardMode::SPECIAL);
    }
}

TEST_CASE("VirtualKeyboard selection", "[virtualkeyboard]") {
    UI ui("Test Window", 800, 600);
    
    auto keyboard = ui.createVirtualKeyboard(10, 20, 300, 100, [](char c) {});
    
    SECTION("Selection index updates correctly") {
        REQUIRE(keyboard->getSelectedIndex() == 0);
        
        keyboard->setSelectedIndex(5);
        REQUIRE(keyboard->getSelectedIndex() == 5);
        
        keyboard->setSelectedIndex(25); // 'z' in lowercase mode
        REQUIRE(keyboard->getSelectedIndex() == 25);
    }
    
    SECTION("Invalid selection index is ignored") {
        keyboard->setSelectedIndex(-1);
        REQUIRE(keyboard->getSelectedIndex() == 0); // Should remain unchanged
        
        keyboard->setSelectedIndex(1000);
        REQUIRE(keyboard->getSelectedIndex() == 0); // Should remain unchanged
    }
}
