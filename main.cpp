#include "UI.h"
#include <memory>
#include <vector>
#include <iostream>
#include <SDL2/SDL.h>
#include "ThemeGlobals.h"

int main() {
    // Create the UI façade with an 800x600 window.
    auto ui = std::make_unique<UI>("TODO App", 800, 600);

    // Create a context menu (spanning the top of the window) for global actions.
    std::vector<TopMenuItem> menus = {
        { "File", { 
            {"Open", [](){ std::cout << "Open selected" << std::endl; }}, 
            {"Save", [](){ std::cout << "Save selected" << std::endl; }},
            {"Quit", [](){ std::cout << "Quit selected" << std::endl; }}
          } 
        },
        { "Theme", { 
            {"Default", [uiPtr = ui.get()](){ uiPtr->setTheme("FrameworkDefault"); }},
            {"Solarized Dark", [uiPtr = ui.get()](){ uiPtr->setTheme("SolarizedDark"); }},
            {"Solarized Light", [uiPtr = ui.get()](){ uiPtr->setTheme("SolarizedLight"); }},
            {"Molokai", [uiPtr = ui.get()](){ uiPtr->setTheme("Molokai"); }}
          } 
        },
        { "Help", { 
            {"Manual", [](){ std::cout << "Manual selected" << std::endl; }},
            {"Credits", [](){ std::cout << "Credits selected" << std::endl; }}
          } 
        }
    };
    ui->contextMenu(menus);

    // --- New Todo Input Area ---
    // Place "New Todo:" label.
    ui->label("New Todo:", 20, 120);
    // Input text box for todo (autoHighlight defaults to true).
    ui::TextBox* inputBox = ui->textBox("Enter todo here...", 150, 115);
    // "Add Todo" button.
    ui->button("Add Todo", 420, 115, [uiPtr = ui.get(), inputBox]() {
         std::string task = inputBox->content;
         std::cout << "New task: " << task << std::endl;
         inputBox->content = ""; // Clear input after submission.
    });

    // --- Priority Selector ---
    ui->label("Priority:", 20, 180);
    // Create an optionSelect for priority with fixed options.
    std::vector<std::string> priorities = { "Low", "Medium", "High", "Urgent" };
    // Position it next to the label.
    ui->optionSelect(0, priorities, 150, 175, [uiPtr = ui.get()](int idx) {
         std::cout << "Priority selected: " << idx << std::endl;
         // In a real app, you might store the priority for the new todo.
    });

    // --- Todo List ---
    ui->label("Todo List:", 20, 240);
    // Sample todo item 1.
    ui->checkBox(false, 20, 280, [](bool state) {
         std::cout << "Todo 'Buy milk' done: " << (state ? "Yes" : "No") << std::endl;
    });
    ui->label("Buy milk", 50, 280);
    // Sample todo item 2.
    ui->checkBox(true, 20, 310, [](bool state) {
         std::cout << "Todo 'Call Mom' done: " << (state ? "Yes" : "No") << std::endl;
    });
    ui->label("Call Mom", 50, 310);

    ui->run();
    return 0;
}
