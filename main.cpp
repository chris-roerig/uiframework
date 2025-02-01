#include "UI.h"
#include <memory>
#include <vector>
#include <iostream>
#include <SDL2/SDL.h>
#include "ThemeGlobals.h"  // Provides declaration for ui::g_currentTheme

// Structure to hold a todo item.
struct TodoItem {
    std::string text;
    bool done;
};

std::vector<TodoItem> todos; // Global vector for demo purposes.

int main() {
    // Create our UI façade with a window wide enough for our layout.
    auto ui = std::make_unique<UI>("TODO App", 800, 600);
    
    // --- Top: Theme Selection ---
    ui->label("Select Theme:", 50, 20);
    std::vector<std::string> themes = { "FrameworkDefault", "SolarizedDark", "SolarizedLight", "Molokai" };
    ui->optionSelect(0, themes, 200, 15, [uiPtr = ui.get()](int idx) {
         switch(idx) {
             case 0: uiPtr->setTheme("FrameworkDefault"); break;
             case 1: uiPtr->setTheme("SolarizedDark"); break;
             case 2: uiPtr->setTheme("SolarizedLight"); break;
             case 3: uiPtr->setTheme("Molokai"); break;
         }
         std::cout << "Theme changed to index: " << idx << std::endl;
    });
    
    // --- Middle: New TODO Input ---
    ui->label("New TODO:", 50, 175);
    // Instead of using the convenience textBox, we create one manually so we can keep a pointer.
    auto inputBox = std::make_shared<ui::TextBox>(50, 175, 300, 40);
    inputBox->content = "Enter new task...";
    ui->addElement(inputBox);
    
    ui->button("Add TODO", 470, 175, [uiPtr = ui.get(), inputBox]() {
         std::string task = inputBox->content;
         if (task.empty() || task == "Enter new task...") {
             std::cout << "No task entered." << std::endl;
             return;
         }
         // Add the new task to our vector.
         todos.push_back({ task, false });
         std::cout << "Added TODO: " << task << std::endl;
         // Clear the input.
         inputBox->content = "";
         
         // Calculate the y-coordinate for the new todo item based on count.
         int newY = 150 + static_cast<int>(todos.size()) * 50;
         
         // Create a checkbox for the new todo.
         auto todoCheck = std::make_shared<ui::CheckBox>(50, newY, 30, false, [task](bool done) {
             std::cout << "Task \"" << task << "\" marked as " << (done ? "done" : "not done") << std::endl;
         });
         // Create a label to show the todo text.
         auto todoLabel = std::make_shared<ui::Label>(90, newY, task);
         
         // Add these new elements to the UI.
         uiPtr->addElement(todoCheck);
         uiPtr->addElement(todoLabel);
    });
    
    // --- Bottom: (Optional) Canvas as a Separator ---
    auto separator = ui->canvas(40, 300, 700, 2);
    separator->drawCommands.push_back([](SDL_Renderer* renderer) {
         SDL_Rect rect = { 0, 0, 700, 2 };
         SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255);
         SDL_RenderFillRect(renderer, &rect);
    });
    
    ui->run();
    return 0;
}
