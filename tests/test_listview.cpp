#include <catch2/catch_test_macros.hpp>
#include "../src/UI.h"

TEST_CASE("ListView creation", "[listview]") {
    UI ui("Test Window", 800, 600);
    
    SECTION("ListView creates successfully") {
        std::vector<std::string> items = {"Item 1", "Item 2", "Item 3"};
        auto listview = ui.createListView(items, 10, 20, 200, 150);
        REQUIRE(listview != nullptr);
    }
    
    SECTION("ListView with empty items") {
        std::vector<std::string> empty_items;
        auto listview = ui.createListView(empty_items, 0, 0, 100, 100);
        REQUIRE(listview != nullptr);
    }
}

TEST_CASE("ListView with different sizes", "[listview]") {
    UI ui("Test Window", 800, 600);
    
    SECTION("Different ListView dimensions") {
        std::vector<std::string> items = {"A", "B", "C"};
        auto small = ui.createListView(items, 0, 0, 100, 80);
        auto large = ui.createListView(items, 0, 90, 300, 200);
        
        REQUIRE(small != nullptr);
        REQUIRE(large != nullptr);
        REQUIRE(small != large);
    }
}

TEST_CASE("Multiple ListViews", "[listview]") {
    UI ui("Test Window", 800, 600);
    
    SECTION("Creating multiple ListViews") {
        std::vector<std::string> items1 = {"One", "Two"};
        std::vector<std::string> items2 = {"Alpha", "Beta", "Gamma"};
        
        auto list1 = ui.createListView(items1, 0, 0, 150, 100);
        auto list2 = ui.createListView(items2, 160, 0, 150, 100);
        
        REQUIRE(list1 != nullptr);
        REQUIRE(list2 != nullptr);
        REQUIRE(list1 != list2);
    }
}
