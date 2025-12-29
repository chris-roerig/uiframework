#include "uiframework/UI.h"
#include <iostream>

/**
 * @brief Simple UI test for audio recorder interface
 * 
 * Tests just the UI components without audio to isolate any font/rendering issues.
 */
int main() {
    try {
        std::cout << "Starting UI test..." << std::endl;
        
        UI ui("Audio Recorder UI Test", 1000, 700);
        
        // Title
        auto title = ui.createLabel("Audio Recorder - UI Test", 20, 20);
        std::cout << "Created title label" << std::endl;
        
        // Control buttons
        auto recordButton = ui.createButton("Record", 20, 60, []() {
            std::cout << "Record button clicked!" << std::endl;
        });
        std::cout << "Created record button" << std::endl;
        
        auto playButton = ui.createButton("Play", 120, 60, []() {
            std::cout << "Play button clicked!" << std::endl;
        });
        std::cout << "Created play button" << std::endl;
        
        // Status label
        auto statusLabel = ui.createLabel("Ready to test UI", 220, 65);
        std::cout << "Created status label" << std::endl;
        
        // Test canvas
        auto canvas = ui.createCanvas(20, 120, 960, 300);
        std::cout << "Created canvas" << std::endl;
        
        // Draw something on canvas to test
        canvas->clear();
        canvas->lineRel(0, 150, 960, 150, {100, 100, 100, 255}); // Center line
        canvas->rectRel(10, 10, 100, 50, {0, 255, 0, 255}); // Test rectangle
        
        std::cout << "UI setup complete, starting main loop..." << std::endl;
        
        ui.run();
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    
    return 0;
}
