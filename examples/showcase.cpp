#include "uiframework/UI.h"
#include <memory>
#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#include <random>

class UIShowcaseApp {
private:
    std::unique_ptr<UI> ui;
    std::string currentTheme = "Default";
    std::mt19937 rng;
    
    // Demo elements
    std::shared_ptr<ui::ProgressBar> progressBar;
    std::shared_ptr<ui::Label> statusLabel;
    std::shared_ptr<ui::Canvas> drawingCanvas;
    std::shared_ptr<ui::ListView> logList;
    std::vector<std::string> logMessages;
    
public:
    UIShowcaseApp() : rng(std::random_device{}()) {
        try {
            ui = std::make_unique<UI>("UI Framework Showcase - Layout System Demo", 1200, 800);
            setupUI();
            addLogMessage("UI Framework Showcase initialized successfully!");
            std::cout << "Showcase application ready!" << std::endl;
        } catch (const ui::UIException& e) {
            std::cerr << "Failed to initialize showcase: " << e.what() << std::endl;
            throw;
        }
    }
    
    void run() {
        ui->run();
    }
    
private:
    void setupUI() {
        createMenuBar();
        createLayoutDemo();
        createElementsDemo();
        createLogPanel();
        
        addLogMessage("All UI components loaded successfully");
        addLogMessage("Framework features: Memory-safe, Thread-safe, Layout management");
        addLogMessage("Try different themes and interact with elements!");
    }
    
    void createMenuBar() {
        std::vector<ui::TopMenuItem> menus = {
            { "Themes", { 
                {"Default", [this](){ changeTheme("Default"); }},
                {"Solarized Dark", [this](){ changeTheme("SolarizedDark"); }},
                {"Solarized Light", [this](){ changeTheme("SolarizedLight"); }},
                {"Molokai", [this](){ changeTheme("Molokai"); }}
            }},
            { "Demos", { 
                {"Canvas Art", [this](){ generateRandomCanvasArt(); }},
                {"Progress Demo", [this](){ updateProgress(); }},
                {"Clear Log", [this](){ logMessages.clear(); if(logList) logList->setItems(logMessages); }}
            }},
            { "Help", { 
                {"About Framework", [this](){ showAboutDialog(); }}
            }}
        };
        
        auto contextMenu = ui->createContextMenu(menus);
        contextMenu->setPosition(0, 0);
        contextMenu->setSize(ui->getWidth(), 30);
    }
    
    void createLayoutDemo() {
        // Create main layout container using VBox
        auto mainLayout = ui->createVBoxLayout(20, 50, ui->getWidth() - 40, 300, 10);
        
        // Title
        auto title = ui->createLabel("Layout Management System Demo", 0, 0);
        mainLayout->addElement(title);
        
        // Create horizontal layout for buttons
        auto buttonLayout = ui->createHBoxLayout(0, 0, 600, 40, 10);
        
        auto btn1 = ui->createButton("Layout Button 1", 0, 0, [this](){
            addLogMessage("Layout Button 1 clicked!");
        });
        
        auto btn2 = ui->createButton("Layout Button 2", 0, 0, [this](){
            addLogMessage("Layout Button 2 clicked!");
        });
        
        auto btn3 = ui->createButton("Layout Button 3", 0, 0, [this](){
            addLogMessage("Layout Button 3 clicked!");
        });
        
        // Add buttons to horizontal layout with stretch
        buttonLayout->addElement(btn1, 1);
        buttonLayout->addElement(btn2, 1);
        buttonLayout->addElement(btn3, 1);
        
        // Add button layout to main layout
        mainLayout->addElement(buttonLayout);
        
        // Create grid layout demo
        auto gridTitle = ui->createLabel("Grid Layout Demo:", 0, 0);
        mainLayout->addElement(gridTitle);
        
        auto gridLayout = ui->createGridLayout(0, 0, 300, 120, 3, 3, 5);
        
        // Add numbered buttons to grid
        for (int i = 0; i < 9; i++) {
            auto gridBtn = ui->createButton(std::to_string(i + 1), 0, 0, [this, i](){
                addLogMessage("Grid button " + std::to_string(i + 1) + " clicked!");
            });
            gridBtn->setSize(30, 30);
            gridLayout->addElement(gridBtn);
        }
        
        mainLayout->addElement(gridLayout);
    }
    
    void createElementsDemo() {
        // Progress bar
        auto progressLabel = ui->createLabel("Progress Bar Demo:", 20, 380);
        progressBar = ui->createProgressBar(20, 410, 300, 25, 0.0f, true);
        
        // Canvas
        auto canvasLabel = ui->createLabel("Canvas Demo:", 350, 380);
        drawingCanvas = ui->createCanvas(350, 410, 200, 150);
        
        // Canvas controls
        auto drawBtn = ui->createButton("Draw Random", 350, 570, [this](){
            generateRandomCanvasArt();
        });
        
        auto clearBtn = ui->createButton("Clear Canvas", 460, 570, [this](){
            drawingCanvas->clear();
            addLogMessage("Canvas cleared");
        });
        
        // Input elements
        auto inputLabel = ui->createLabel("Input Elements:", 580, 380);
        
        auto textBox = ui->createTextBox("Type here...", 580, 410);
        textBox->setSize(200, 30);
        
        auto checkbox = ui->createCheckBox(false, 580, 450, [this](bool checked){
            addLogMessage(checked ? "Checkbox enabled!" : "Checkbox disabled!");
        });
        
        auto checkLabel = ui->createLabel("Enable feature", 610, 450);
        
        std::vector<std::string> options = {"Option A", "Option B", "Option C"};
        auto optionSelect = ui->createOptionSelect(0, options, 580, 480, [this](int index){
            addLogMessage("Selected option " + std::to_string(index));
        });
        optionSelect->setSize(200, 30);
        
        // Progress update button
        auto progressBtn = ui->createButton("Update Progress", 20, 450, [this](){
            updateProgress();
        });
    }
    
    void createLogPanel() {
        // Log panel at bottom
        auto logLabel = ui->createLabel("Activity Log:", 20, 620);
        
        logList = ui->createListView(logMessages, 20, 650, ui->getWidth() - 40, 100, 20);
        
        // Status bar
        statusLabel = ui->createLabel("Ready - UI Framework Showcase", 20, ui->getHeight() - 30);
    }
    
    void addLogMessage(const std::string& message) {
        logMessages.push_back("[" + std::to_string(logMessages.size() + 1) + "] " + message);
        if (logMessages.size() > 20) {
            logMessages.erase(logMessages.begin());
        }
        if (logList) {
            logList->setItems(logMessages);
            if (!logMessages.empty()) {
                logList->setSelectedIndex(static_cast<int>(logMessages.size()) - 1);
            }
        }
        if (statusLabel) {
            statusLabel->setText("Last: " + message);
        }
    }
    
    void changeTheme(const std::string& theme) {
        currentTheme = theme;
        ui->setTheme(theme);
        addLogMessage("Theme changed to: " + theme);
    }
    
    void updateProgress() {
        if (progressBar) {
            float current = progressBar->getProgress();
            current += 0.15f;
            if (current > 1.0f) current = 0.0f;
            progressBar->setProgress(current);
            addLogMessage("Progress updated to " + std::to_string(static_cast<int>(current * 100)) + "%");
        }
    }
    
    void generateRandomCanvasArt() {
        if (!drawingCanvas) return;
        
        drawingCanvas->clear();
        
        std::uniform_int_distribution<int> posDist(10, 180);
        std::uniform_int_distribution<int> sizeDist(10, 40);
        std::uniform_int_distribution<int> colorDist(0, 255);
        
        for (int i = 0; i < 10; i++) {
            uint8_t r = colorDist(rng);
            uint8_t g = colorDist(rng);
            uint8_t b = colorDist(rng);
            
            if (i % 2 == 0) {
                drawingCanvas->filledRectRel(posDist(rng), posDist(rng), sizeDist(rng), sizeDist(rng), {r, g, b, 255});
            } else {
                drawingCanvas->lineRel(posDist(rng), posDist(rng), posDist(rng), posDist(rng), {r, g, b, 255});
            }
        }
        
        addLogMessage("Generated random canvas art");
    }
    
    void showAboutDialog() {
        std::stringstream about;
        about << "UI Framework Showcase Application\n\n";
        about << "This demo showcases the refactored UI framework features:\n\n";
        about << "🔧 Core Features:\n";
        about << "• Memory-safe shared_ptr management\n";
        about << "• Thread-safe operations\n";
        about << "• Exception handling\n";
        about << "• RAII resource management\n\n";
        about << "📐 Layout System:\n";
        about << "• VBoxLayout - Vertical arrangement\n";
        about << "• HBoxLayout - Horizontal arrangement\n";
        about << "• GridLayout - Grid positioning\n";
        about << "• Automatic sizing and constraints\n\n";
        about << "🎨 UI Elements:\n";
        about << "• Buttons, Labels, TextBoxes\n";
        about << "• ListView, ProgressBar, Canvas\n";
        about << "• CheckBoxes, OptionSelect, Modals\n\n";
        about << "Built with SDL2, C++17, and modern design patterns.";
        
        ui->createInfoModal(about.str(), [this](){
            addLogMessage("About dialog closed");
        });
    }
};

int main() {
    try {
        UIShowcaseApp app;
        std::cout << "\n=== UI Framework Showcase ===" << std::endl;
        std::cout << "Demonstrating layout management and all framework features!" << std::endl;
        
        app.run();
    } catch (const std::exception& e) {
        std::cerr << "Showcase application error: " << e.what() << std::endl;
        return -1;
    }
    
    return 0;
}
