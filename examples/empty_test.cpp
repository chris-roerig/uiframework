#include "../src/UI.h"
#include <iostream>

int main() {
    try {
        std::cout << "Creating UI..." << std::endl;
        UI ui("Empty Test", 400, 300);
        
        std::cout << "Starting main loop..." << std::endl;
        ui.run();
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    
    return 0;
}
