#include "../src/UI.h"
#include <iostream>

int main() {
    try {
        UI ui("VirtualKeyboard Test", 600, 400);
        
        std::string result = "";
        
        auto keyboard = ui.createVirtualKeyboard(50, 50, 500, 150, [&](char c) {
            result += c;
            std::cout << "Input: '" << c << "' -> Result: \"" << result << "\"" << std::endl;
        });
        
        auto resultLabel = ui.createLabel("Result: " + result, 50, 220);
        
        std::cout << "VirtualKeyboard Test - Controls:" << std::endl;
        std::cout << "- Arrow keys: Navigate" << std::endl;
        std::cout << "- Shift: Cycle modes (abc->ABC->123->!@#)" << std::endl;
        std::cout << "- Enter: Input character" << std::endl;
        std::cout << "- Double Enter: Space" << std::endl;
        
        ui.setFocus(keyboard);
        ui.run();
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    
    return 0;
}
