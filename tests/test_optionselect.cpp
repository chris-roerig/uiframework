#include <catch2/catch_test_macros.hpp>
#include "../src/UI.h"

TEST_CASE("OptionSelect creation", "[optionselect]") {
    UI ui("Test Window", 800, 600);
    
    SECTION("OptionSelect creates successfully") {
        std::vector<std::string> options = {"Option 1", "Option 2", "Option 3"};
        auto optionSelect = ui.createOptionSelect(1, options, 10, 20, [](int){});
        
        REQUIRE(optionSelect != nullptr);
        REQUIRE(optionSelect->getSelectedIndex() == 1);
        REQUIRE(optionSelect->getSelectedOption() == "Option 2");
    }
    
    SECTION("OptionSelect with empty options") {
        std::vector<std::string> options = {};
        auto optionSelect = ui.createOptionSelect(0, options, 0, 0, [](int){});
        
        REQUIRE(optionSelect != nullptr);
        REQUIRE(optionSelect->getSelectedIndex() == -1);
        REQUIRE(optionSelect->getSelectedOption() == "");
    }
    
    SECTION("OptionSelect with invalid initial index") {
        std::vector<std::string> options = {"A", "B"};
        auto optionSelect = ui.createOptionSelect(5, options, 0, 0, [](int){});
        
        REQUIRE(optionSelect != nullptr);
        REQUIRE(optionSelect->getSelectedIndex() == 0); // Should default to 0
    }
}

TEST_CASE("OptionSelect selection operations", "[optionselect]") {
    UI ui("Test Window", 800, 600);
    
    SECTION("setSelectedIndex and getSelectedIndex") {
        std::vector<std::string> options = {"First", "Second", "Third"};
        auto optionSelect = ui.createOptionSelect(0, options, 10, 10, [](int){});
        
        optionSelect->setSelectedIndex(2);
        REQUIRE(optionSelect->getSelectedIndex() == 2);
        REQUIRE(optionSelect->getSelectedOption() == "Third");
        
        // Invalid index should be ignored
        optionSelect->setSelectedIndex(10);
        REQUIRE(optionSelect->getSelectedIndex() == 2);
    }
    
    SECTION("Callback functionality") {
        std::vector<std::string> options = {"A", "B", "C"};
        int callbackIndex = -1;
        
        auto optionSelect = ui.createOptionSelect(0, options, 10, 10, [&callbackIndex](int index) {
            callbackIndex = index;
        });
        
        optionSelect->setSelectedIndex(1);
        REQUIRE(callbackIndex == 1);
    }
}

TEST_CASE("OptionSelect option management", "[optionselect]") {
    UI ui("Test Window", 800, 600);
    
    SECTION("addOption") {
        std::vector<std::string> options = {"Initial"};
        auto optionSelect = ui.createOptionSelect(0, options, 10, 10, [](int){});
        
        optionSelect->addOption("Added");
        REQUIRE(optionSelect->getSelectedIndex() == 0);
        
        // Test adding to empty list
        optionSelect->clearOptions();
        REQUIRE(optionSelect->getSelectedIndex() == -1);
        
        optionSelect->addOption("First");
        REQUIRE(optionSelect->getSelectedIndex() == 0);
    }
    
    SECTION("removeOption") {
        std::vector<std::string> options = {"A", "B", "C", "D"};
        auto optionSelect = ui.createOptionSelect(2, options, 10, 10, [](int){});
        
        // Remove option before current selection
        optionSelect->removeOption(0);
        REQUIRE(optionSelect->getSelectedIndex() == 1); // Should shift down
        
        // Remove current selection (index 1, which is now "C")
        optionSelect->removeOption(1);
        REQUIRE(optionSelect->getSelectedIndex() == 1); // Should stay at index 1 (now "D")
    }
    
    SECTION("setOptions") {
        std::vector<std::string> initial = {"Old1", "Old2"};
        auto optionSelect = ui.createOptionSelect(1, initial, 10, 10, [](int){});
        
        std::vector<std::string> newOptions = {"New1", "New2", "New3"};
        optionSelect->setOptions(newOptions);
        
        REQUIRE(optionSelect->getSelectedIndex() == 1); // Should preserve if valid
        REQUIRE(optionSelect->getSelectedOption() == "New2");
        
        // Test with smaller list
        std::vector<std::string> smaller = {"Only"};
        optionSelect->setOptions(smaller);
        REQUIRE(optionSelect->getSelectedIndex() == 0); // Should adjust
    }
    
    SECTION("clearOptions") {
        std::vector<std::string> options = {"A", "B"};
        auto optionSelect = ui.createOptionSelect(1, options, 10, 10, [](int){});
        
        optionSelect->clearOptions();
        REQUIRE(optionSelect->getSelectedIndex() == -1);
        REQUIRE(optionSelect->getSelectedOption() == "");
    }
}

TEST_CASE("OptionSelect state management", "[optionselect]") {
    UI ui("Test Window", 800, 600);
    
    SECTION("Expand and collapse") {
        std::vector<std::string> options = {"A", "B"};
        auto optionSelect = ui.createOptionSelect(0, options, 10, 10, [](int){});
        
        REQUIRE_FALSE(optionSelect->isExpanded());
        
        optionSelect->expand();
        REQUIRE(optionSelect->isExpanded());
        
        optionSelect->collapse();
        REQUIRE_FALSE(optionSelect->isExpanded());
    }
    
    SECTION("Focus loss collapses dropdown") {
        std::vector<std::string> options = {"A", "B"};
        auto optionSelect = ui.createOptionSelect(0, options, 10, 10, [](int){});
        
        optionSelect->expand();
        REQUIRE(optionSelect->isExpanded());
        
        optionSelect->onFocusLost();
        REQUIRE_FALSE(optionSelect->isExpanded());
        REQUIRE_FALSE(optionSelect->hasFocus);
    }
}
