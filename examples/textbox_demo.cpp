#include "../src/UI.h"
#include <iostream>

int main() {
    try {
        UI ui("TextBox Test", 500, 300);
        
        auto label = ui.createLabel("Click in the text boxes to test focus:", 20, 20);
        
        auto textBox1 = ui.createTextBox("First text box", 20, 60);
        auto textBox2 = ui.createTextBox("Second text box", 20, 110);
        auto textBox3 = ui.createTextBox("Third text box", 20, 160);
        
        auto button = ui.createButton("Clear All", 300, 60, [&]() {
            textBox1->setText("");
            textBox2->setText("");
            textBox3->setText("");
            std::cout << "All text boxes cleared" << std::endl;
        });
        
        std::cout << "TextBox Demo started" << std::endl;
        std::cout << "Click in text boxes to focus and type" << std::endl;
        
        ui.run();
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    
    return 0;
}
