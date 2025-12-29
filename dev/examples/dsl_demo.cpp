#include "uiframework/UI.h"
#include "demo_ui.h"
#include <iostream>

int main() {
    try {
        UI ui("SynthSampler Pro", 800, 600);
        
        // Create widgets from .ui file
        auto widgets = ui_generated::createDemo(ui);
        
        // Add event handlers programmatically
        widgets.playButton = ui.createButton("PLAY", 20, 280, [&widgets]() {
            widgets.status->setText("Playing...");
            std::cout << "Play button pressed!" << std::endl;
        });
        
        widgets.stopButton = ui.createButton("STOP", 80, 280, [&widgets]() {
            widgets.status->setText("Stopped");
            std::cout << "Stop button pressed!" << std::endl;
        });
        
        widgets.loopEnable = ui.createCheckBox(false, 20, 220, [&widgets](bool checked) {
            widgets.status->setText(checked ? "Loop enabled" : "Loop disabled");
            std::cout << "Loop " << (checked ? "enabled" : "disabled") << std::endl;
        });
        
        std::cout << "SynthSampler Pro started. UI created from demo.ui file." << std::endl;
        std::cout << "- Title: " << widgets.title->getText() << std::endl;
        std::cout << "- Play button: " << widgets.playButton->getText() << std::endl;
        std::cout << "Close window to exit." << std::endl;
        
        ui.run();
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    
    return 0;
}
