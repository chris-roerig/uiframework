#include "../src/UI.h"
#include <iostream>

int main() {
    try {
        UI ui("Test Without OptionSelect", 400, 200);
        
        auto label = ui.createLabel("This demo has NO OptionSelect", 50, 50);
        auto button = ui.createButton("Click me", 50, 100, [](){
            std::cout << "Button clicked!" << std::endl;
        });
        
        std::cout << "Demo without OptionSelect started. Should work fine." << std::endl;
        
        ui.run();
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    
    return 0;
}
