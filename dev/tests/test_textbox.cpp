#include <catch2/catch_test_macros.hpp>
#include "uiframework/UI.h"

TEST_CASE("TextBox creation", "[textbox]") {
    UI ui("Test Window", 800, 600);
    
    SECTION("TextBox creates successfully") {
        auto textBox = ui.createTextBox("Default text", 10, 20);
        
        REQUIRE(textBox != nullptr);
        REQUIRE(textBox->getText() == "Default text");
    }
    
    SECTION("TextBox with empty text") {
        auto textBox = ui.createTextBox("", 0, 0);
        REQUIRE(textBox != nullptr);
        REQUIRE(textBox->getText() == "");
    }
}

TEST_CASE("TextBox text operations", "[textbox]") {
    UI ui("Test Window", 800, 600);
    
    SECTION("setText and getText") {
        auto textBox = ui.createTextBox("Initial", 10, 10);
        
        textBox->setText("New text");
        REQUIRE(textBox->getText() == "New text");
        
        textBox->clear();
        REQUIRE(textBox->getText() == "");
    }
    
    SECTION("Selection operations") {
        auto textBox = ui.createTextBox("Test text", 10, 10);
        
        REQUIRE_FALSE(textBox->hasSelection());
        
        textBox->selectAll();
        REQUIRE(textBox->hasSelection());
        
        textBox->clearSelection();
        REQUIRE_FALSE(textBox->hasSelection());
    }
}

TEST_CASE("TextBox focus behavior", "[textbox]") {
    UI ui("Test Window", 800, 600);
    
    SECTION("Auto-highlight on focus") {
        auto textBox = ui.createTextBox("Test", 10, 10, true);
        
        textBox->onFocusGained();
        REQUIRE(textBox->hasSelection());
        
        textBox->onFocusLost();
        REQUIRE_FALSE(textBox->hasSelection());
    }
    
    SECTION("No auto-highlight") {
        auto textBox = ui.createTextBox("Test", 10, 10, false);
        
        textBox->onFocusGained();
        REQUIRE_FALSE(textBox->hasSelection());
    }
}

TEST_CASE("TextBox text input handling", "[textbox]") {
    UI ui("Test Window", 800, 600);
    
    SECTION("Text input with selected text") {
        auto textBox = ui.createTextBox("Original text", 10, 10);
        textBox->onFocusGained();
        textBox->selectAll();
        
        // Simulate text input event
        SDL_Event e;
        e.type = SDL_TEXTINPUT;
        strcpy(e.text.text, "New");
        
        textBox->handleEvent(e);
        
        REQUIRE(textBox->getText() == "New");
        REQUIRE_FALSE(textBox->hasSelection());
    }
    
    SECTION("Text input without selection") {
        auto textBox = ui.createTextBox("Test", 10, 10);
        textBox->onFocusGained();
        textBox->clearSelection();
        
        // Simulate text input event
        SDL_Event e;
        e.type = SDL_TEXTINPUT;
        strcpy(e.text.text, "!");
        
        textBox->handleEvent(e);
        
        REQUIRE(textBox->getText() == "Test!");
    }
}
