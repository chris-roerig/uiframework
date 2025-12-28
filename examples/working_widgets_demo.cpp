#include "../src/UI.h"
#include <iostream>
#include <vector>

int main() {
    try {
        UI ui("Working Widgets Demo", 800, 600);
        
        // Only include widgets we know work from progressive testing
        auto titleLabel = ui.createLabel("UI Framework - Working Widgets Only", 20, 10);
        
        auto button1 = ui.createButton("Button 1", 20, 50, []() {
            std::cout << "Button 1 clicked!" << std::endl;
        });
        
        auto button2 = ui.createButton("Button 2", 150, 50, []() {
            std::cout << "Button 2 clicked!" << std::endl;
        });
        
        bool checkState = false;
        auto checkbox = ui.createCheckBox(checkState, 20, 90, [&](bool state) {
            checkState = state;
            std::cout << "Checkbox: " << (state ? "checked" : "unchecked") << std::endl;
        });
        auto checkLabel = ui.createLabel("Enable feature", 45, 95);
        
        auto progressBar = ui.createProgressBar(20, 130, 200, 25, 0.7f, true);
        auto canvas = ui.createCanvas(20, 170, 200, 100);
        
        // Theme buttons
        auto themeLabel = ui.createLabel("Themes:", 300, 50);
        auto themeBtn1 = ui.createButton("Default", 300, 80, [&]() { ui.setTheme("Default"); });
        auto themeBtn2 = ui.createButton("Dark", 400, 80, [&]() { ui.setTheme("SolarizedDark"); });
        
        // Instructions
        auto instructions = ui.createLabel("All widgets working! Click buttons, check checkbox, try themes.", 20, 300);
        auto instructions2 = ui.createLabel("Close window to exit.", 20, 320);
        
        ui.assignHotKey(button1, "1");
        ui.assignHotKey(button2, "2");
        
        std::cout << "Working widgets demo loaded successfully!" << std::endl;
        std::cout << "Widgets: Label, Button, CheckBox, ProgressBar, Canvas" << std::endl;
        std::cout << "Hotkeys: 1=Button1, 2=Button2" << std::endl;
        
        ui.run();
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    
    return 0;
}
