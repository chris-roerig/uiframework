#include "../UI.h"
#include <memory>
#include <vector>
#include <iostream>
#include <SDL2/SDL.h>
#include "../Theme/ThemeGlobals.h"
#include <cctype>

// For embedded images:
#include "smiley_image.h"
#include "sprite_sheet_image.h"

int main() {
    // Create the UI façade with a 1024x896 window.
    auto ui = std::make_unique<UI>("TODO App", 1024, 896);

    // -------------------------------
    // Header: Context Menu & Title
    // -------------------------------
    // Create a context menu for global commands.
    std::vector<ui::TopMenuItem> menus = {
        { "File", { 
            {"Open", [](){ std::cout << "Open selected" << std::endl; }}, 
            {"Save", [](){ std::cout << "Save selected" << std::endl; }},
            {"Quit", [uiPtr = ui.get()](){
                 uiPtr->confirmModal("Are you sure you want to quit?",
                     [](){ exit(0); },
                     [](){ std::cout << "Quit cancelled." << std::endl; }
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
    ui::ContextMenu* ctxMenu = ui->contextMenu(menus);

    // Create a top-level vertical StackContainer covering the window.
    ui::StackContainer* mainStack = ui->stackContainer(0, 0, 1024, 896, ui::StackOrientation::Vertical);

    // --- Header Area ---
    // A FlowContainer that holds a title.
    ui::FlowContainer* headerFlow = ui->flowContainer(0, 0, 1024, 50);
    headerFlow->addChild(ui->label("TODO App", 10, 10));
    mainStack->addChild(headerFlow);

    // -------------------------------
    // Content Area: Split Left and Right Panels
    // -------------------------------
    ui::SplitContainer* contentSplit = ui->splitContainer(0, 50, 1024, 500, 500);
    
    // Left Panel: Recent Tasks
    ui::StackContainer* leftStack = ui->stackContainer(0, 0, 500, 500, ui::StackOrientation::Vertical);
    leftStack->addChild(ui->label("Recent Tasks", 0, 0));
    std::vector<std::string> tasks = {
        "Write report", "Clean room", "Email boss", "Buy groceries", "Schedule meeting",
        "Call mom", "Prepare presentation", "Pay bills", "Review code", "Fix bugs"
    };
    ui::ListView* lv = ui->listView(tasks, 0, 0, 500, 400, 35);
    leftStack->addChild(lv);
    contentSplit->setLeftPanel(leftStack);

    // Right Panel: New Todo, Filtering, and Priority Controls
    ui::StackContainer* rightStack = ui->stackContainer(0, 0, 524, 500, ui::StackOrientation::Vertical);
    
    // New Todo Input
    ui::FlowContainer* newTodoFlow = ui->flowContainer(0, 0, 524, 50);
    newTodoFlow->addChild(ui->label("New Todo:", 0, 0));
    newTodoFlow->addChild(ui->textBox("Enter todo here...", 0, 0));
    rightStack->addChild(newTodoFlow);
    
    // Filter and Sort Controls
    ui::FlowContainer* filterFlow = ui->flowContainer(0, 0, 524, 50);
    filterFlow->addChild(ui->label("Filter:", 0, 0));
    filterFlow->addChild(ui->textBox("", 0, 0));
    filterFlow->addChild(ui->button("Apply Filter", 0, 0, [lv](){
        lv->applyFilter("sample filter");
    }));
    filterFlow->addChild(ui->button("Sort Tasks", 0, 0, [lv](){
        lv->sortItems([](const std::string &a, const std::string &b){ return a < b; });
    }));
    rightStack->addChild(filterFlow);
    
    // Priority Selector and Add Todo Button
    ui::FlowContainer* priorityFlow = ui->flowContainer(0, 0, 524, 50);
    priorityFlow->addChild(ui->label("Priority:", 0, 0));
    std::vector<std::string> priorities = {"Low", "Medium", "High", "Urgent"};
    priorityFlow->addChild(ui->optionSelect(0, priorities, 0, 0, [](int idx){
        std::cout << "Priority selected: " << idx << std::endl;
    }));
    priorityFlow->addChild(ui->button("Add Todo", 0, 0, [uiPtr = ui.get()](){
        if (!uiPtr->infoModal("Task added successfully!", [](){
            std::cout << "Info modal closed." << std::endl;
        })) {
            std::cout << "A modal is already active." << std::endl;
        }
    }));
    rightStack->addChild(priorityFlow);
    
    // Todo List: Label and Checkboxes
    rightStack->addChild(ui->label("Todo List:", 0, 0));
    ui::FlowContainer* todoListFlow = ui->flowContainer(0, 0, 524, 100);
    todoListFlow->addChild(ui->checkBox(false, 0, 0, [](bool state){
        std::cout << "Todo 'Buy milk' done: " << (state ? "Yes" : "No") << std::endl;
    }));
    todoListFlow->addChild(ui->label("Buy milk", 0, 0));
    todoListFlow->addChild(ui->checkBox(true, 0, 0, [](bool state){
        std::cout << "Todo 'Call Mom' done: " << (state ? "Yes" : "No") << std::endl;
    }));
    todoListFlow->addChild(ui->label("Call Mom", 0, 0));
    rightStack->addChild(todoListFlow);
    
    contentSplit->setRightPanel(rightStack);
    mainStack->addChild(contentSplit);
    
    // -------------------------------
    // Bottom Area: Images and Sprites
    // -------------------------------
    ui::FlowContainer* bottomFlow = ui->flowContainer(0, 550, 1024, 346);
    bottomFlow->addChild(ui->image("demos/smiley.png", 0, 0, 100, 100, true));
    bottomFlow->addChild(ui->image(smiley_png, smiley_png_len, 110, 0, 100, 100, true));
    bottomFlow->addChild(ui->animatedSprite("demos/sprite_sheet.png", 220, 0, 85, 85, 9, 100, true));
    bottomFlow->addChild(ui->animatedSprite(sprite_sheet_png, sprite_sheet_png_len, 310, 0, 85, 85, 9, 100, true));
    mainStack->addChild(bottomFlow);

    // Add the top-level mainStack to UI.
    ui->addElement(mainStack);  // The wrapper now accepts a pointer (non-owning), since UI core already manages lifetimes.

    // -------------------------------
    // Hotkey Registrations
    // -------------------------------
    ui->assignHotKey(ctxMenu, "f", [ctxMenu, uiPtr = ui.get()](){
        ctxMenu->activeItemIndex = 0;
        ctxMenu->expanded = true;
        uiPtr->setFocus(ctxMenu);
    });
    ui->assignHotKey(ctxMenu, "q", [ctxMenu]() {
        ctxMenu->activeItemIndex = 0;
        ctxMenu->expanded = true;
        if (!ctxMenu->items.empty() && ctxMenu->items[0].subCallbacks.size() >= 3)
            ctxMenu->items[0].subCallbacks[2]();
    });
    ui->assignHotKey(ctxMenu, "m", [ctxMenu]() {
        ctxMenu->activeItemIndex = 1;
        if (!ctxMenu->items.empty() && ctxMenu->items[1].subCallbacks.size() >= 4)
            ctxMenu->items[1].subCallbacks[3]();
    });
    // For demonstration, assign a hotkey to the rightStack.
    ui->assignHotKey(rightStack, "b", [](){
        std::cout << "Right stack hotkey activated." << std::endl;
    });
    
    ui->run();
    return 0;
}
