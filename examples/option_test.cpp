#include "uiframework/UI.h"
#include <iostream>
#include <vector>

int main() {
    try {
        std::cout << "Creating UI..." << std::endl;
        UI ui("Simple Option Test", 400, 300);
        
        std::cout << "Creating option selector..." << std::endl;
        std::vector<std::string> options = {"Option 1", "Option 2", "Option 3"};
        
        auto optionSelect = ui.createOptionSelect(0, options, 10, 10, [](int index) {
            std::cout << "Selected: " << index << std::endl;
        });
        
        std::cout << "Starting main loop..." << std::endl;
        ui.run();
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    
    return 0;
}
