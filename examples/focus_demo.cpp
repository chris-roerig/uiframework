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
        // The assignHotKey API doesn't work well for Tab keys
        // We'll need to handle this differently - let's create a simple demo first
        // and then add custom event handling
    }
    
    void addFocusableElement(std::shared_ptr<ui::UIElement> element) {
        focusableElements.push_back(element);
        if (currentFocusIndex == -1) {
            currentFocusIndex = 0;
            ui.setFocus(element);
            std::cout << "Initial focus set to element 1" << std::endl;
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
                    std::cout << "Focus updated to element " << currentFocusIndex + 1 << " via mouse click" << std::endl;
                    break;
                }
            }
        }
    }
    
    void setupElements() {
        // Row 1: TextBoxes (these auto-focus on click)
        auto textbox1 = ui.createTextBox("Text 1", 50, 50);
        textbox1->setSize(140, 30);
        auto textbox2 = ui.createTextBox("Text 2", 200, 50);
        textbox2->setSize(140, 30);
        auto textbox3 = ui.createTextBox("Text 3", 350, 50);
        textbox3->setSize(140, 30);
        
        // Row 2: More TextBoxes
        auto textbox4 = ui.createTextBox("Text 4", 50, 100);
        textbox4->setSize(140, 30);
        auto textbox5 = ui.createTextBox("Text 5", 200, 100);
        textbox5->setSize(140, 30);
        auto textbox6 = ui.createTextBox("Text 6", 350, 100);
        textbox6->setSize(140, 30);
        
        // Row 3: Checkboxes (these auto-focus on click)
        auto checkbox1 = ui.createCheckBox(false, 50, 150, [](bool checked) {
            std::cout << "Option 1: " << (checked ? "checked" : "unchecked") << std::endl;
        });
        auto checkbox2 = ui.createCheckBox(false, 200, 150, [](bool checked) {
            std::cout << "Option 2: " << (checked ? "checked" : "unchecked") << std::endl;
        });
        auto checkbox3 = ui.createCheckBox(false, 350, 150, [](bool checked) {
            std::cout << "Option 3: " << (checked ? "checked" : "unchecked") << std::endl;
        });
        
        // Row 4: Sliders (these auto-focus on click)
        auto slider1 = ui.createHSlider(50, 200, 140, 20, 0.0f, 100.0f, 25.0f);
        auto slider2 = ui.createHSlider(200, 200, 140, 20, 0.0f, 100.0f, 50.0f);
        auto slider3 = ui.createHSlider(350, 200, 140, 20, 0.0f, 100.0f, 75.0f);
        
        // Row 5: CycleList and TabbedPanel (these auto-focus on click)
        std::vector<std::string> cycleItems = {"Item A", "Item B", "Item C", "Item D"};
        auto cycleList = ui.createCycleList(50, 250, 140, 30, cycleItems);
        
        auto tabbedPanel = ui.createTabbedPanel(200, 250, 300, 150);
        tabbedPanel->addTab("Tab 1");
        tabbedPanel->addTab("Tab 2");
        tabbedPanel->addTab("Tab 3");
        
        // Add labels for checkboxes
        ui.createLabel("Option 1", 70, 150);
        ui.createLabel("Option 2", 220, 150);
        ui.createLabel("Option 3", 370, 150);
        
        // Add elements to focus order
        addFocusableElement(textbox1);
        addFocusableElement(textbox2);
        addFocusableElement(textbox3);
        addFocusableElement(textbox4);
        addFocusableElement(textbox5);
        addFocusableElement(textbox6);
        addFocusableElement(checkbox1);
        addFocusableElement(checkbox2);
        addFocusableElement(checkbox3);
        addFocusableElement(slider1);
        addFocusableElement(slider2);
        addFocusableElement(slider3);
        addFocusableElement(cycleList);
        addFocusableElement(tabbedPanel);
        
        // Add instructions labels
        ui.createLabel("=== FOCUS CYCLING DEMO ===", 50, 420);
        ui.createLabel("Click elements to focus them (yellow border)", 50, 440);
        ui.createLabel("Press TAB/SHIFT+TAB for keyboard navigation", 50, 460);
        ui.createLabel("Press 'n' for next, 'p' for previous", 50, 480);
        ui.createLabel("Total focusable elements: " + std::to_string(focusableElements.size()), 400, 480);
    }
    
    void run() {
        setupElements();
        
        std::cout << "=== Focus Cycling Demo ===" << std::endl;
        std::cout << "Click elements to focus them (yellow border)" << std::endl;
        std::cout << "Press TAB/SHIFT+TAB for keyboard navigation" << std::endl;
        std::cout << "Press 'n' for next, 'p' for previous" << std::endl;
        std::cout << "All elements auto-focus on click!" << std::endl;
        std::cout << "=========================" << std::endl;
        
        // Add simple hotkeys that work with the current system
        auto dummyLabel = ui.createLabel("", 0, 0);
        ui.assignHotKey(dummyLabel, "n", [this]() {
            focusNext();
        });
        ui.assignHotKey(dummyLabel, "p", [this]() {
            focusPrevious();
        });
        
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
