#include "../src/UI.h"
#include <iostream>
#include <vector>
#include <string>

int main() {
    try {
        UI ui("Simple UI Demo - Core Widgets", 1000, 700);
        
        // Theme selector
        std::vector<std::string> themes = {"Default", "SolarizedDark", "SolarizedLight", "Molokai"};
        int currentTheme = 0;
        auto themeSelect = ui.createOptionSelect(currentTheme, themes, 10, 10, [&](int index) {
            ui.setTheme(themes[index]);
            currentTheme = index;
        });
        
        // Labels
        auto titleLabel = ui.createLabel("UI Framework Widget Showcase", 200, 10);
        auto infoLabel = ui.createLabel("Theme:", 10, 50);
        
        // Buttons
        auto button1 = ui.createButton("Standard Button", 10, 90, []() {
            std::cout << "Standard button clicked!" << std::endl;
        });
        
        auto button2 = ui.createButton("Action Button", 150, 90, []() {
            std::cout << "Action performed!" << std::endl;
        });
        
        // Text input
        auto textBox = ui.createTextBox("Enter text here...", 10, 130);
        
        // Checkboxes
        bool check1State = false, check2State = true;
        auto checkbox1 = ui.createCheckBox(check1State, 10, 170, [&](bool state) {
            check1State = state;
            std::cout << "Checkbox 1: " << (state ? "checked" : "unchecked") << std::endl;
        });
        auto checkLabel1 = ui.createLabel("Enable feature A", 35, 175);
        
        auto checkbox2 = ui.createCheckBox(check2State, 10, 200, [&](bool state) {
            check2State = state;
            std::cout << "Checkbox 2: " << (state ? "checked" : "unchecked") << std::endl;
        });
        auto checkLabel2 = ui.createLabel("Enable feature B", 35, 205);
        
        // Option selector
        std::vector<std::string> options = {"Option 1", "Option 2", "Option 3", "Option 4"};
        auto optionSelect = ui.createOptionSelect(0, options, 10, 240, [](int index) {
            std::cout << "Selected option: " << index << std::endl;
        });
        
        // Progress bar
        auto progressBar = ui.createProgressBar(10, 280, 200, 25, 0.65f, true);
        
        // List view
        std::vector<std::string> listItems = {
            "List Item 1", "List Item 2", "List Item 3", 
            "List Item 4", "List Item 5", "List Item 6"
        };
        auto listView = ui.createListView(listItems, 300, 90, 200, 150);
        
        // Canvas for custom drawing
        auto canvas = ui.createCanvas(300, 250, 200, 100);
        
        // Modal buttons
        auto infoModalBtn = ui.createButton("Info Modal", 520, 90, [&]() {
            ui.createInfoModal("This is an information modal dialog.");
        });
        
        auto confirmModalBtn = ui.createButton("Confirm Modal", 520, 130, [&]() {
            ui.createConfirmModal("Are you sure you want to proceed?",
                []() { std::cout << "Confirmed!" << std::endl; },
                []() { std::cout << "Cancelled!" << std::endl; }
            );
        });
        
        // Context menu
        std::vector<ui::TopMenuItem> menuItems = {
            {"File", {
                {"New", []() { std::cout << "New file" << std::endl; }},
                {"Open", []() { std::cout << "Open file" << std::endl; }},
                {"Save", []() { std::cout << "Save file" << std::endl; }}
            }},
            {"Edit", {
                {"Cut", []() { std::cout << "Cut" << std::endl; }},
                {"Copy", []() { std::cout << "Copy" << std::endl; }},
                {"Paste", []() { std::cout << "Paste" << std::endl; }}
            }}
        };
        auto contextMenu = ui.createContextMenu(menuItems);
        
        // Additional widgets in right column
        auto moreButtons = ui.createLabel("More Controls:", 650, 90);
        auto btn3 = ui.createButton("Button 3", 650, 120, []() { std::cout << "Button 3!" << std::endl; });
        auto btn4 = ui.createButton("Button 4", 650, 160, []() { std::cout << "Button 4!" << std::endl; });
        auto btn5 = ui.createButton("Button 5", 650, 200, []() { std::cout << "Button 5!" << std::endl; });
        
        // Progress bars with different values
        auto progress1 = ui.createProgressBar(650, 240, 150, 20, 0.25f, true);
        auto progress2 = ui.createProgressBar(650, 270, 150, 20, 0.75f, true);
        auto progress3 = ui.createProgressBar(650, 300, 150, 20, 1.0f, true);
        
        // Instructions
        auto instructionLabel1 = ui.createLabel("Instructions:", 10, 400);
        auto instructionLabel2 = ui.createLabel("• Use Tab/Shift+Tab to navigate", 10, 420);
        auto instructionLabel3 = ui.createLabel("• Press Enter/Space on focused buttons", 10, 440);
        auto instructionLabel4 = ui.createLabel("• Right-click for context menu", 10, 460);
        auto instructionLabel5 = ui.createLabel("• Try different themes with dropdown", 10, 480);
        auto instructionLabel6 = ui.createLabel("• Type in text box and check checkboxes", 10, 500);
        
        // Hotkeys
        ui.assignHotKey(button1, "1");
        ui.assignHotKey(button2, "2");
        ui.assignHotKey(infoModalBtn, "i");
        ui.assignHotKey(confirmModalBtn, "c");
        
        // Focus on first interactive element
        ui.setFocus(themeSelect);
        
        std::cout << "Demo started! Try interacting with all the widgets." << std::endl;
        std::cout << "Hotkeys: 1=Button1, 2=Button2, i=Info Modal, c=Confirm Modal" << std::endl;
        
        ui.run();
        
    } catch (const ui::UIException& e) {
        std::cerr << "UI Error: " << e.what() << std::endl;
        return -1;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    
    return 0;
}
