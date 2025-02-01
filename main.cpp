#include "UI.h"
#include <memory>
#include <vector>
#include <iostream>
#include <SDL2/SDL.h>
#include "ThemeGlobals.h"

// (You can reuse your helper functions from previous demos as needed.)
int main() {
    // Create the UI façade with a window sized to 800x600.
    auto ui = std::make_unique<UI>("TODO App", 800, 600);
    
    // --- Top: Theme Selection ---
    std::vector<std::string> themes = { "FrameworkDefault", "SolarizedDark", "SolarizedLight", "Molokai" };
    ui->label("Select Theme:", 20, 20);
    ui->optionSelect(0, themes, 150, 15, [uiPtr = ui.get()](int idx) {
         switch(idx) {
             case 0: uiPtr->setTheme("FrameworkDefault"); break;
             case 1: uiPtr->setTheme("SolarizedDark"); break;
             case 2: uiPtr->setTheme("SolarizedLight"); break;
             case 3: uiPtr->setTheme("Molokai"); break;
         }
         std::cout << "Theme changed to index: " << idx << std::endl;
    });
    
    // --- Middle: New Todo Input ---
    ui->label("New Todo:", 20, 100);
    // Create input box; autoHighlight defaults to true.
    ui::TextBox* inputBox = ui->textBox("Enter todo here...", 150, 95);
    // Submit button.
    ui->button("Add Todo", 320, 95, [uiPtr = ui.get(), inputBox]() {
         std::string task = inputBox->content;
         std::cout << "New task: " << task << std::endl;
         inputBox->content = ""; // Clear input after submission.
    });
    
    // --- Lower: Todo List ---
    ui->label("Todo List:", 20, 170);
    // Example todo item 1.
    ui->checkBox(false, 20, 210, [](bool state) {
         std::cout << "Todo 'Buy milk' done: " << (state ? "Yes" : "No") << std::endl;
    });
    ui->label("Buy milk", 50, 210);
    // Example todo item 2.
    ui->checkBox(true, 20, 250, [](bool state) {
         std::cout << "Todo 'Call Mom' done: " << (state ? "Yes" : "No") << std::endl;
    });
    ui->label("Call Mom", 50, 250);
    
    // (Optionally, you could add a canvas here for a progress preview.)
    
    ui->run();
    return 0;
}
