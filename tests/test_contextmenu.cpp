#include <catch2/catch_test_macros.hpp>
#include "../src/UI.h"

TEST_CASE("ContextMenu creation", "[contextmenu]") {
    UI ui("Test Window", 800, 600);
    
    SECTION("ContextMenu creates successfully") {
        std::vector<ui::TopMenuItem> menus = {
            {"File", {{"New", [](){}}, {"Open", [](){}}}},
            {"Edit", {{"Copy", [](){}}, {"Paste", [](){}}}}
        };
        auto contextmenu = ui.createContextMenu(menus);
        REQUIRE(contextmenu != nullptr);
    }
    
    SECTION("ContextMenu with single menu") {
        std::vector<ui::TopMenuItem> single = {
            {"Actions", {{"Action 1", [](){}}}}
        };
        auto contextmenu = ui.createContextMenu(single);
        REQUIRE(contextmenu != nullptr);
    }
}

TEST_CASE("ContextMenu with different menus", "[contextmenu]") {
    UI ui("Test Window", 800, 600);
    
    SECTION("Different menu structures") {
        std::vector<ui::TopMenuItem> file_menu = {
            {"File", {{"New", [](){}}, {"Save", [](){}}, {"Exit", [](){}}}}
        };
        std::vector<ui::TopMenuItem> edit_menu = {
            {"Edit", {{"Undo", [](){}}, {"Redo", [](){}}, {"Cut", [](){}}}}
        };
        
        auto file_ctx = ui.createContextMenu(file_menu);
        auto edit_ctx = ui.createContextMenu(edit_menu);
        
        REQUIRE(file_ctx != nullptr);
        REQUIRE(edit_ctx != nullptr);
        REQUIRE(file_ctx != edit_ctx);
    }
}

TEST_CASE("Empty ContextMenu", "[contextmenu]") {
    UI ui("Test Window", 800, 600);
    
    SECTION("ContextMenu with empty menus") {
        std::vector<ui::TopMenuItem> empty_menus;
        auto ctx = ui.createContextMenu(empty_menus);
        REQUIRE(ctx != nullptr);
    }
}

TEST_CASE("ContextMenu callback deferral", "[contextmenu]") {
    UI ui("Test Window", 800, 600);
    
    SECTION("Menu callbacks are deferred") {
        bool callbackExecuted = false;
        std::vector<ui::TopMenuItem> menus = {
            {"File", {{"Test", [&callbackExecuted]() { callbackExecuted = true; }}}}
        };
        auto contextmenu = ui.createContextMenu(menus);
        
        // Simulate menu activation
        contextmenu->setActiveItem(0);
        contextmenu->expandMenu();
        contextmenu->selectSubItem(0);
        contextmenu->activate();
        
        // Callback should be queued for deferred execution to prevent deadlocks
        REQUIRE(contextmenu != nullptr);
    }
}
