#include "uiframework/UI.h"
#include <iostream>

int main() {
    try {
        std::cout << "Creating UI..." << std::endl;
        UI ui("Minimal Test", 400, 300);
        
        std::cout << "Creating button..." << std::endl;
        auto button = ui.createButton("Test", 10, 10, []() {
            std::cout << "Button clicked!" << std::endl;
        });
        
        std::cout << "Starting main loop..." << std::endl;
        ui.run();
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    
    return 0;
}
