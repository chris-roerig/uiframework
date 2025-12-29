#include "uiframework/UI.h"
#include <iostream>

int main() {
    try {
        UI ui("Working Demo - All Widgets", 800, 600);
        
        // Create a simple button that works
        auto button = ui.createButton("Click Me!", 50, 50, []() {
            std::cout << "Button clicked!" << std::endl;
        });
        
        // Create other widgets
        auto label = ui.createLabel("UI Framework Demo", 50, 20);
        auto textBox = ui.createTextBox("Type here...", 50, 100);
        
        bool checkState = false;
        auto checkbox = ui.createCheckBox(checkState, 50, 140, [&](bool state) {
            checkState = state;
            std::cout << "Checkbox: " << (state ? "checked" : "unchecked") << std::endl;
        });
        auto checkLabel = ui.createLabel("Enable feature", 75, 145);
        
        // Progress bar
        auto progress = ui.createProgressBar(50, 180, 200, 25, 0.7f, true);
        
        // List view
        std::vector<std::string> items = {"Item 1", "Item 2", "Item 3", "Item 4"};
        auto listView = ui.createListView(items, 300, 50, 200, 120);
        
        // Canvas
        auto canvas = ui.createCanvas(300, 200, 200, 100);
        
        // Modal buttons
        auto infoBtn = ui.createButton("Info", 50, 220, [&]() {
            ui.createInfoModal("This is an info message!");
        });
        
        auto confirmBtn = ui.createButton("Confirm", 150, 220, [&]() {
            ui.createConfirmModal("Are you sure?",
                []() { std::cout << "Confirmed!" << std::endl; },
                []() { std::cout << "Cancelled!" << std::endl; }
            );
        });
        
        // Context menu
        std::vector<ui::TopMenuItem> menuItems = {
            {"File", {
                {"New", []() { std::cout << "New" << std::endl; }},
                {"Open", []() { std::cout << "Open" << std::endl; }},
                {"Save", []() { std::cout << "Save" << std::endl; }}
            }}
        };
        auto contextMenu = ui.createContextMenu(menuItems);
        
        // Instructions
        auto instructions = ui.createLabel("Click button, type in textbox, check checkbox, right-click for menu", 50, 350);
        auto instructions2 = ui.createLabel("Press Escape or close window to exit", 50, 370);
        
        std::cout << "Demo ready! Interact with the widgets." << std::endl;
        std::cout << "Close the window or press Escape to exit." << std::endl;
        
        ui.run();
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    
    return 0;
}
