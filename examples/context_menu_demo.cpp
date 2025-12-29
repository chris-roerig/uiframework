#include "uiframework/UI.h"
#include <iostream>

int main() {
    try {
        UI ui("Complete UI Demo - All Elements", 1200, 800);
        ui.setTheme("SolarizedDark");

        // Create context menu (menu bar)
        std::vector<ui::TopMenuItem> menuItems = {
            {"File", {
                {"New File", []() { std::cout << "New File selected" << std::endl; }},
                {"Open File", []() { std::cout << "Open File selected" << std::endl; }},
                {"Save", []() { std::cout << "Save selected" << std::endl; }}
            }},
            {"Edit", {
                {"Undo", []() { std::cout << "Undo selected" << std::endl; }},
                {"Copy", []() { std::cout << "Copy selected" << std::endl; }},
                {"Paste", []() { std::cout << "Paste selected" << std::endl; }}
            }},
            {"View", {
                {"Zoom In", []() { std::cout << "Zoom In selected" << std::endl; }},
                {"Zoom Out", []() { std::cout << "Zoom Out selected" << std::endl; }}
            }}
        };
        
        auto contextMenu = ui.createContextMenu(menuItems);

        // Create main 3x4 grid layout with borders
        auto mainGrid = ui.createGridLayout(20, 80, 1160, 700, 3, 4);
        mainGrid->setBorderWidth(2);

        // Row 1: Basic Input Elements
        auto basicLabel = ui.createLabel("Basic Inputs", 0, 0);
        mainGrid->addElement(basicLabel, 0, 0);
        
        auto button = ui.createButton("Click Me", 0, 0, []() {
            std::cout << "Button clicked!" << std::endl;
        });
        mainGrid->addElement(button, 0, 1);
        
        auto textBox = ui.createTextBox("Type here...", 0, 0);
        mainGrid->addElement(textBox, 0, 2);
        
        auto checkbox = ui.createCheckBox(false, 0, 0, [](bool checked) {
            std::cout << "Checkbox: " << (checked ? "ON" : "OFF") << std::endl;
        });
        mainGrid->addElement(checkbox, 0, 3);

        // Row 2: Progress and Sliders
        auto progressLabel = ui.createLabel("Progress & Sliders", 0, 0);
        mainGrid->addElement(progressLabel, 1, 0);
        
        auto progressBar = ui.createProgressBar(0, 0, 250, 25, 0.6f, true);
        mainGrid->addElement(progressBar, 1, 1);
        
        auto hSlider = ui.createHSlider(0, 0, 200, 25, 0.0f, 100.0f, 50.0f);
        hSlider->setOnChange([](float value) {
            std::cout << "H-Slider: " << value << std::endl;
        });
        mainGrid->addElement(hSlider, 1, 2);
        
        // VBox for vertical slider and knob
        auto sliderVBox = ui.createVBoxLayout(0, 0, 250, 200);
        auto vSlider = ui.createVSlider(0, 0, 25, 100, 0.0f, 100.0f, 25.0f);
        auto knobSlider = ui.createKnobSlider(0, 0, 60, 0.0f, 100.0f, 75.0f);
        sliderVBox->addElement(vSlider);
        sliderVBox->addElement(knobSlider);
        mainGrid->addElement(sliderVBox, 1, 3);

        // Row 3: Lists, Canvas, and Advanced
        auto advancedLabel = ui.createLabel("Lists & Canvas", 0, 0);
        mainGrid->addElement(advancedLabel, 2, 0);
        
        // ListView
        std::vector<std::string> listItems = {"Item 1", "Item 2", "Item 3", "Item 4"};
        auto listView = ui.createListView(listItems, 0, 0, 250, 150);
        mainGrid->addElement(listView, 2, 1);
        
        // Canvas with some drawings
        auto canvas = ui.createCanvas(0, 0, 250, 150);
        canvas->filledRectRel(10, 10, 50, 30, {255, 0, 0, 255});    // Red rect
        canvas->filledRectRel(70, 20, 40, 40, {0, 255, 0, 255});    // Green rect
        canvas->lineRel(0, 80, 250, 80, {255, 255, 255, 255});      // White line
        mainGrid->addElement(canvas, 2, 2);
        
        // VBox for modals and option select
        auto modalVBox = ui.createVBoxLayout(0, 0, 250, 150);
        
        auto infoModalBtn = ui.createButton("Info Modal", 0, 0, [&]() {
            ui.createInfoModal("This is an info modal!");
        });
        modalVBox->addElement(infoModalBtn);
        
        auto confirmModalBtn = ui.createButton("Confirm Modal", 0, 0, [&]() {
            ui.createConfirmModal("Are you sure?",
                []() { std::cout << "Confirmed!" << std::endl; },
                []() { std::cout << "Cancelled!" << std::endl; }
            );
        });
        modalVBox->addElement(confirmModalBtn);
        
        // OptionSelect
        std::vector<std::string> options = {"Option A", "Option B", "Option C"};
        auto optionSelect = ui.createOptionSelect(0, options, 0, 0, [](int selected) {
            std::cout << "Selected option: " << selected << std::endl;
        });
        modalVBox->addElement(optionSelect);
        
        mainGrid->addElement(modalVBox, 2, 3);

        // Add some hotkeys
        ui.assignHotKey(button, "b");
        ui.assignHotKey(infoModalBtn, "i");
        ui.assignHotKey(confirmModalBtn, "c");

        std::cout << "Complete UI Demo Started!" << std::endl;
        std::cout << "All UI elements organized in a 3x4 grid with borders:" << std::endl;
        std::cout << "Row 1: Basic inputs (Label, Button, TextBox, CheckBox)" << std::endl;
        std::cout << "Row 2: Progress & Sliders (ProgressBar, H/V/Knob Sliders)" << std::endl;
        std::cout << "Row 3: Advanced (ListView, Canvas, Modals, OptionSelect)" << std::endl;
        std::cout << "Hotkeys: b=Button, i=Info Modal, c=Confirm Modal" << std::endl;
        std::cout << "Menu bar provides File/Edit/View options" << std::endl;
        
        ui.run();
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    
    return 0;
}
