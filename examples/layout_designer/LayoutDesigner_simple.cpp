#include "LayoutDesigner.h"
#include <iostream>
#include <sstream>

LayoutDesigner::LayoutDesigner() : ui("Layout Designer", 1000, 700) {
    setupToolbar();
}

void LayoutDesigner::setupToolbar() {
    // Toolbar buttons
    addButtonBtn = ui.createButton("Add Button", 10, 10, [this]() { addButton(); });
    addLabelBtn = ui.createButton("Add Label", 120, 10, [this]() { addLabel(); });
    addTextBoxBtn = ui.createButton("Add TextBox", 220, 10, [this]() { addTextBox(); });
    addCanvasBtn = ui.createButton("Add Canvas", 330, 10, [this]() { addCanvas(); });
    showLayoutBtn = ui.createButton("Show Layout", 440, 10, [this]() { showLayout(); });
    
    // Instructions
    instructionsLabel = ui.createLabel("Click buttons to add elements. Elements will be placed automatically.", 10, 45);
    
    // Design area background
    designArea = ui.createCanvas(0, DESIGN_AREA_Y, 1000, 700 - DESIGN_AREA_Y);
    designArea->filledRect({0, 0, 1000, 700 - DESIGN_AREA_Y}, {240, 240, 240, 255});
    designArea->rect({0, 0, 1000, 700 - DESIGN_AREA_Y}, {100, 100, 100, 255});
}

void LayoutDesigner::addButton() {
    static int buttonCount = 0;
    std::string text = "Button " + std::to_string(++buttonCount);
    
    int x = 100 + (buttonCount * 20) % 400;
    int y = DESIGN_AREA_Y + 50 + (buttonCount * 20) % 300;
    
    auto button = ui.createButton(text, x, y, []() {
        std::cout << "Design button clicked!" << std::endl;
    });
    designElements.emplace_back(button, "Button", x, y, 100, 30);
}

void LayoutDesigner::addLabel() {
    static int labelCount = 0;
    std::string text = "Label " + std::to_string(++labelCount);
    
    int x = 150 + (labelCount * 25) % 400;
    int y = DESIGN_AREA_Y + 100 + (labelCount * 25) % 300;
    
    auto label = ui.createLabel(text, x, y);
    designElements.emplace_back(label, "Label", x, y, 80, 25);
}

void LayoutDesigner::addTextBox() {
    static int inputCount = 0;
    std::string text = "Input " + std::to_string(++inputCount);
    
    int x = 200 + (inputCount * 30) % 400;
    int y = DESIGN_AREA_Y + 150 + (inputCount * 30) % 300;
    
    auto input = ui.createTextBox(text, x, y);
    designElements.emplace_back(input, "TextBox", x, y, 120, 25);
}

void LayoutDesigner::addCanvas() {
    static int canvasCount = 0;
    
    int x = 250 + (canvasCount * 35) % 400;
    int y = DESIGN_AREA_Y + 200 + (canvasCount * 35) % 300;
    
    auto canvas = ui.createCanvas(x, y, 100, 80);
    // Draw a simple pattern on the canvas
    canvas->filledRect({0, 0, 100, 80}, {200, 150, 100, 255});
    canvas->rect({0, 0, 100, 80}, {0, 0, 0, 255});
    canvas->filledRect({10, 10, 80, 60}, {150, 200, 255, 255});
    
    designElements.emplace_back(canvas, "Canvas", x, y, 100, 80);
    canvasCount++;
}

void LayoutDesigner::showLayout() {
    std::cout << "\n=== LAYOUT DESIGN ===\n";
    std::cout << "Window Size: 1000x700\n";
    std::cout << "Design Area: y=" << DESIGN_AREA_Y << " (below toolbar)\n\n";
    
    for (size_t i = 0; i < designElements.size(); ++i) {
        const auto& elem = designElements[i];
        std::cout << "Element " << (i + 1) << ":\n";
        std::cout << "  Type: " << elem.type << "\n";
        std::cout << "  Position: (" << elem.x << ", " << elem.y << ")\n";
        std::cout << "  Size: " << elem.width << "x" << elem.height << "\n";
        
        // Generate code snippet
        std::cout << "  Code: ";
        if (elem.type == "Button") {
            std::cout << "ui.createButton(\"Button Text\", " << elem.x << ", " << elem.y << ", callback);";
        } else if (elem.type == "Label") {
            std::cout << "ui.createLabel(\"Label Text\", " << elem.x << ", " << elem.y << ");";
        } else if (elem.type == "TextBox") {
            std::cout << "ui.createTextBox(\"Default Text\", " << elem.x << ", " << elem.y << ");";
        } else if (elem.type == "Canvas") {
            std::cout << "ui.createCanvas(" << elem.x << ", " << elem.y << ", " << elem.width << ", " << elem.height << ");";
        }
        std::cout << "\n\n";
    }
    
    std::cout << "Total Elements: " << designElements.size() << "\n";
    std::cout << "=====================\n\n";
}

DesignElement* LayoutDesigner::findElementAt(int x, int y) {
    // Search in reverse order (top elements first)
    for (auto it = designElements.rbegin(); it != designElements.rend(); ++it) {
        if (x >= it->x && x < it->x + it->width &&
            y >= it->y && y < it->y + it->height) {
            return &(*it);
        }
    }
    return nullptr;
}

bool LayoutDesigner::isInDesignArea(int x, int y) {
    return y >= DESIGN_AREA_Y;
}

void LayoutDesigner::run() {
    std::cout << "Layout Designer started!\n";
    std::cout << "- Click toolbar buttons to add UI elements\n";
    std::cout << "- Elements are placed automatically in the design area\n";
    std::cout << "- Click 'Show Layout' to export element positions\n";
    std::cout << "- Close window to exit\n\n";
    
    ui.run();
}
