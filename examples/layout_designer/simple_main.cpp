#include "SimpleLayoutDesigner.h"
#include <iostream>

int main() {
    try {
        std::cout << "=== Simple Layout Designer ===\n";
        std::cout << "Instructions:\n";
        std::cout << "1. Click toolbar buttons to add colored rectangles\n";
        std::cout << "2. Each rectangle represents a UI element type\n";
        std::cout << "3. Click 'Show Layout' to see positions and code\n";
        std::cout << "4. Use 'Clear All' to remove all elements\n\n";
        
        SimpleLayoutDesigner designer;
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
