#include "../src/UI.h"
#include <iostream>
#include <vector>

int main() {
    try {
        UI ui("Progressive Widget Test", 800, 600);
        
        std::cout << "=== WIDGET TEST 1: Label ===" << std::endl;
        auto label = ui.createLabel("Test Label", 10, 10);
        std::cout << "Label created successfully" << std::endl;
        
        std::cout << "=== WIDGET TEST 2: Button ===" << std::endl;
        auto button = ui.createButton("Test Button", 10, 40, []() {
            std::cout << "Button clicked!" << std::endl;
        });
        std::cout << "Button created successfully" << std::endl;
        
        std::cout << "=== WIDGET TEST 3: CheckBox ===" << std::endl;
        bool checkState = false;
        auto checkbox = ui.createCheckBox(checkState, 10, 80, [&](bool state) {
            checkState = state;
            std::cout << "Checkbox: " << (state ? "checked" : "unchecked") << std::endl;
        });
        std::cout << "CheckBox created successfully" << std::endl;
        
        std::cout << "=== WIDGET TEST 4: ProgressBar ===" << std::endl;
        auto progressBar = ui.createProgressBar(10, 120, 200, 25, 0.5f, true);
        std::cout << "ProgressBar created successfully" << std::endl;
        
        std::cout << "=== WIDGET TEST 5: Canvas ===" << std::endl;
        auto canvas = ui.createCanvas(10, 160, 200, 100);
        std::cout << "Canvas created successfully" << std::endl;
        
        std::cout << "=== WIDGET TEST 6: TextBox (FIXED) ===" << std::endl;
        auto textBox = ui.createTextBox("Type here...", 10, 280);
        std::cout << "TextBox created successfully" << std::endl;
        
        std::cout << "=== WIDGET TEST 7: OptionSelect (FIXED) ===" << std::endl;
        std::vector<std::string> options = {"Option 1", "Option 2", "Option 3"};
        auto optionSelect = ui.createOptionSelect(0, options, 10, 320, [](int index) {
            std::cout << "Selected: " << index << std::endl;
        });
        std::cout << "OptionSelect created successfully" << std::endl;
        
        ui.run();
        
    } catch (const std::exception& e) {
        std::cerr << "ERROR at Label: " << e.what() << std::endl;
        return -1;
    }
    
    return 0;
}
