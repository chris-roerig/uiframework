#include "LayoutDesigner.h"
#include <iostream>
#include <sstream>

LayoutDesigner::LayoutDesigner() : ui("Layout Designer", 1000, 700) {
    setupToolbar();
    
    // Set up mouse event handlers
    ui.setMouseDownCallback([this](int x, int y) { handleMouseDown(x, y); });
    ui.setMouseUpCallback([this](int x, int y) { handleMouseUp(x, y); });
    ui.setMouseMoveCallback([this](int x, int y) { handleMouseMove(x, y); });
}

void LayoutDesigner::setupToolbar() {
    // Toolbar buttons
    addButtonBtn = ui.createButton("Add Button", 10, 10, [this]() { addButton(); });
    addLabelBtn = ui.createButton("Add Label", 120, 10, [this]() { addLabel(); });
    addTextInputBtn = ui.createButton("Add TextBox", 220, 10, [this]() { addTextInput(); });
    addCanvasBtn = ui.createButton("Add Canvas", 340, 10, [this]() { addCanvas(); });
    showLayoutBtn = ui.createButton("Show Layout", 450, 10, [this]() { showLayout(); });
    
    // Instructions
    instructionsLabel = ui.createLabel("Drag elements to move, drag corners to resize", 10, 45);
    
    // Design area separator line
    auto separator = ui.createCanvas(0, TOOLBAR_HEIGHT, 1000, 2);
    separator->setDrawCallback([](ui::Canvas* canvas) {
        canvas->setColor(100, 100, 100, 255);
        canvas->filledRect(0, 0, 1000, 2);
    });
}

void LayoutDesigner::addButton() {
    static int buttonCount = 0;
    std::string text = "Button " + std::to_string(++buttonCount);
    
    int x = 100 + (buttonCount * 20) % 400;
    int y = DESIGN_AREA_Y + 50 + (buttonCount * 20) % 300;
    
    auto button = ui.createButton(text, x, y, []() {});
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

void LayoutDesigner::addTextInput() {
    static int inputCount = 0;
    
    int x = 200 + (inputCount * 30) % 400;
    int y = DESIGN_AREA_Y + 150 + (inputCount * 30) % 300;
    
    auto input = ui.createTextBox(x, y, 120, 25);
    input->setText("Input " + std::to_string(++inputCount));
    designElements.emplace_back(input, "TextBox", x, y, 120, 25);
}

void LayoutDesigner::addCanvas() {
    static int canvasCount = 0;
    
    int x = 250 + (canvasCount * 35) % 400;
    int y = DESIGN_AREA_Y + 200 + (canvasCount * 35) % 300;
    
    auto canvas = ui.createCanvas(x, y, 100, 80);
    canvas->setDrawCallback([canvasCount](ui::Canvas* c) {
        c->setColor(200, 150, 100, 255);
        c->filledRect(0, 0, 100, 80);
        c->setColor(0, 0, 0, 255);
        c->rect(0, 0, 100, 80);
        
        std::string text = "Canvas " + std::to_string(canvasCount + 1);
        // Simple text positioning (approximate)
        c->setColor(0, 0, 0, 255);
    });
    
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
            std::cout << "ui.createTextBox(" << elem.x << ", " << elem.y << ", " << elem.width << ", " << elem.height << ");";
        } else if (elem.type == "Canvas") {
            std::cout << "ui.createCanvas(" << elem.x << ", " << elem.y << ", " << elem.width << ", " << elem.height << ");";
        }
        std::cout << "\n\n";
    }
    
    std::cout << "Total Elements: " << designElements.size() << "\n";
    std::cout << "=====================\n\n";
}

void LayoutDesigner::handleMouseDown(int x, int y) {
    mouseX = x;
    mouseY = y;
    mouseDown = true;
    
    if (!isInDesignArea(x, y)) return;
    
    draggedElement = findElementAt(x, y);
    if (draggedElement) {
        draggedElement->isDragging = true;
        draggedElement->dragOffsetX = x - draggedElement->x;
        draggedElement->dragOffsetY = y - draggedElement->y;
    }
}

void LayoutDesigner::handleMouseUp(int x, int y) {
    mouseDown = false;
    
    if (draggedElement) {
        draggedElement->isDragging = false;
        draggedElement = nullptr;
    }
}

void LayoutDesigner::handleMouseMove(int x, int y) {
    mouseX = x;
    mouseY = y;
    
    if (!mouseDown || !draggedElement || !draggedElement->isDragging) return;
    
    // Update element position
    int newX = x - draggedElement->dragOffsetX;
    int newY = y - draggedElement->dragOffsetY;
    
    // Keep in design area bounds
    newX = std::max(0, std::min(newX, 1000 - draggedElement->width));
    newY = std::max(DESIGN_AREA_Y, std::min(newY, 700 - draggedElement->height));
    
    draggedElement->x = newX;
    draggedElement->y = newY;
    
    // Update the actual UI element position
    draggedElement->element->setPosition(newX, newY);
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
    ui.run();
}
