#include "uiframework/UI.h"
#include <iostream>

int main() {
    try {
        UI ui("CycleList Demo", 400, 300);
        
        // Create test data
        std::vector<std::string> options = {"Option A", "Option B", "Option C", "Option D", "Option E"};
        
        // Create CycleList
        auto cycleList = ui.createCycleList(50, 50, 200, 30, options);
        cycleList->setOnChange([](int index, const std::string& value) {
            std::cout << "Selected: " << index << " - " << value << std::endl;
        });
        
        // Add instructions
        ui.createLabel("Use UP/DOWN arrows to cycle through options", 50, 100);
        ui.createLabel("Click to focus the CycleList first", 50, 120);
        
        ui.run();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    
    return 0;
}
