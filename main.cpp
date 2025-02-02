#include "UI.h"
#include <memory>
#include <vector>
#include <iostream>
#include <SDL2/SDL.h>
#include "ThemeGlobals.h"

int main() {
    // Create the UI façade with an 800x600 window.
    auto ui = std::make_unique<UI>("TODO App", 800, 600);

    // --- Context Menu ---
    // Create a context menu for global commands.
    std::vector<ui::TopMenuItem> menus = {
        { "File", { 
            {"Open", [](){ std::cout << "Open selected" << std::endl; }}, 
            {"Save", [](){ std::cout << "Save selected" << std::endl; }},
            {"Quit", [uiPtr = ui.get()](){
                 // Show a confirm modal for quitting.
                 uiPtr->confirmModal("Are you sure you want to quit?",
                     [](){ // onConfirm: quit the app.
                         exit(0);
                     },
                     [](){ // onCancel: do nothing.
                         std::cout << "Quit cancelled." << std::endl;
                     }
                 );
            }}
        }},
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
    ui->label("New Todo:", 20, 100);
    // Input text box for the new todo (size 250x40).
    ui::TextBox* inputBox = ui->textBox("Enter todo here...", 150, 100);
    // "Add Todo" button (positioned at 420,100).
    ui->button("Add Todo", 420, 100, [uiPtr = ui.get(), inputBox]() {
             // Check if a modal is already active by seeing if infoModal returns nullptr.
             ui::Modal* modal = uiPtr->infoModal("Task added successfully!", [](){
                  std::cout << "Info modal closed." << std::endl;
             });
             if (!modal) {
                 std::cout << "A modal is already active." << std::endl;
                 return;
             }
             std::string task = inputBox->content;
             std::cout << "New task: " << task << std::endl;
             inputBox->content = "";
    });

    // --- Priority Selector ---
    ui->label("Priority:", 20, 170);
    std::vector<std::string> priorities = { "Low", "Medium", "High", "Urgent" };
    ui->optionSelect(0, priorities, 150, 165, [uiPtr = ui.get()](int idx) {
         std::cout << "Priority selected: " << idx << std::endl;
    });

    // --- Todo List ---
    ui->label("Todo List:", 20, 240);
    ui->checkBox(false, 20, 280, [](bool state) {
         std::cout << "Todo 'Buy milk' done: " << (state ? "Yes" : "No") << std::endl;
    });
    ui->label("Buy milk", 50, 278);
    ui->checkBox(true, 20, 310, [](bool state) {
         std::cout << "Todo 'Call Mom' done: " << (state ? "Yes" : "No") << std::endl;
    });
    ui->label("Call Mom", 50, 308);

    // --- ListView for Recent Tasks ---
    ui->label("Recent Tasks:", 400, 240);
    std::vector<std::string> recentTasks = { "Finish report", "Clean kitchen", "Email boss", "Plan weekend" };
    ui::ListView* lv = ui->listView(recentTasks, 400, 260, 350, 120, 30);

    ui->run();
    return 0;
}
