#include "uiframework/UI.h"
#include <iostream>
#include <vector>
#include <algorithm>

class FocusDemo {
private:
    UI ui;
    std::vector<std::shared_ptr<ui::UIElement>> focusableElements;
    int currentFocusIndex = -1;
    
public:
    FocusDemo() : ui("Focus Cycling Demo", 800, 600) {
        // Create a dummy element to assign global hotkeys to
        auto dummyLabel = ui.createLabel("", 0, 0);
        
        // Use assignHotKey with custom callbacks for global focus cycling
        ui.assignHotKey(dummyLabel, "Tab", [this]() {
            focusNext();
        });
        
        ui.assignHotKey(dummyLabel, "`", [this]() {
            focusPrevious();
        });
        
        ui.assignHotKey(dummyLabel, "Escape", []() {
            std::cout << "ESC pressed - exiting demo" << std::endl;
            exit(0);
        });
    }
    
    void addFocusableElement(std::shared_ptr<ui::UIElement> element) {
        focusableElements.push_back(element);
        if (currentFocusIndex == -1) {
            currentFocusIndex = 0;
            ui.setFocus(element);
        }
    }
    
    void focusNext() {
        if (focusableElements.empty()) return;
        
        currentFocusIndex = (currentFocusIndex + 1) % focusableElements.size();
        ui.setFocus(focusableElements[currentFocusIndex]);
        
        std::cout << "Focus moved to element " << currentFocusIndex + 1 
                  << " of " << focusableElements.size() << std::endl;
    }
    
    void focusPrevious() {
        if (focusableElements.empty()) return;
        
        currentFocusIndex = (currentFocusIndex - 1 + focusableElements.size()) % focusableElements.size();
        ui.setFocus(focusableElements[currentFocusIndex]);
        
        std::cout << "Focus moved to element " << currentFocusIndex + 1 
                  << " of " << focusableElements.size() << std::endl;
    }
    
    void updateCurrentFocusIndex() {
        // Update currentFocusIndex when focus changes via mouse click
        std::string focusedId = ui.getFocusedElementId();
        if (!focusedId.empty()) {
            for (size_t i = 0; i < focusableElements.size(); ++i) {
                if (focusableElements[i]->getId() == focusedId) {
                    currentFocusIndex = static_cast<int>(i);
                    break;
                }
            }
        }
    }
    
    void setupElements() {
        // Row 1: Buttons
        auto button1 = ui.createButton("Button 1", 50, 50, []() {
            std::cout << "Button 1 clicked!" << std::endl;
        });
        auto button2 = ui.createButton("Button 2", 200, 50, []() {
            std::cout << "Button 2 clicked!" << std::endl;
        });
        auto button3 = ui.createButton("Button 3", 350, 50, []() {
            std::cout << "Button 3 clicked!" << std::endl;
        });
        
        // Row 2: Text inputs (correct API - no width/height parameters)
        auto textbox1 = ui.createTextBox("Enter text 1", 50, 120);
        textbox1->setSize(140, 30);
        auto textbox2 = ui.createTextBox("Enter text 2", 200, 120);
        textbox2->setSize(140, 30);
        auto textbox3 = ui.createTextBox("Enter text 3", 350, 120);
        textbox3->setSize(140, 30);
        
        // Row 3: Checkboxes (need callback functions)
        auto checkbox1 = ui.createCheckBox(false, 50, 190, [](bool checked) {
            std::cout << "Option 1: " << (checked ? "checked" : "unchecked") << std::endl;
        });
        auto checkbox2 = ui.createCheckBox(false, 200, 190, [](bool checked) {
            std::cout << "Option 2: " << (checked ? "checked" : "unchecked") << std::endl;
        });
        auto checkbox3 = ui.createCheckBox(false, 350, 190, [](bool checked) {
            std::cout << "Option 3: " << (checked ? "checked" : "unchecked") << std::endl;
        });
        
        // Row 4: Sliders (use HSlider)
        auto slider1 = ui.createHSlider(50, 260, 140, 20, 0.0f, 100.0f, 25.0f);
        auto slider2 = ui.createHSlider(200, 260, 140, 20, 0.0f, 100.0f, 50.0f);
        auto slider3 = ui.createHSlider(350, 260, 140, 20, 0.0f, 100.0f, 75.0f);
        
        // Row 5: CycleList and TabbedPanel
        std::vector<std::string> cycleItems = {"Item A", "Item B", "Item C", "Item D"};
        auto cycleList = ui.createCycleList(50, 330, 140, 30, cycleItems);
        
        auto tabbedPanel = ui.createTabbedPanel(200, 330, 300, 150);
        int tab1 = tabbedPanel->addTab("Tab 1");
        int tab2 = tabbedPanel->addTab("Tab 2");
        int tab3 = tabbedPanel->addTab("Tab 3");
        
        // Add labels for checkboxes
        ui.createLabel("Option 1", 70, 190);
        ui.createLabel("Option 2", 220, 190);
        ui.createLabel("Option 3", 370, 190);
        
        // Add elements to focus order
        addFocusableElement(button1);
        addFocusableElement(button2);
        addFocusableElement(button3);
        addFocusableElement(textbox1);
        addFocusableElement(textbox2);
        addFocusableElement(textbox3);
        addFocusableElement(checkbox1);
        addFocusableElement(checkbox2);
        addFocusableElement(checkbox3);
        addFocusableElement(slider1);
        addFocusableElement(slider2);
        addFocusableElement(slider3);
        addFocusableElement(cycleList);
        addFocusableElement(tabbedPanel);
        
        // Add instructions labels
        ui.createLabel("=== FOCUS CYCLING DEMO ===", 50, 500);
        ui.createLabel("TAB: Focus next element", 50, 520);
        ui.createLabel("` (backtick): Focus previous element", 50, 540);
        ui.createLabel("ESC: Exit demo", 50, 560);
        ui.createLabel("Mouse click: Focus element directly", 400, 520);
        ui.createLabel("Total focusable elements: " + std::to_string(focusableElements.size()), 400, 540);
    }
    
    void run() {
        setupElements();
        
        std::cout << "=== Focus Cycling Demo ===" << std::endl;
        std::cout << "TAB: Focus next element" << std::endl;
        std::cout << "` (backtick): Focus previous element" << std::endl;
        std::cout << "ESC: Exit demo" << std::endl;
        std::cout << "Mouse click: Focus element directly" << std::endl;
        std::cout << "=========================" << std::endl;
        
        // Use the framework's built-in event loop
        ui.run();
    }
};

int main() {
    try {
        FocusDemo demo;
        demo.run();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    
    return 0;
}
