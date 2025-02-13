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

    std::array<int, 3> cols = {15, 555, 600};
    std::array<int, 4> rows = {40, 64, 95, 200};

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
    ui->label("Recent Tasks:", cols[0], rows[0]);
    std::vector<std::string> tasks = {
        "Write report", "Clean room", "Email boss", "Buy groceries", "Schedule meeting",
        "Call mom", "Prepare presentation", "Pay bills", "Review code", "Fix bugs",
        "Plan trip", "Update resume", "Read book", "Watch tutorial", "Practice coding"
    };
    // Create the ListView at (25,60) with width 495, height 375 and each item 35 pixels tall.
    ui::ListView* lv = ui->listView(tasks, cols[0], rows[1], 495, 375, 25);
    
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
    ui::Label* filterLabel = ui->label("Filter:", cols[0], 450);
    // Create a text box for filtering (positioned at 110,395, size 200x30 assumed).
    ui::TextBox* filterBox = ui->textBox("", cols[0] + filterLabel->width + 10, 445);

    // "Apply Filter" button: when pressed, applies the filter to the list.
    ui->button("Apply Filter", cols[0], 480, [lv, filterBox]() {
         lv->applyFilter(filterBox->content);
    });
    
    // "Clear Filter" button: clears the filter and resets the list.
    ui->button("Clear Filter", 188, 480, [lv, filterBox]() {
         filterBox->content = "";
         lv->applyFilter("");
    });
    
    // "Sort Tasks" button: sorts the tasks alphabetically.
    ui->button("Sort Tasks", 360, 480, [lv]() {
         lv->sortItems([](const std::string &a, const std::string &b) {
             return a < b;
         });
    });

    // --- New Todo Input Area ---
    ui::Label* newTodoLabel = ui->label("New Todo:", cols[1], rows[0]);
    ui::TextBox* inputBox = ui->textBox("Enter todo here...", cols[1] + newTodoLabel->width + 10, rows[0] - 5);

    // --- Priority Selector ---
    ui::Label* priorityLabel = ui->label("Priority:", cols[1], rows[1]);
    std::vector<std::string> priorities = { "Low", "Medium", "High", "Urgent" };
    ui::OptionSelect* prioOptions = ui->optionSelect(0, priorities, cols[1] + priorityLabel->width + 10,  rows[1], [uiPtr = ui.get()](int idx) {
         std::cout << "Priority selected: " << idx << std::endl;
    });

    ui::Button* addTodoButton = ui->button("Add Todo", prioOptions->x, rows[2], [uiPtr = ui.get(), inputBox]() {
         std::string task = inputBox->content;
         std::cout << "New task: " << task << std::endl;
         inputBox->content = "";
         if (!uiPtr->infoModal("Task added successfully!", [](){
              std::cout << "Info modal closed." << std::endl;
         })) {
             std::cout << "A modal is already active." << std::endl;
         }
    });

    // --- Todo List ---
    ui->label("Todo List:", cols[1], rows[3]);

    ui::CheckBox* firstCheckbox = ui->checkBox(false, cols[1], 250, [](bool state) {
         std::cout << "Todo 'Buy milk' done: " << (state ? "Yes" : "No") << std::endl;
    });
    ui->label("Buy milk", cols[2], 250);

    ui->checkBox(true, cols[1], 280, [](bool state) {
         std::cout << "Todo 'Call Mom' done: " << (state ? "Yes" : "No") << std::endl;
    });
    ui->label("Call Mom", cols[2], 280);

    // **** Image **** */
    ui->image("demos/smiley.png", cols[1], 400, 100, 100, true);
    // demo of loading a binary/embedded image
    #include "smiley_image.h"
    ui->image(smiley_png, smiley_png_len, cols[1] + 100, 400, 100, 100, true);


    // **** Sprite **** */
    // Example: File-based animated sprite.
    // Create the animated sprite with a destination size of 85×85 (or different, if you want to scale).
    // The parameters: x, y, width, height, file path, renderer is obtained internally,
    // number of frames (9), frame delay in milliseconds (e.g. 100), and stretch (true).
    ui::AnimatedSprite* animSprFile = ui->animatedSprite("demos/sprite_sheet.png", cols[1], 500, 85, 85, 9, 100, true);

    // Since your sprite sheet is 1024×1024 but you want fixed 85×85 frames,
    // override the default frame calculation by explicitly setting the frame dimensions.
    animSprFile->setFrameDimensions(80, 80, 9);

    // Example: Embedded animated sprite.
    // Include your generated header with binary data.
    #include "sprite_sheet_image.h"
    ui::AnimatedSprite* animSprEmbedded = ui->animatedSprite(sprite_sheet_png, sprite_sheet_png_len, cols[1] + 120, 500, 85, 85, 9, 100, true);
    animSprEmbedded->setFrameDimensions(80, 80, 9);


    // Create a progress bar at (50, 100) with a size of 200x20 pixels
    ui::ProgressBar* pb = ui->progressBar(cols[0], 700, 400, 40, true);
    pb->reset(0);
    ui::Button* pbButton = ui->button("Increment", cols[1], 700, [pb](){
        if(pb->getProgress() >= 1) {
            pb->reset(0);
        }
        pb->incrementProgress(0.01);
    }); 


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
    
    // Assign hotkey Ctrl+m to change the theme to Molokai
    ui->assignHotKey(ctxMenu, "m", [ctxMenu]() {
        ctxMenu->activeItemIndex = 1; // "File"
        if (!ctxMenu->items.empty() && ctxMenu->items[0].subCallbacks.size() >= 3)
            ctxMenu->items[1].subCallbacks[3]();
    });


    // Assign hotkey Ctrl+b to activate the Add Todo button.
    ui->assignHotKey(addTodoButton, "b");
    
    // Assign hotkey Ctrl+1 to toggle the first checkbox.
    ui->assignHotKey(firstCheckbox, "1");
    
    
    ui->run();
    return 0;
}
