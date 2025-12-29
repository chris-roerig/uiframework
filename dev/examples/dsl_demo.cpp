#include "uiframework/UI.h"
#include "demo_ui.h"
#include <iostream>

int main() {
    try {
        UI ui("DSL Demo", 800, 600);
        
        // Create widgets from .ui file
        auto widgets = ui_generated::createDemo(ui);
        
        // Add event handlers programmatically
        widgets.clickButton = ui.createButton("Click Me!", 10, 50, [&widgets]() {
            widgets.status->setText("Button clicked!");
            std::cout << "Button was clicked!" << std::endl;
        });
        
        widgets.enableFeature = ui.createCheckBox(false, 10, 130, [&widgets](bool checked) {
            widgets.status->setText(checked ? "Feature enabled" : "Feature disabled");
            std::cout << "Feature " << (checked ? "enabled" : "disabled") << std::endl;
        });
        
        std::cout << "DSL Demo started. UI created from demo.ui file." << std::endl;
        std::cout << "- Title: " << widgets.title->getText() << std::endl;
        std::cout << "- Button text: " << widgets.clickButton->getText() << std::endl;
        std::cout << "Close window to exit." << std::endl;
        
        ui.run();
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    
    return 0;
}
