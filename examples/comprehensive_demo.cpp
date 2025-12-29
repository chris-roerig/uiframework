#include "uiframework/UI.h"
#include <iostream>
#include <vector>
#include <string>

int main() {
    try {
        UI ui("Comprehensive UI Demo - All Widgets with Layouts", 1400, 900);
        
        // Title
        auto titleLabel = ui.createLabel("UI Framework Widget Showcase with Layout System", 200, 10);
        
        // Create main horizontal layout for organizing content
        auto mainHBox = ui.createHBoxLayout(10, 50, 1380, 800, 20);
        
        // Left column - Basic widgets in VBox
        auto leftVBox = ui.createVBoxLayout(0, 0, 350, 750, 10);
        
        // Basic widgets section
        auto basicLabel = ui.createLabel("Basic Widgets:", 0, 0);
        leftVBox->addElement(basicLabel);
        
        auto button1 = ui.createButton("Standard Button", 0, 0, []() {
            std::cout << "Standard button clicked!" << std::endl;
        });
        leftVBox->addElement(button1);
        
        auto button2 = ui.createButton("Action Button", 0, 0, []() {
            std::cout << "Action performed!" << std::endl;
        });
        leftVBox->addElement(button2);
        
        auto textBox = ui.createTextBox("Enter text here...", 0, 0);
        leftVBox->addElement(textBox);
        
        // Checkboxes in horizontal layout
        auto checkHBox = ui.createHBoxLayout(0, 0, 280, 30, 5);
        bool check1State = false, check2State = true;
        auto checkbox1 = ui.createCheckBox(check1State, 0, 0, [&](bool state) {
            check1State = state;
            std::cout << "Checkbox 1: " << (state ? "checked" : "unchecked") << std::endl;
        });
        auto checkLabel1 = ui.createLabel("Feature A", 0, 0);
        checkHBox->addElement(checkbox1);
        checkHBox->addElement(checkLabel1);
        
        auto checkbox2 = ui.createCheckBox(check2State, 0, 0, [&](bool state) {
            check2State = state;
            std::cout << "Checkbox 2: " << (state ? "checked" : "unchecked") << std::endl;
        });
        auto checkLabel2 = ui.createLabel("Feature B", 0, 0);
        checkHBox->addElement(checkbox2);
        checkHBox->addElement(checkLabel2);
        
        leftVBox->addElement(checkHBox);
        
        // Progress bars
        auto progressLabel = ui.createLabel("Progress Bars:", 0, 0);
        leftVBox->addElement(progressLabel);
        
        auto progressBar1 = ui.createProgressBar(0, 0, 280, 25, 0.35f, true);
        leftVBox->addElement(progressBar1);
        
        auto progressBar2 = ui.createProgressBar(0, 0, 280, 25, 0.75f, true);
        leftVBox->addElement(progressBar2);
        
        // Theme buttons in horizontal layout
        auto themeLabel = ui.createLabel("Themes:", 0, 0);
        leftVBox->addElement(themeLabel);
        
        auto themeHBox = ui.createHBoxLayout(0, 0, 280, 35, 5);
        auto themeBtn1 = ui.createButton("Default", 0, 0, [&]() { ui.setTheme("Default"); });
        auto themeBtn2 = ui.createButton("Dark", 0, 0, [&]() { ui.setTheme("SolarizedDark"); });
        auto themeBtn3 = ui.createButton("Light", 0, 0, [&]() { ui.setTheme("SolarizedLight"); });
        auto themeBtn4 = ui.createButton("Molokai", 0, 0, [&]() { ui.setTheme("Molokai"); });
        
        themeHBox->addElement(themeBtn1);
        themeHBox->addElement(themeBtn2);
        themeHBox->addElement(themeBtn3);
        themeHBox->addElement(themeBtn4);
        leftVBox->addElement(themeHBox);
        
        // Add leftVBox to main layout
        mainHBox->addElement(leftVBox);
        
        // Middle column - List and Canvas in VBox
        auto middleVBox = ui.createVBoxLayout(0, 0, 300, 750, 10);
        
        auto listLabel = ui.createLabel("List View:", 0, 0);
        middleVBox->addElement(listLabel);
        
        std::vector<std::string> listItems = {
            "List Item 1", "List Item 2", "List Item 3", 
            "List Item 4", "List Item 5", "List Item 6"
        };
        auto listView = ui.createListView(listItems, 0, 0, 240, 200);
        middleVBox->addElement(listView);
        
        auto canvasLabel = ui.createLabel("Canvas:", 0, 0);
        middleVBox->addElement(canvasLabel);
        
        auto canvas = ui.createCanvas(0, 0, 240, 150);
        // Add some drawing to the canvas
        canvas->filledRectRel(10, 10, 50, 30, {255, 0, 0, 255});
        canvas->filledRectRel(70, 20, 40, 40, {0, 255, 0, 255});
        canvas->filledRectRel(120, 15, 60, 25, {0, 0, 255, 255});
        canvas->lineRel(0, 70, 240, 70, {255, 255, 255, 255});
        middleVBox->addElement(canvas);
        
        // Modal buttons
        auto modalLabel = ui.createLabel("Modals:", 0, 0);
        middleVBox->addElement(modalLabel);
        
        auto infoModalBtn = ui.createButton("Info Modal", 0, 0, [&]() {
            ui.createInfoModal("This is an information modal dialog.");
        });
        middleVBox->addElement(infoModalBtn);
        
        auto confirmModalBtn = ui.createButton("Confirm Modal", 0, 0, [&]() {
            ui.createConfirmModal("Are you sure you want to proceed?",
                []() { std::cout << "Confirmed!" << std::endl; },
                []() { std::cout << "Cancelled!" << std::endl; }
            );
        });
        middleVBox->addElement(confirmModalBtn);
        
        // Add middleVBox to main layout
        mainHBox->addElement(middleVBox);
        
        // Right column - Sliders and Grid Layout Demo
        auto rightVBox = ui.createVBoxLayout(0, 0, 350, 750, 15);
        
        // Sliders section
        auto sliderLabel = ui.createLabel("Sliders:", 0, 0);
        rightVBox->addElement(sliderLabel);
        
        // Horizontal slider with label
        auto hSliderHBox = ui.createHBoxLayout(0, 0, 280, 35, 10);
        auto hSlider = ui.createHSlider(0, 0, 180, 30, 0.0f, 100.0f, 50.0f);
        auto hSliderLabel = ui.createLabel("H: 50", 0, 0);
        hSliderHBox->addElement(hSlider);
        hSliderHBox->addElement(hSliderLabel);
        rightVBox->addElement(hSliderHBox);
        
        // Vertical and Knob sliders in horizontal layout
        auto vkSliderHBox = ui.createHBoxLayout(0, 0, 280, 100, 20);
        auto vSlider = ui.createVSlider(0, 0, 30, 90, 0.0f, 100.0f, 25.0f);
        auto vSliderLabel = ui.createLabel("V: 25", 0, 0);
        auto vSliderVBox = ui.createVBoxLayout(0, 0, 60, 90, 5);
        vSliderVBox->addElement(vSlider);
        vSliderVBox->addElement(vSliderLabel);
        
        auto knobSlider = ui.createKnobSlider(0, 0, 80, 0.0f, 100.0f, 75.0f);
        auto knobLabel = ui.createLabel("Knob: 75", 0, 0);
        auto knobVBox = ui.createVBoxLayout(0, 0, 80, 90, 5);
        knobVBox->addElement(knobSlider);
        knobVBox->addElement(knobLabel);
        
        vkSliderHBox->addElement(vSliderVBox);
        vkSliderHBox->addElement(knobVBox);
        rightVBox->addElement(vkSliderHBox);
        
        // Grid Layout Demo with borders
        auto gridLabel = ui.createLabel("Grid Layout Demo (2x3) with borders:", 0, 0);
        rightVBox->addElement(gridLabel);
        
        auto gridDemo = ui.createGridLayout(0, 0, 280, 120, 2, 3, 5);
        gridDemo->setBorderWidth(1);  // Add subtle borders to show grid structure
        for (int i = 0; i < 6; i++) {
            std::string label = "G" + std::to_string(i + 1);
            auto gridBtn = ui.createButton(label, 0, 0, [i]() { 
                std::cout << "Grid Button " << (i + 1) << " clicked!" << std::endl; 
            });
            gridDemo->addElement(gridBtn);
        }
        rightVBox->addElement(gridDemo);
        
        // Layout Demo section
        auto layoutLabel = ui.createLabel("Layout Types:", 0, 0);
        rightVBox->addElement(layoutLabel);
        
        auto layoutInfo1 = ui.createLabel("• VBox: Vertical stacking", 0, 0);
        auto layoutInfo2 = ui.createLabel("• HBox: Horizontal arrangement", 0, 0);
        auto layoutInfo3 = ui.createLabel("• Grid: 2D positioning", 0, 0);
        rightVBox->addElement(layoutInfo1);
        rightVBox->addElement(layoutInfo2);
        rightVBox->addElement(layoutInfo3);
        
        mainHBox->addElement(rightVBox);
        
        // Add callbacks to update slider labels
        hSlider->setOnChange([hSliderLabel](float value) {
            hSliderLabel->setText("H: " + std::to_string((int)value));
        });
        
        vSlider->setOnChange([vSliderLabel](float value) {
            vSliderLabel->setText("V: " + std::to_string((int)value));
        });
        
        knobSlider->setOnChange([knobLabel](float value) {
            knobLabel->setText("Knob: " + std::to_string((int)value));
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
            {"Layout", {
                {"VBox Demo", []() { std::cout << "VBox layout demonstrated in left column" << std::endl; }},
                {"HBox Demo", []() { std::cout << "HBox layout demonstrated in theme buttons" << std::endl; }},
                {"Grid Demo", []() { std::cout << "Grid layout demonstrated with 6 buttons" << std::endl; }}
            }}
        };
        auto contextMenu = ui.createContextMenu(menuItems);
        
        // Instructions at bottom
        auto instructionLabel1 = ui.createLabel("Instructions: All widgets organized using VBox, HBox, and Grid layouts", 10, 760);
        auto instructionLabel2 = ui.createLabel("• Right-click for context menu • Try theme buttons • Interact with all widgets", 10, 780);
        
        // Hotkeys
        ui.assignHotKey(button1, "1");
        ui.assignHotKey(button2, "2");
        ui.assignHotKey(infoModalBtn, "i");
        ui.assignHotKey(confirmModalBtn, "c");
        
        // Focus on first interactive element
        ui.setFocus(button1);
        
        std::cout << "Comprehensive demo with Layout System started!" << std::endl;
        std::cout << "Layout organization:" << std::endl;
        std::cout << "• Main HBox: 3 columns (Basic Widgets | List/Canvas | Sliders/Grid)" << std::endl;
        std::cout << "• Left VBox: Basic widgets stacked vertically" << std::endl;
        std::cout << "• Middle VBox: ListView and Canvas" << std::endl;
        std::cout << "• Right VBox: Sliders and Grid layout demo" << std::endl;
        std::cout << "• Nested layouts: Theme buttons in HBox, Checkboxes in HBox" << std::endl;
        std::cout << "• Grid demo: 2x3 button grid with borders" << std::endl;
        std::cout << "Hotkeys: 1=Button1, 2=Button2, i=Info Modal, c=Confirm Modal" << std::endl;
        std::cout << "Close the window to exit." << std::endl;
        
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
