#include "uiframework/UI.h"
#include <iostream>
#include <vector>
#include <string>

int main() {
    try {
        UI ui("Complete UI Framework Demo - All Widgets", 1200, 800);
        
        // Title
        auto titleLabel = ui.createLabel("UI Framework - Complete Widget Showcase", 20, 10);
        
        // Column 1: Basic Widgets
        auto col1Label = ui.createLabel("Basic Widgets:", 20, 50);
        
        auto button1 = ui.createButton("Standard Button", 20, 80, []() {
            std::cout << "Standard button clicked!" << std::endl;
        });
        
        auto button2 = ui.createButton("Action Button", 150, 80, []() {
            std::cout << "Action performed!" << std::endl;
        });
        
        auto textBox = ui.createTextBox("Type here...", 20, 120);
        
        bool check1State = false, check2State = true;
        auto checkbox1 = ui.createCheckBox(check1State, 20, 160, [&](bool state) {
            check1State = state;
            std::cout << "Checkbox 1: " << (state ? "checked" : "unchecked") << std::endl;
        });
        auto checkLabel1 = ui.createLabel("Enable feature A", 45, 165);
        
        auto checkbox2 = ui.createCheckBox(check2State, 20, 190, [&](bool state) {
            check2State = state;
            std::cout << "Checkbox 2: " << (state ? "checked" : "unchecked") << std::endl;
        });
        auto checkLabel2 = ui.createLabel("Enable feature B", 45, 195);
        
        // Option selector
        std::vector<std::string> options = {"Option 1", "Option 2", "Option 3", "Option 4"};
        auto optionSelect = ui.createOptionSelect(0, options, 20, 230, [](int index) {
            std::cout << "Selected option: " << index << std::endl;
        });
        
        // Column 2: Visual Widgets
        auto col2Label = ui.createLabel("Visual Widgets:", 350, 50);
        
        auto progressBar1 = ui.createProgressBar(350, 80, 200, 25, 0.25f, true);
        auto progressBar2 = ui.createProgressBar(350, 115, 200, 25, 0.65f, true);
        auto progressBar3 = ui.createProgressBar(350, 150, 200, 25, 1.0f, true);
        
        std::vector<std::string> listItems = {
            "List Item 1", "List Item 2", "List Item 3", 
            "List Item 4", "List Item 5", "List Item 6"
        };
        auto listView = ui.createListView(listItems, 350, 185, 200, 120);
        
        auto canvas = ui.createCanvas(350, 315, 200, 100);
        
        // Column 3: Interactive Widgets
        auto col3Label = ui.createLabel("Interactive Widgets:", 650, 50);
        
        auto infoModalBtn = ui.createButton("Info Modal", 650, 80, [&]() {
            ui.createInfoModal("This is an information modal dialog!");
        });
        
        auto confirmModalBtn = ui.createButton("Confirm Modal", 650, 120, [&]() {
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
            }},
            {"View", {
                {"Zoom In", []() { std::cout << "Zoom In" << std::endl; }},
                {"Zoom Out", []() { std::cout << "Zoom Out" << std::endl; }}
            }}
        };
        auto contextMenu = ui.createContextMenu(menuItems);
        
        // Theme Controls
        auto themeLabel = ui.createLabel("Themes:", 650, 180);
        auto themeBtn1 = ui.createButton("Default", 650, 210, [&]() { 
            ui.setTheme("Default"); 
            std::cout << "Theme: Default" << std::endl;
        });
        auto themeBtn2 = ui.createButton("Dark", 750, 210, [&]() { 
            ui.setTheme("SolarizedDark"); 
            std::cout << "Theme: SolarizedDark" << std::endl;
        });
        auto themeBtn3 = ui.createButton("Light", 650, 250, [&]() { 
            ui.setTheme("SolarizedLight"); 
            std::cout << "Theme: SolarizedLight" << std::endl;
        });
        auto themeBtn4 = ui.createButton("Molokai", 750, 250, [&]() { 
            ui.setTheme("Molokai"); 
            std::cout << "Theme: Molokai" << std::endl;
        });
        
        // Additional Controls
        auto moreLabel = ui.createLabel("More Controls:", 950, 50);
        auto btn3 = ui.createButton("Button 3", 950, 80, []() { std::cout << "Button 3!" << std::endl; });
        auto btn4 = ui.createButton("Button 4", 950, 120, []() { std::cout << "Button 4!" << std::endl; });
        auto btn5 = ui.createButton("Button 5", 950, 160, []() { std::cout << "Button 5!" << std::endl; });
        
        // Instructions
        auto instructionLabel1 = ui.createLabel("Instructions:", 20, 450);
        auto instructionLabel2 = ui.createLabel("• Click buttons and interact with all widgets", 20, 470);
        auto instructionLabel3 = ui.createLabel("• Type in text box, check checkboxes, select options", 20, 490);
        auto instructionLabel4 = ui.createLabel("• Right-click anywhere for context menu", 20, 510);
        auto instructionLabel5 = ui.createLabel("• Try different theme buttons to change appearance", 20, 530);
        auto instructionLabel6 = ui.createLabel("• Use hotkeys: 1=Button1, 2=Button2, i=Info, c=Confirm", 20, 550);
        auto instructionLabel7 = ui.createLabel("• Close window to exit", 20, 570);
        
        // Status
        auto statusLabel = ui.createLabel("Status: All widgets loaded successfully!", 20, 620);
        
        // Hotkeys
        ui.assignHotKey(button1, "1");
        ui.assignHotKey(button2, "2");
        ui.assignHotKey(infoModalBtn, "i");
        ui.assignHotKey(confirmModalBtn, "c");
        
        // Focus on first interactive element
        ui.setFocus(button1);
        
        std::cout << "=== UI Framework Complete Demo ===" << std::endl;
        std::cout << "All widgets loaded successfully!" << std::endl;
        std::cout << "Widgets included:" << std::endl;
        std::cout << "  ✓ Labels (text display)" << std::endl;
        std::cout << "  ✓ Buttons (interactive with callbacks)" << std::endl;
        std::cout << "  ✓ TextBox (text input)" << std::endl;
        std::cout << "  ✓ CheckBoxes (boolean toggles)" << std::endl;
        std::cout << "  ✓ OptionSelect (dropdown menu)" << std::endl;
        std::cout << "  ✓ ProgressBars (visual indicators)" << std::endl;
        std::cout << "  ✓ ListView (scrollable lists)" << std::endl;
        std::cout << "  ✓ Canvas (custom drawing)" << std::endl;
        std::cout << "  ✓ Modal dialogs (popups)" << std::endl;
        std::cout << "  ✓ Context menus (right-click)" << std::endl;
        std::cout << "  ✓ Theme system (4 themes)" << std::endl;
        std::cout << "" << std::endl;
        std::cout << "Hotkeys: 1=Button1, 2=Button2, i=Info Modal, c=Confirm Modal" << std::endl;
        std::cout << "Interact with all widgets and close window to exit." << std::endl;
        
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
