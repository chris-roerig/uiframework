#include "../src/UI.h"
#include <iostream>

int main() {
    try {
        UI ui("Slider Test", 600, 400);
        
        auto label1 = ui.createLabel("Horizontal Slider:", 20, 20);
        auto hslider = ui.createHSlider(20, 50, 200, 30, 0.0f, 100.0f, 25.0f);
        auto hlabel = ui.createLabel("Value: 25.0", 250, 55);
        
        auto label2 = ui.createLabel("Vertical Slider:", 20, 120);
        auto vslider = ui.createVSlider(20, 150, 30, 200, -50.0f, 50.0f, 0.0f);
        auto vlabel = ui.createLabel("Value: 0.0", 70, 250);
        
        // Set callbacks
        hslider->setOnChange([hlabel](float value) {
            hlabel->setText("Value: " + std::to_string(value));
        });
        
        vslider->setOnChange([vlabel](float value) {
            vlabel->setText("Value: " + std::to_string(value));
        });
        
        std::cout << "Slider Test started" << std::endl;
        std::cout << "Drag the sliders to test functionality" << std::endl;
        
        ui.run();
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    
    return 0;
}
