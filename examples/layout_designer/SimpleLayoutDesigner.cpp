#include "SimpleLayoutDesigner.h"
#include <iostream>
#include <sstream>

SimpleLayoutDesigner::SimpleLayoutDesigner() : ui("Simple Layout Designer", 1000, 700) {
    setupUI();
}

void SimpleLayoutDesigner::setupUI() {
    // Toolbar buttons - fixed spacing to prevent overlap
    addButtonBtn = ui.createButton("Add Button", 10, 10, [this]() { addButton(); });
    addLabelBtn = ui.createButton("Add Label", 120, 10, [this]() { addLabel(); });
    addTextBoxBtn = ui.createButton("Add TextBox", 220, 10, [this]() { addTextBox(); });
    addCanvasBtn = ui.createButton("Add Canvas", 330, 10, [this]() { addCanvas(); });
    showLayoutBtn = ui.createButton("Show Layout", 440, 10, [this]() { showLayout(); });
    clearAllBtn = ui.createButton("Clear All", 550, 10, [this]() { clearAll(); });
    
    // Instructions
    instructionsLabel = ui.createLabel("Click buttons to add rectangles. Drag-and-drop requires custom SDL event handling.", 10, 40);
    positionLabel = ui.createLabel("Note: Elements show type labels and can be moved via 'Show Layout' export", 10, 55);
    
    // Main canvas for drawing design rectangles
    canvas = ui.createCanvas(0, DESIGN_AREA_Y, 1000, 700 - DESIGN_AREA_Y);
    updateCanvas();
}

void SimpleLayoutDesigner::addButton() {
    static int count = 0;
    int x = 50 + (count * 20) % 400;
    int y = DESIGN_AREA_Y + 50 + (count * 20) % 300;
    
    designRects.emplace_back("Button", x, y, 100, 30, SDL_Color{100, 150, 255, 255});
    count++;
    updateCanvas();
}

void SimpleLayoutDesigner::addLabel() {
    static int count = 0;
    int x = 100 + (count * 25) % 400;
    int y = DESIGN_AREA_Y + 100 + (count * 25) % 300;
    
    designRects.emplace_back("Label", x, y, 80, 25, SDL_Color{150, 255, 150, 255});
    count++;
    updateCanvas();
}

void SimpleLayoutDesigner::addTextBox() {
    static int count = 0;
    int x = 150 + (count * 30) % 400;
    int y = DESIGN_AREA_Y + 150 + (count * 30) % 300;
    
    designRects.emplace_back("TextBox", x, y, 120, 25, SDL_Color{255, 200, 100, 255});
    count++;
    updateCanvas();
}

void SimpleLayoutDesigner::addCanvas() {
    static int count = 0;
    int x = 200 + (count * 35) % 400;
    int y = DESIGN_AREA_Y + 200 + (count * 35) % 300;
    
    designRects.emplace_back("Canvas", x, y, 100, 80, SDL_Color{200, 150, 200, 255});
    count++;
    updateCanvas();
}

void SimpleLayoutDesigner::updateCanvas() {
    canvas->clear();
    
    // Draw background
    canvas->filledRect({0, 0, 1000, 700 - DESIGN_AREA_Y}, {245, 245, 245, 255});
    
    // Draw all design rectangles - SIMPLE VERSION
    for (size_t i = 0; i < designRects.size(); ++i) {
        const auto& rect = designRects[i];
        
        int canvasX = rect.x;
        int canvasY = rect.y - DESIGN_AREA_Y;
        
        if (canvasY < 0) continue;
        
        // Draw simple filled rectangle
        canvas->filledRect({canvasX, canvasY, rect.width, rect.height}, 
                          {rect.color.r, rect.color.g, rect.color.b, rect.color.a});
        
        // Draw simple black border
        canvas->rect({canvasX, canvasY, rect.width, rect.height}, {0, 0, 0, 255});
    }
}

void SimpleLayoutDesigner::drawSimpleText(int x, int y, const std::string& text) {
    // Removed - keeping rectangles simple
}

void SimpleLayoutDesigner::showLayout() {
    std::cout << "\n=== SIMPLE LAYOUT DESIGN ===\n";
    std::cout << "Window Size: 1000x700\n";
    std::cout << "Design Area: y=" << DESIGN_AREA_Y << " (below toolbar)\n\n";
    
    for (size_t i = 0; i < designRects.size(); ++i) {
        const auto& rect = designRects[i];
        std::cout << "Element " << (i + 1) << ":\n";
        std::cout << "  Type: " << rect.type << "\n";
        std::cout << "  Position: (" << rect.x << ", " << rect.y << ")\n";
        std::cout << "  Size: " << rect.width << "x" << rect.height << "\n";
        
        // Generate code snippet
        std::cout << "  Code: ";
        if (rect.type == "Button") {
            std::cout << "ui.createButton(\"Button Text\", " << rect.x << ", " << rect.y << ", callback);";
        } else if (rect.type == "Label") {
            std::cout << "ui.createLabel(\"Label Text\", " << rect.x << ", " << rect.y << ");";
        } else if (rect.type == "TextBox") {
            std::cout << "ui.createTextBox(\"Default Text\", " << rect.x << ", " << rect.y << ");";
        } else if (rect.type == "Canvas") {
            std::cout << "ui.createCanvas(" << rect.x << ", " << rect.y << ", " << rect.width << ", " << rect.height << ");";
        }
        std::cout << "\n\n";
    }
    
    std::cout << "Total Elements: " << designRects.size() << "\n";
    std::cout << "============================\n\n";
}

void SimpleLayoutDesigner::clearAll() {
    designRects.clear();
    selectedRect = nullptr;
    updateCanvas();
    updatePositionLabel();
    std::cout << "All elements cleared!\n";
}

void SimpleLayoutDesigner::updatePositionLabel() {
    std::cout << "Elements: " << designRects.size();
    if (selectedRect) {
        std::cout << " | Selected: " << selectedRect->type 
                  << " at (" << selectedRect->x << ", " << selectedRect->y 
                  << ") size " << selectedRect->width << "x" << selectedRect->height;
    }
    std::cout << std::endl;
}

DesignRect* SimpleLayoutDesigner::findRectAt(int x, int y) {
    // Search in reverse order (top elements first)
    for (auto it = designRects.rbegin(); it != designRects.rend(); ++it) {
        if (it->contains(x, y)) {
            return &(*it);
        }
    }
    return nullptr;
}

bool SimpleLayoutDesigner::isInDesignArea(int x, int y) {
    return y >= DESIGN_AREA_Y;
}

void SimpleLayoutDesigner::run() {
    std::cout << "Simple Layout Designer started!\n";
    std::cout << "IMPORTANT: Drag-and-drop is NOT implemented because:\n";
    std::cout << "- The UI framework doesn't expose mouse events\n";
    std::cout << "- Canvas elements don't receive click events\n";
    std::cout << "- Would require modifying core framework code\n\n";
    std::cout << "Current functionality:\n";
    std::cout << "- Click toolbar buttons to add colored rectangles with text labels\n";
    std::cout << "- Each rectangle represents a UI element type\n";
    std::cout << "- Click 'Show Layout' to see positions and generate code\n";
    std::cout << "- Use 'Clear All' to remove all elements\n\n";
    std::cout << "To implement drag-and-drop, we would need to:\n";
    std::cout << "1. Create a standalone SDL application, OR\n";
    std::cout << "2. Modify the UI framework to expose mouse events, OR\n";
    std::cout << "3. Use keyboard shortcuts for element manipulation\n\n";
    
    ui.run();
}
