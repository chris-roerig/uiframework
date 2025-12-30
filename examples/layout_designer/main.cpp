#include "LayoutDesigner.h"
#include <iostream>

int main() {
    try {
        std::cout << "=== UI Framework Layout Designer ===\n";
        std::cout << "Instructions:\n";
        std::cout << "1. Click toolbar buttons to add UI elements\n";
        std::cout << "2. Drag elements in the design area to reposition\n";
        std::cout << "3. Click 'Show Layout' to print element positions\n";
        std::cout << "4. Resize window to test responsive design\n\n";
        
        LayoutDesigner designer;
        designer.run();
        
    } catch (const ui::UIException& e) {
        std::cerr << "UI Error: " << e.what() << std::endl;
        return -1;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    
    return 0;
}
