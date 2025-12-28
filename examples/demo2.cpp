#include "../src/UI.h"
#include <iostream>

int main() {
    try {
        UI ui("Demo2 - Layout & Grid Showcase", 1000, 700);
        ui.setTheme("SolarizedDark");

        // Main horizontal layout
        auto mainLayout = ui.createHBoxLayout(10, 10, 980, 680);
        
        // Left panel - vertical layout with controls
        auto leftPanel = ui.createVBoxLayout(0, 0, 300, 680);
        
        // Control section
        auto titleLabel = ui.createLabel("UI Controls", 0, 0);
        leftPanel->addElement(titleLabel);
        
        auto button1 = ui.createButton("Action Button", 0, 0, []() {
            std::cout << "Action executed!" << std::endl;
        });
        leftPanel->addElement(button1);
        
        auto checkbox = ui.createCheckBox(false, 0, 0, [](bool checked) {
            std::cout << "Checkbox: " << (checked ? "ON" : "OFF") << std::endl;
        });
        leftPanel->addElement(checkbox);
        
        auto progress = ui.createProgressBar(0, 0, 250, 20);
        progress->setProgress(0.7f);
        leftPanel->addElement(progress);
        
        auto slider = ui.createHSlider(0, 0, 200, 20, 0.0f, 100.0f, 50.0f);
        slider->setOnChange([](float value) {
            std::cout << "Slider: " << value << std::endl;
        });
        leftPanel->addElement(slider);
        
        // Right panel - grid layout showcase
        auto rightPanel = ui.createGridLayout(0, 0, 650, 680, 4, 4);
        
        // Fill grid with various widgets using unified API
        auto gridTitle = ui.createLabel("4x4 Grid Layout", 0, 0);
        rightPanel->addElement(gridTitle, 0, 0, 2, 1); // Span 2 columns
        
        // Row 1 (after title)
        auto btn1 = ui.createButton("Grid 1", 0, 0, []() { std::cout << "Grid button 1" << std::endl; });
        auto btn2 = ui.createButton("Grid 2", 0, 0, []() { std::cout << "Grid button 2" << std::endl; });
        rightPanel->addElement(btn1, 2, 0);
        rightPanel->addElement(btn2, 3, 0);
        
        // Row 2
        auto label1 = ui.createLabel("Cell A", 0, 0);
        auto label2 = ui.createLabel("Cell B", 0, 0);
        auto label3 = ui.createLabel("Cell C", 0, 0);
        auto label4 = ui.createLabel("Cell D", 0, 0);
        rightPanel->addElement(label1, 0, 1);
        rightPanel->addElement(label2, 1, 1);
        rightPanel->addElement(label3, 2, 1);
        rightPanel->addElement(label4, 3, 1);
        
        // Row 3
        auto cb1 = ui.createCheckBox(true, 0, 0, [](bool) {});
        auto cb2 = ui.createCheckBox(false, 0, 0, [](bool) {});
        rightPanel->addElement(cb1, 0, 2);
        rightPanel->addElement(cb2, 1, 2);
        
        auto vslider = ui.createVSlider(0, 0, 20, 80, 0.0f, 10.0f, 5.0f);
        rightPanel->addElement(vslider, 2, 2);
        
        auto knob = ui.createKnobSlider(0, 0, 50, 0.0f, 360.0f, 180.0f);
        rightPanel->addElement(knob, 3, 2);
        
        // Row 4
        auto prog1 = ui.createProgressBar(0, 0, 120, 15);
        prog1->setProgress(0.3f);
        auto prog2 = ui.createProgressBar(0, 0, 120, 15);
        prog2->setProgress(0.8f);
        rightPanel->addElement(prog1, 0, 3, 2, 1); // Span 2 columns
        rightPanel->addElement(prog2, 2, 3, 2, 1); // Span 2 columns
        
        // Add panels to main layout
        mainLayout->addElement(leftPanel);
        mainLayout->addElement(rightPanel);
        
        std::cout << "Demo2 started - showcasing layout and grid system" << std::endl;
        std::cout << "Left panel: Vertical layout with controls" << std::endl;
        std::cout << "Right panel: 4x4 grid with various widgets" << std::endl;
        std::cout << "Grid uses unified API - no casting required!" << std::endl;
        
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
