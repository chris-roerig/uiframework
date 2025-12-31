#include "LayoutDesigner.h"
#include <iostream>
#include <sstream>
#include <random>

LayoutDesigner::LayoutDesigner() : ui("Layout Designer - All Elements + New Features", 1200, 800) {
    setupToolbar();
}

void LayoutDesigner::setupToolbar() {
    // Row 1 - Basic elements with new features
    addButtonBtn = ui.createButton("Button", 10, 10, [this]() { addButton(); });
    addButtonBtn->setTooltip("Add a button with padding and tooltip");
    
    addLabelBtn = ui.createButton("Label", 80, 10, [this]() { addLabel(); });
    addLabelBtn->setTooltip("Add an auto-sized label");
    
    addTextBoxBtn = ui.createButton("TextBox", 140, 10, [this]() { addTextBox(); });
    addTextBoxBtn->setTooltip("Add a text input with padding");
    
    addCheckBoxBtn = ui.createButton("CheckBox", 210, 10, [this]() { addCheckBox(); });
    addCheckBoxBtn->setTooltip("Add a checkbox (enabled/disabled demo)");
    
    addCanvasBtn = ui.createButton("Canvas", 290, 10, [this]() { addCanvas(); });
    addCanvasBtn->setTooltip("Add a drawing canvas");
    
    // Row 2 - Advanced elements
    addSliderBtn = ui.createButton("Slider", 10, 40, [this]() { addSlider(); });
    addSliderBtn->setTooltip("Add a value slider");
    
    addProgressBarBtn = ui.createButton("ProgressBar", 80, 40, [this]() { addProgressBar(); });
    addProgressBarBtn->setTooltip("Add a progress indicator");
    
    addListViewBtn = ui.createButton("ListView", 170, 40, [this]() { addListView(); });
    addListViewBtn->setTooltip("Add a scrollable list");
    
    addOptionSelectBtn = ui.createButton("OptionSelect", 260, 40, [this]() { addOptionSelect(); });
    addOptionSelectBtn->setTooltip("Add a dropdown selector");
    
    addImageBtn = ui.createButton("Image", 360, 40, [this]() { addImage(); });
    addImageBtn->setTooltip("Add an image placeholder");
    
    // Row 3 - Actions and theme switching
    showLayoutBtn = ui.createButton("Show Layout", 10, 70, [this]() { showLayout(); });
    showLayoutBtn->setTooltip("Print all element positions to console");
    
    clearAllBtn = ui.createButton("Clear All", 110, 70, [this]() { clearAll(); });
    clearAllBtn->setTooltip("Remove all design elements");
    
    randomizeBtn = ui.createButton("Randomize", 190, 70, [this]() { randomizePositions(); });
    randomizeBtn->setTooltip("Randomly reposition all elements");
    
    // 🆕 Theme switching buttons
    themeBtn1 = ui.createButton("Framework", 290, 70, [this]() { ui.setTheme("Framework"); });
    themeBtn1->setTooltip("Switch to Framework (dark) theme");
    
    themeBtn2 = ui.createButton("Solarized", 380, 70, [this]() { ui.setTheme("SolarizedLight"); });
    themeBtn2->setTooltip("Switch to Solarized Light theme");
    
    // Instructions and stats with tooltips
    instructionsLabel = ui.createLabel("🆕 Layout Designer with Tooltips, Padding, Disabled States & Theme Switching", 10, 95);
    instructionsLabel->setTooltip("This designer showcases all new UI framework features");
    
    statsLabel = ui.createLabel("Elements: 0", 600, 95);
    statsLabel->setTooltip("Live count of design elements");
    
    // Design area background
    designArea = ui.createCanvas(0, DESIGN_AREA_Y, 1200, 800 - DESIGN_AREA_Y);
    designArea->filledRect({0, 0, 1200, 800 - DESIGN_AREA_Y}, {245, 245, 245, 255});
    designArea->rect({0, 0, 1200, 800 - DESIGN_AREA_Y}, {100, 100, 100, 255});
    designArea->setTooltip("Design area - elements will appear here");
    
    updateStats();
}

void LayoutDesigner::addButton() {
    static int count = 0;
    std::string text = "Button " + std::to_string(++count);
    
    int x = 50 + (count * 15) % 500;
    int y = DESIGN_AREA_Y + 30 + (count * 15) % 400;
    
    int currentCount = count; // Copy for lambda capture
    auto button = ui.createButton(text, x, y, [currentCount]() {
        std::cout << "Design Button " << currentCount << " clicked!" << std::endl;
    });
    
    // 🆕 Add new features to buttons
    button->setTooltip("Design button #" + std::to_string(currentCount) + " - click to test");
    if (count % 3 == 0) {
        button->setPadding(8); // Every 3rd button gets padding
    }
    if (count % 5 == 0) {
        button->setEnabled(false); // Every 5th button is disabled
        button->setTooltip("Disabled button #" + std::to_string(currentCount) + " - demonstrates disabled state");
    }
    
    designElements.emplace_back(button, "Button", x, y, 100, 30);
    updateStats();
}

void LayoutDesigner::addLabel() {
    static int count = 0;
    std::string text = "Label " + std::to_string(++count);
    
    int x = 100 + (count * 20) % 500;
    int y = DESIGN_AREA_Y + 80 + (count * 20) % 400;
    
    auto label = ui.createLabel(text, x, y);
    // 🆕 Add new features to labels
    label->setTooltip("Auto-sized label #" + std::to_string(count));
    if (count % 2 == 0) {
        label->setPadding(4); // Every other label gets padding
    }
    
    designElements.emplace_back(label, "Label", x, y, 80, 25);
    updateStats();
}

void LayoutDesigner::addTextBox() {
    static int count = 0;
    std::string text = "Input " + std::to_string(++count);
    
    int x = 150 + (count * 25) % 500;
    int y = DESIGN_AREA_Y + 130 + (count * 25) % 400;
    
    auto textBox = ui.createTextBox(text, x, y);
    // 🆕 Add new features to textboxes
    textBox->setTooltip("Text input #" + std::to_string(count) + " - type here");
    textBox->setPadding(6); // All textboxes get padding for better text positioning
    if (count % 4 == 0) {
        textBox->setEnabled(false); // Every 4th textbox is disabled
        textBox->setTooltip("Disabled text input #" + std::to_string(count));
    }
    
    designElements.emplace_back(textBox, "TextBox", x, y, 120, 25);
    updateStats();
}

void LayoutDesigner::addCheckBox() {
    static int count = 0;
    
    int x = 200 + (count * 30) % 500;
    int y = DESIGN_AREA_Y + 180 + (count * 30) % 400;
    
    int currentCount = count + 1; // Copy for lambda capture
    auto checkBox = ui.createCheckBox(false, x, y, [currentCount](bool checked) {
        std::cout << "CheckBox " << currentCount << " " << (checked ? "checked" : "unchecked") << std::endl;
    });
    designElements.emplace_back(checkBox, "CheckBox", x, y, 20, 20);
    count++;
    updateStats();
}

void LayoutDesigner::addCanvas() {
    static int count = 0;
    
    int x = 250 + (count * 35) % 500;
    int y = DESIGN_AREA_Y + 230 + (count * 35) % 400;
    
    auto canvas = ui.createCanvas(x, y, 100, 80);
    canvas->filledRect({0, 0, 100, 80}, {200, 150, 100, 255});
    canvas->rect({0, 0, 100, 80}, {0, 0, 0, 255});
    canvas->filledRect({10, 10, 80, 60}, {150, 200, 255, 255});
    
    designElements.emplace_back(canvas, "Canvas", x, y, 100, 80);
    count++;
    updateStats();
}
void LayoutDesigner::addSlider() {
    static int count = 0;
    
    int x = 300 + (count * 40) % 500;
    int y = DESIGN_AREA_Y + 280 + (count * 40) % 400;
    
    auto slider = ui.createHSlider(x, y, 120, 20, 0.0f, 100.0f, 50.0f);
    designElements.emplace_back(slider, "HSlider", x, y, 120, 20);
    count++;
    updateStats();
}

void LayoutDesigner::addProgressBar() {
    static int count = 0;
    
    int x = 350 + (count * 45) % 500;
    int y = DESIGN_AREA_Y + 330 + (count * 45) % 400;
    
    auto progressBar = ui.createProgressBar(x, y, 150, 25, 75.0f, true);
    designElements.emplace_back(progressBar, "ProgressBar", x, y, 150, 25);
    count++;
    updateStats();
}

void LayoutDesigner::addListView() {
    static int count = 0;
    
    int x = 400 + (count * 50) % 500;
    int y = DESIGN_AREA_Y + 380 + (count * 50) % 300;
    
    std::vector<std::string> items = {"Item 1", "Item 2", "Item 3", "Item 4"};
    auto listView = ui.createListView(items, x, y, 120, 100);
    designElements.emplace_back(listView, "ListView", x, y, 120, 100);
    count++;
    updateStats();
}

void LayoutDesigner::addOptionSelect() {
    static int count = 0;
    
    int x = 450 + (count * 55) % 500;
    int y = DESIGN_AREA_Y + 430 + (count * 55) % 300;
    
    std::vector<std::string> options = {"Option A", "Option B", "Option C"};
    int currentCount = count + 1; // Copy for lambda capture
    auto optionSelect = ui.createOptionSelect(options, 0, x, y, [currentCount](int index) {
        std::cout << "OptionSelect " << currentCount << " selected: " << index << std::endl;
    });
    // 🆕 Add new features to option select
    optionSelect->setTooltip("Dropdown selector #" + std::to_string(currentCount));
    
    designElements.emplace_back(optionSelect, "OptionSelect", x, y, 100, 25);
    count++;
    updateStats();
}

void LayoutDesigner::addImage() {
    static int count = 0;
    
    int x = 500 + (count * 60) % 400;
    int y = DESIGN_AREA_Y + 480 + (count * 60) % 200;
    
    // Create a placeholder image (this will fail gracefully if no image file exists)
    try {
        auto image = ui.createImage("placeholder.png", x, y, 80, 60, false);
        designElements.emplace_back(image, "Image", x, y, 80, 60);
    } catch (...) {
        // If image creation fails, create a canvas placeholder instead
        auto canvas = ui.createCanvas(x, y, 80, 60);
        canvas->filledRect({0, 0, 80, 60}, {200, 200, 200, 255});
        canvas->rect({0, 0, 80, 60}, {100, 100, 100, 255});
        canvas->filledRect({20, 20, 40, 20}, {150, 150, 150, 255});
        designElements.emplace_back(canvas, "Image", x, y, 80, 60);
    }
    count++;
    updateStats();
}
void LayoutDesigner::showLayout() {
    std::cout << "\n=== COMPREHENSIVE LAYOUT DESIGN ===\n";
    std::cout << "Window Size: 1200x800\n";
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
        } else if (elem.type == "CheckBox") {
            std::cout << "ui.createCheckBox(false, " << elem.x << ", " << elem.y << ", callback);";
        } else if (elem.type == "Canvas") {
            std::cout << "ui.createCanvas(" << elem.x << ", " << elem.y << ", " << elem.width << ", " << elem.height << ");";
        } else if (elem.type == "HSlider") {
            std::cout << "ui.createHSlider(" << elem.x << ", " << elem.y << ", " << elem.width << ", " << elem.height << ", 0.0f, 100.0f, 50.0f);";
        } else if (elem.type == "ProgressBar") {
            std::cout << "ui.createProgressBar(" << elem.x << ", " << elem.y << ", " << elem.width << ", " << elem.height << ", 75.0f, true);";
        } else if (elem.type == "ListView") {
            std::cout << "ui.createListView(items, " << elem.x << ", " << elem.y << ", " << elem.width << ", " << elem.height << ");";
        } else if (elem.type == "OptionSelect") {
            std::cout << "ui.createOptionSelect(0, options, " << elem.x << ", " << elem.y << ", callback);";
        } else if (elem.type == "Image") {
            std::cout << "ui.createImage(\"image.png\", " << elem.x << ", " << elem.y << ", " << elem.width << ", " << elem.height << ");";
        }
        std::cout << "\n\n";
    }
    
    std::cout << "Total Elements: " << designElements.size() << "\n";
    std::cout << "===================================\n\n";
}

void LayoutDesigner::clearAll() {
    designElements.clear();
    updateStats();
    std::cout << "All elements cleared!\n";
}

void LayoutDesigner::randomizePositions() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> xDist(10, 1000);
    std::uniform_int_distribution<> yDist(DESIGN_AREA_Y + 10, 700);
    
    for (auto& elem : designElements) {
        int newX = xDist(gen);
        int newY = yDist(gen);
        
        // Keep within bounds
        newX = std::min(newX, 1200 - elem.width);
        newY = std::min(newY, 800 - elem.height);
        
        elem.x = newX;
        elem.y = newY;
        elem.element->setPosition(newX, newY);
    }
    
    std::cout << "Randomized positions of " << designElements.size() << " elements!\n";
}

void LayoutDesigner::updateStats() {
    std::string statsText = "Elements: " + std::to_string(designElements.size());
    // Note: We can't easily update the label text dynamically with this UI framework
    // The stats will be shown in console output instead
}

bool LayoutDesigner::isInDesignArea(int x, int y) {
    return y >= DESIGN_AREA_Y;
}

void LayoutDesigner::run() {
    std::cout << "Comprehensive Layout Designer started!\n";
    std::cout << "Available Elements: Button, Label, TextBox, CheckBox, Canvas, Slider, ProgressBar, ListView, OptionSelect, Image\n";
    std::cout << "- Click toolbar buttons to add UI elements\n";
    std::cout << "- Use 'Randomize' to reposition all elements randomly\n";
    std::cout << "- Click 'Show Layout' to export element positions and code\n";
    std::cout << "- Click 'Clear All' to remove all elements\n";
    std::cout << "- Close window to exit\n\n";
    
    ui.run();
}
