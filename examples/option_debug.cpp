#include "../src/UI.h"
#include <iostream>
#include <vector>

int main() {
    try {
        UI ui("OptionSelect Debug", 400, 300);
        
        std::cout << "Creating simple options..." << std::endl;
        std::vector<std::string> options = {"A", "B"};  // Very short options
        
        std::cout << "Creating OptionSelect..." << std::endl;
        auto optionSelect = ui.createOptionSelect(0, options, 50, 50, [](int index) {
            std::cout << "Selected: " << index << std::endl;
        });
        
        std::cout << "OptionSelect created, starting UI..." << std::endl;
        ui.run();
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    
    return 0;
}
