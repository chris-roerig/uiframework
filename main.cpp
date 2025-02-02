#include "UI.h"
#include <memory>
#include <vector>
#include <iostream>
#include <SDL2/SDL.h>
#include "ThemeGlobals.h"
#include <cctype>

// Helper: Convert a single-character string to SDL_Keycode (always lowercase)
SDL_Keycode keycodeFromString(const std::string &s) {
    if (s.empty())
        return SDLK_UNKNOWN;
    char ch = s[0];
    ch = static_cast<char>(std::tolower(ch));
    if (std::isdigit(ch))
        return SDLK_0 + (ch - '0');
    return ch; // letters etc.
}

int main() {
    // Create the UI façade with a 1024x896 window.
    auto ui = std::make_unique<UI>("TODO App", 1024, 896);

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
    // Make contextMenu return a pointer.
    ui::ContextMenu* ctxMenu = ui->contextMenu(menus);
    
    // --- New Todo Input Area ---
    ui->label("New Todo:", 20, 100);
    ui::TextBox* inputBox = ui->textBox("Enter todo here...", 150, 100);
    ui::Button* addTodoButton = ui->button("Add Todo", 420, 100, [uiPtr = ui.get(), inputBox]() {
         std::string task = inputBox->content;
         std::cout << "New task: " << task << std::endl;
         inputBox->content = "";
         if (!uiPtr->infoModal("Task added successfully!", [](){
              std::cout << "Info modal closed." << std::endl;
         })) {
             std::cout << "A modal is already active." << std::endl;
         }
    });

    // --- Priority Selector ---
    ui->label("Priority:", 20, 170);
    std::vector<std::string> priorities = { "Low", "Medium", "High", "Urgent" };
    ui->optionSelect(0, priorities, 150, 165, [uiPtr = ui.get()](int idx) {
         std::cout << "Priority selected: " << idx << std::endl;
    });

    // --- Todo List ---
    ui->label("Todo List:", 20, 240);
    ui::CheckBox* firstCheckbox = ui->checkBox(false, 20, 280, [](bool state) {
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

    // --- Hotkey Registration ---
    // Assign hotkey Ctrl+f to the context menu: set active top-level item to "File" (index 0) and expand.
    ui->assignHotKey(ctxMenu, "f"); // In your implementation, this lambda should set: ctxMenu->activeItemIndex = 0; ctxMenu->expanded = true;
    
    // Register hotkey Ctrl+q to trigger the Quit action.
    // We assume that you can access the core's hot key registry via a getter or public member.
    // For this demo, we'll assume ui->core is accessible (or create a UI::registerHotKey helper).
    ui->assignHotKey(ctxMenu, "q"); 
    // In your assignHotKey implementation for context menu with "q", the lambda should:
    //   set ctxMenu->activeItemIndex = 0; ctxMenu->expanded = true; and then call
    //   ctxMenu->items[0].subCallbacks[2]() (for "Quit") if available.
    
    // Assign hotkey Ctrl+b to the Add Todo button.
    ui->assignHotKey(addTodoButton, "b");
    
    // Assign hotkey Ctrl+1 to toggle the first checkbox.
    ui->assignHotKey(firstCheckbox, "1");
    
    ui->run();
    return 0;
}
