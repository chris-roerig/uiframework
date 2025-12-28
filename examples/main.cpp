#include "../src/UI.h"
#include <memory>
#include <vector>
#include <iostream>
#include <string>
#include <sstream>

class DemoApplication {
private:
    std::unique_ptr<UI> ui;
    
    // Demo state
    std::vector<std::string> todoItems;
    std::string currentTheme = "Default";
    int todoCounter = 1;
    
    // UI Elements (stored as shared_ptr for easy access)
    std::shared_ptr<ui::TextBox> todoInput;
    std::shared_ptr<ui::ListView> todoList;
    std::shared_ptr<ui::Label> statusLabel;
    std::shared_ptr<ui::ProgressBar> progressBar;
    std::shared_ptr<ui::CheckBox> darkModeCheck;
    std::shared_ptr<ui::OptionSelect> themeSelect;
    std::shared_ptr<ui::Canvas> drawingCanvas;
    
public:
    DemoApplication() {
        try {
            ui = std::make_unique<UI>("UI Framework Demo - Refactored", 1200, 800);
            setupUI();
            std::cout << "Demo application initialized successfully!" << std::endl;
        } catch (const ui::UIException& e) {
            std::cerr << "Failed to initialize demo: " << e.what() << std::endl;
            throw;
        }
    }
    
    void run() {
        ui->run();
    }
    
private:
    void setupUI() {
        createMenuBar();
        createTodoSection();
        createControlsSection();
        createCanvasSection();
        createStatusSection();
        
        // Set initial focus
        ui->setFocus(todoInput);
        
        // Setup hotkeys
        setupHotkeys();
        
        updateStatus("Demo application ready!");
    }
    
    void createMenuBar() {
        std::vector<ui::TopMenuItem> menus = {
            { "File", { 
                {"New Todo", [this](){ addTodoItem("New Task " + std::to_string(todoCounter++)); }}, 
                {"Clear All", [this](){ clearAllTodos(); }},
                {"---", nullptr}, // Separator (implementation dependent)
                {"Quit", [this](){ showQuitConfirmation(); }}
            }},
            { "Theme", { 
                {"Default", [this](){ changeTheme("Default"); }},
                {"Solarized Dark", [this](){ changeTheme("SolarizedDark"); }},
                {"Solarized Light", [this](){ changeTheme("SolarizedLight"); }},
                {"Molokai", [this](){ changeTheme("Molokai"); }}
            }},
            { "Help", { 
                {"About", [this](){ showAboutDialog(); }},
                {"Controls", [this](){ showControlsDialog(); }}
            }}
        };
        
        auto contextMenu = ui->createContextMenu(menus);
        contextMenu->setPosition(0, 0);
        contextMenu->setSize(ui->getWidth(), 30);
    }
    
    void createTodoSection() {
        // Todo section title
        auto todoTitle = ui->createLabel("Todo List Demo", 20, 50);
        
        // Todo input
        todoInput = ui->createTextBox("Enter new todo item...", 20, 80, true);
        todoInput->setSize(300, 30);
        
        // Add button
        auto addButton = ui->createButton("Add Todo", 330, 80, [this](){
            std::string text = todoInput->getText();
            if (!text.empty() && text != "Enter new todo item...") {
                addTodoItem(text);
                todoInput->clear();
                ui->setFocus(todoInput);
            }
        });
        addButton->setSize(100, 30);
        
        // Todo list
        todoList = ui->createListView(todoItems, 20, 120, 410, 200, 25);
        
        // List controls
        auto removeButton = ui->createButton("Remove Selected", 20, 330, [this](){
            int selected = todoList->getSelectedIndex();
            if (selected >= 0 && selected < static_cast<int>(todoItems.size())) {
                todoItems.erase(todoItems.begin() + selected);
                todoList->setItems(todoItems);
                updateProgress();
            }
        });
        removeButton->setSize(120, 30);
        
        auto clearButton = ui->createButton("Clear All", 150, 330, [this](){
            clearAllTodos();
        });
        clearButton->setSize(80, 30);
        
        // Mark complete checkbox
        auto completeCheck = ui->createCheckBox(false, 240, 335, [this](bool checked){
            if (checked) {
                int selected = todoList->getSelectedIndex();
                if (selected >= 0 && selected < static_cast<int>(todoItems.size())) {
                    todoItems[selected] = "✓ " + todoItems[selected];
                    todoList->setItems(todoItems);
                    updateProgress();
                }
            }
        });
        
        auto completeLabel = ui->createLabel("Mark Complete", 265, 335);
    }
    
    void createControlsSection() {
        // Controls section
        auto controlsTitle = ui->createLabel("Controls Demo", 500, 50);
        
        // Theme selector
        auto themeLabel = ui->createLabel("Theme:", 500, 80);
        std::vector<std::string> themes = {"Default", "SolarizedDark", "SolarizedLight", "Molokai"};
        themeSelect = ui->createOptionSelect(0, themes, 550, 80, [this](int index){
            std::vector<std::string> themeNames = {"Default", "SolarizedDark", "SolarizedLight", "Molokai"};
            if (index >= 0 && index < static_cast<int>(themeNames.size())) {
                changeTheme(themeNames[index]);
            }
        });
        themeSelect->setSize(150, 30);
        
        // Dark mode toggle
        darkModeCheck = ui->createCheckBox(false, 500, 120, [this](bool checked){
            if (checked) {
                changeTheme("SolarizedDark");
            } else {
                changeTheme("Default");
            }
        });
        auto darkModeLabel = ui->createLabel("Dark Mode", 525, 120);
        
        // Progress bar demo
        auto progressLabel = ui->createLabel("Progress:", 500, 160);
        progressBar = ui->createProgressBar(500, 180, 200, 25, 0.0f, true);
        
        auto progressButton = ui->createButton("Update Progress", 500, 215, [this](){
            float current = progressBar->getProgress();
            progressBar->setProgress(current + 0.1f);
            if (progressBar->isComplete()) {
                progressBar->reset();
                updateStatus("Progress reset!");
            }
        });
        progressButton->setSize(120, 30);
        
        // Test buttons
        auto infoButton = ui->createButton("Info Dialog", 500, 255, [this](){
            ui->createInfoModal("This is an information dialog!\n\nIt demonstrates the modal system.", [this](){
                updateStatus("Info dialog closed");
            });
        });
        infoButton->setSize(100, 30);
        
        auto confirmButton = ui->createButton("Confirm Dialog", 610, 255, [this](){
            ui->createConfirmModal("Do you want to proceed?", 
                [this](){ updateStatus("User confirmed!"); },
                [this](){ updateStatus("User cancelled!"); }
            );
        });
        confirmButton->setSize(120, 30);
    }
    
    void createCanvasSection() {
        // Canvas section
        auto canvasTitle = ui->createLabel("Canvas Demo", 500, 300);
        
        drawingCanvas = ui->createCanvas(500, 320, 200, 150);
        
        // Drawing buttons
        auto drawRectButton = ui->createButton("Draw Rect", 500, 480, [this](){
            drawingCanvas->filledRectRel(10, 10, 50, 30, {255, 0, 0, 255});
        });
        drawRectButton->setSize(80, 25);
        
        auto drawLineButton = ui->createButton("Draw Line", 590, 480, [this](){
            drawingCanvas->lineRel(0, 0, 200, 150, {0, 255, 0, 255});
        });
        drawLineButton->setSize(80, 25);
        
        auto clearCanvasButton = ui->createButton("Clear", 680, 480, [this](){
            drawingCanvas->clear();
        });
        clearCanvasButton->setSize(60, 25);
    }
    
    void createStatusSection() {
        // Status bar
        statusLabel = ui->createLabel("Ready", 20, ui->getHeight() - 30);
        statusLabel->setSize(ui->getWidth() - 40, 25);
        
        // Version info
        auto versionLabel = ui->createLabel("UI Framework v2.0 - Refactored", ui->getWidth() - 250, ui->getHeight() - 30);
    }
    
    void setupHotkeys() {
        // Global hotkeys
        ui->assignHotKey(todoInput, "n"); // Focus new todo input
        
        // Button hotkeys - need to use the 3-parameter version for custom callbacks
        ui->assignHotKey(todoInput->getId(), "q", [this](){ showQuitConfirmation(); });
        ui->assignHotKey(todoInput->getId(), "t", [this](){ 
            // Cycle through themes
            std::vector<std::string> themes = {"Default", "SolarizedDark", "SolarizedLight", "Molokai"};
            auto it = std::find(themes.begin(), themes.end(), currentTheme);
            if (it != themes.end()) {
                int index = (std::distance(themes.begin(), it) + 1) % themes.size();
                changeTheme(themes[index]);
            }
        });
    }
    
    void addTodoItem(const std::string& item) {
        todoItems.push_back(item);
        todoList->setItems(todoItems);
        updateProgress();
        updateStatus("Added: " + item);
    }
    
    void clearAllTodos() {
        if (!todoItems.empty()) {
            ui->createConfirmModal("Clear all todo items?",
                [this](){
                    todoItems.clear();
                    todoList->setItems(todoItems);
                    updateProgress();
                    updateStatus("All todos cleared");
                },
                [this](){
                    updateStatus("Clear cancelled");
                }
            );
        }
    }
    
    void changeTheme(const std::string& theme) {
        currentTheme = theme;
        ui->setTheme(theme);
        
        // Update theme selector
        std::vector<std::string> themes = {"Default", "SolarizedDark", "SolarizedLight", "Molokai"};
        auto it = std::find(themes.begin(), themes.end(), theme);
        if (it != themes.end()) {
            int index = std::distance(themes.begin(), it);
            themeSelect->setSelectedIndex(index);
        }
        
        // Update dark mode checkbox
        darkModeCheck->setChecked(theme == "SolarizedDark");
        
        updateStatus("Theme changed to: " + theme);
    }
    
    void updateProgress() {
        if (todoItems.empty()) {
            progressBar->setProgress(0.0f);
        } else {
            int completed = 0;
            for (const auto& item : todoItems) {
                if (item.find("✓") == 0) {
                    completed++;
                }
            }
            float progress = static_cast<float>(completed) / todoItems.size();
            progressBar->setProgress(progress);
        }
    }
    
    void updateStatus(const std::string& message) {
        if (statusLabel) {
            statusLabel->setText(message);
            std::cout << "Status: " << message << std::endl;
        }
    }
    
    void showQuitConfirmation() {
        ui->createConfirmModal("Are you sure you want to quit?",
            [](){ exit(0); },
            [this](){ updateStatus("Quit cancelled"); }
        );
    }
    
    void showAboutDialog() {
        std::stringstream about;
        about << "UI Framework Demo Application\n\n";
        about << "This demo showcases the refactored UI framework with:\n";
        about << "• Memory-safe shared_ptr management\n";
        about << "• Thread-safe operations\n";
        about << "• Comprehensive error handling\n";
        about << "• Modern C++17 features\n";
        about << "• Multiple themes\n";
        about << "• Rich UI elements\n\n";
        about << "Built with SDL2, C++17, and modern design patterns.";
        
        ui->createInfoModal(about.str(), [this](){
            updateStatus("About dialog closed");
        });
    }
    
    void showControlsDialog() {
        std::stringstream controls;
        controls << "Keyboard Controls:\n\n";
        controls << "N - Focus todo input\n";
        controls << "T - Cycle themes\n";
        controls << "Q - Quit application\n";
        controls << "Tab - Navigate between elements\n";
        controls << "Enter - Activate focused element\n";
        controls << "Space - Toggle checkboxes\n";
        controls << "Arrow keys - Navigate lists/menus\n";
        
        ui->createInfoModal(controls.str(), [this](){
            updateStatus("Controls dialog closed");
        });
    }
};

int main() {
    try {
        DemoApplication app;
        app.run();
    } catch (const std::exception& e) {
        std::cerr << "Application error: " << e.what() << std::endl;
        return -1;
    }
    
    return 0;
}
