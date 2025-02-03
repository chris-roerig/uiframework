#include "../UI.h"
#include <memory>
#include <vector>
#include <iostream>
#include <SDL2/SDL.h>
#include "../Theme/ThemeGlobals.h"
#include <cctype>

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
    
    // --- Recent Tasks ListView Demonstration ---
    ui->label("Recent Tasks:", 50, 50);
    std::vector<std::string> tasks = {
        "Write report", "Clean room", "Email boss", "Buy groceries", "Schedule meeting",
        "Call mom", "Prepare presentation", "Pay bills", "Review code", "Fix bugs",
        "Plan trip", "Update resume", "Read book", "Watch tutorial", "Practice coding"
    };
    // Create the ListView at (50,80) with width 400, height 300, and each item 30 pixels tall.
    ui::ListView* lv = ui->listView(tasks, 50, 80, 400, 300, 30);
    
    lv->onItemActivated = [](int index) {
        std::cout << "Item activated at index: " << index << std::endl;
        // You can also retrieve the text of the activated item if needed.
        // (Assuming you have a method getSelectedItem())
    };

    std::string selected = lv->getSelectedItem();
    std::cout << "Currently selected: " << selected << std::endl;

    lv->setSelectedIndex(3);  // This sets the selection to the first item.

    selected = lv->getSelectedItem();
    std::cout << "Currently selected: " << selected << std::endl;


    // --- Filtering and Sorting Controls ---
    ui->label("Filter:", 50, 400);
    // Create a text box for filtering (positioned at 110,395, size 200x30 assumed).
    ui::TextBox* filterBox = ui->textBox("", 110, 395);
    
    // "Apply Filter" button: when pressed, applies the filter to the list.
    ui->button("Apply Filter", 330, 395, [lv, filterBox]() {
         lv->applyFilter(filterBox->content);
    });
    
    // "Clear Filter" button: clears the filter and resets the list.
    ui->button("Clear Filter", 470, 395, [lv, filterBox]() {
         filterBox->content = "";
         lv->applyFilter("");
    });
    
    // "Sort Tasks" button: sorts the tasks alphabetically.
    ui->button("Sort Tasks", 50, 440, [lv]() {
         lv->sortItems([](const std::string &a, const std::string &b) {
             return a < b;
         });
    });

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

    // --- Hotkey Registration ---
    // Assign hotkey Ctrl+f to bring focus to the File menu.
    ui->assignHotKey(ctxMenu, "f", [ctxMenu, uiPtr = ui.get()](){
        ctxMenu->activeItemIndex = 0; // "File"
        ctxMenu->expanded = true;
        uiPtr->setFocus(ctxMenu);
    });
    
    // Assign hotkey Ctrl+q to fire the Quit action.
    ui->assignHotKey(ctxMenu, "q", [ctxMenu]() {
        ctxMenu->activeItemIndex = 0; // "File"
        ctxMenu->expanded = true;
        if (!ctxMenu->items.empty() && ctxMenu->items[0].subCallbacks.size() >= 3)
            ctxMenu->items[0].subCallbacks[2]();
    });
    
    // Assign hotkey Ctrl+b to activate the Add Todo button.
    ui->assignHotKey(addTodoButton, "b");
    
    // Assign hotkey Ctrl+1 to toggle the first checkbox.
    ui->assignHotKey(firstCheckbox, "1");
    
    
    ui->run();
    return 0;
}
