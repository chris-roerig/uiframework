#include "../src/UI.h"
#include <iostream>
#include <vector>

int main() {
    try {
        UI ui("Original OptionSelect Test", 600, 400);
        
        std::vector<std::string> options = {"Apple", "Banana", "Cherry", "Date"};
        
        auto optionSelect = ui.createOptionSelect(1, options, 50, 50, [](int index) {
            std::cout << "Original OptionSelect selected: " << index << std::endl;
        });
        
        auto label = ui.createLabel("Click dropdown to test original OptionSelect", 50, 20);
        
        std::cout << "Original OptionSelect Demo started" << std::endl;
        std::cout << "Initial: " << optionSelect->getSelectedOption() << std::endl;
        
        ui.run();
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    
    return 0;
}
