#include <catch2/catch_test_macros.hpp>
#include "uiframework/UI.h"

TEST_CASE("CycleList creation", "[cyclelist]") {
    UI ui("Test Window", 800, 600);
    
    SECTION("CycleList creates successfully") {
        std::vector<std::string> items = {"Item 1", "Item 2", "Item 3"};
        auto cycleList = ui.createCycleList(10, 20, 150, 30, items);
        
        REQUIRE(cycleList != nullptr);
        REQUIRE(cycleList->getSelectedIndex() == 0);
        REQUIRE(cycleList->getSelectedValue() == "Item 1");
    }
    
    SECTION("CycleList with empty items") {
        std::vector<std::string> emptyItems;
        auto cycleList = ui.createCycleList(0, 0, 100, 30, emptyItems);
        
        REQUIRE(cycleList != nullptr);
        REQUIRE(cycleList->getSelectedIndex() == 0);
        REQUIRE(cycleList->getSelectedValue() == "");
    }
    
    SECTION("CycleList with single item") {
        std::vector<std::string> singleItem = {"Only Item"};
        auto cycleList = ui.createCycleList(0, 0, 100, 30, singleItem);
        
        REQUIRE(cycleList != nullptr);
        REQUIRE(cycleList->getSelectedIndex() == 0);
        REQUIRE(cycleList->getSelectedValue() == "Only Item");
    }
}

TEST_CASE("CycleList navigation", "[cyclelist]") {
    UI ui("Test Window", 800, 600);
    std::vector<std::string> items = {"A", "B", "C", "D"};
    auto cycleList = ui.createCycleList(10, 20, 150, 30, items);
    
    SECTION("Forward navigation") {
        cycleList->setSelectedIndex(1);
        REQUIRE(cycleList->getSelectedIndex() == 1);
        REQUIRE(cycleList->getSelectedValue() == "B");
        
        cycleList->setSelectedIndex(3);
        REQUIRE(cycleList->getSelectedIndex() == 3);
        REQUIRE(cycleList->getSelectedValue() == "D");
    }
    
    SECTION("Index bounds checking") {
        cycleList->setSelectedIndex(-1);
        REQUIRE(cycleList->getSelectedIndex() == 0);
        
        cycleList->setSelectedIndex(10);
        REQUIRE(cycleList->getSelectedIndex() == 3);
    }
}

TEST_CASE("CycleList callback", "[cyclelist]") {
    UI ui("Test Window", 800, 600);
    std::vector<std::string> items = {"First", "Second", "Third"};
    auto cycleList = ui.createCycleList(10, 20, 150, 30, items);
    
    SECTION("Change callback fires") {
        int callbackIndex = -1;
        std::string callbackValue;
        
        cycleList->setOnChange([&](int index, const std::string& value) {
            callbackIndex = index;
            callbackValue = value;
        });
        
        cycleList->setSelectedIndex(2);
        REQUIRE(callbackIndex == 2);
        REQUIRE(callbackValue == "Third");
    }
}

TEST_CASE("CycleList items management", "[cyclelist]") {
    UI ui("Test Window", 800, 600);
    std::vector<std::string> items = {"A", "B"};
    auto cycleList = ui.createCycleList(10, 20, 150, 30, items);
    
    SECTION("Update items list") {
        std::vector<std::string> newItems = {"X", "Y", "Z"};
        cycleList->setItems(newItems);
        
        REQUIRE(cycleList->getSelectedIndex() == 0);
        REQUIRE(cycleList->getSelectedValue() == "X");
    }
    
    SECTION("Update to empty items") {
        std::vector<std::string> emptyItems;
        cycleList->setItems(emptyItems);
        
        REQUIRE(cycleList->getSelectedIndex() == 0);
        REQUIRE(cycleList->getSelectedValue() == "");
    }
}
