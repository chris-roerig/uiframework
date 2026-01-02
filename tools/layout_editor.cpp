#include "uiframework/UI.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
#include <algorithm>
#include <map>

struct WireframeElement {
    std::string type;
    std::string id;
    int x, y, width, height;
    std::string text;
};

struct UndoAction {
    enum Type { ADD_ELEMENT, REMOVE_ELEMENT, MODIFY_ELEMENT, CLEAR_ALL };
    Type type;
    WireframeElement element;
    WireframeElement oldElement; // For modify operations
    int index; // For remove operations
};

class LayoutEditor {
private:
    UI* ui;
    std::vector<WireframeElement> wireframeElements;
    std::shared_ptr<ui::UIElement> selectedElement;
    WireframeElement* selectedWireframeElement = nullptr;
    std::map<std::string, int> elementCounters; // For auto-naming
    std::vector<std::shared_ptr<ui::UIElement>> canvasElements;
    std::vector<std::shared_ptr<ui::UIElement>> elementNameLabels; // Labels showing element names
    std::shared_ptr<ui::UIElement> selectedIndicator;
    std::vector<std::shared_ptr<ui::UIElement>> resizeHandles;
    std::vector<std::shared_ptr<ui::UIElement>> selectionButtons;
    
    // Properties panel elements
    std::shared_ptr<ui::TextBox> propNameInput;
    std::shared_ptr<ui::TextBox> propXInput;
    std::shared_ptr<ui::TextBox> propYInput;
    std::shared_ptr<ui::TextBox> propWidthInput;
    std::shared_ptr<ui::TextBox> propHeightInput;
    std::shared_ptr<ui::TextBox> propTextInput;
    std::shared_ptr<ui::Label> propTypeLabel;
    
    // Project management
    std::string currentProjectPath;
    std::string binaryDirectory; // Directory where the binary is located
    bool hasUnsavedChanges = false;
    
    // Undo/Redo system
    std::vector<UndoAction> undoStack;
    std::vector<UndoAction> redoStack;
    static const size_t MAX_UNDO_STACK = 50;

public:
    LayoutEditor(UI* uiInstance) : ui(uiInstance) {
        setupUI();
    }
    
    // Set the binary directory for file operations
    void setBinaryDirectory(const std::string& dir) {
        binaryDirectory = dir;
    }
    
    // Public method for loading wireframe files
    void loadProjectFromFile(const std::string& path) {
        loadProjectFromFileInternal(path);
    }

    void setupUI() {
        // Enable 10px grid for precise positioning
        ui->setGridSize(10);
        
        // Create context menu at top
        setupContextMenu();
        
        // Create separator lines for visual separation (moved down for menu)
        setupSeparatorLines();
        
        // Create palette area (left side, moved down for menu)
        auto paletteTitle = ui->createLabel("Element Palette", 10, 80);
        
        // Palette buttons - vertical list (moved down for menu)
        auto buttonPalette = ui->createButton("Button", 10, 110, [this](){
            addElementToCanvas("button", generateElementName("Button"));
        });
        
        auto labelPalette = ui->createButton("Label", 10, 150, [this](){
            addElementToCanvas("label", generateElementName("Label"));
        });
        
        auto textboxPalette = ui->createButton("TextBox", 10, 190, [this](){
            addElementToCanvas("textbox", generateElementName("TextBox"));
        });
        
        auto canvasPalette = ui->createButton("Canvas", 10, 230, [this](){
            addElementToCanvas("canvas", generateElementName("Canvas"));
        });
        
        auto checkboxPalette = ui->createButton("CheckBox", 10, 270, [this](){
            addElementToCanvas("checkbox", generateElementName("CheckBox"));
        });
        
        auto hsliderPalette = ui->createButton("HSlider", 10, 310, [this](){
            addElementToCanvas("hslider", generateElementName("HSlider"));
        });
        
        auto vsliderPalette = ui->createButton("VSlider", 10, 350, [this](){
            addElementToCanvas("vslider", generateElementName("VSlider"));
        });
        
        auto progressPalette = ui->createButton("Progress", 10, 390, [this](){
            addElementToCanvas("progress", generateElementName("ProgressBar"));
        });
        
        auto imagePalette = ui->createButton("Image", 10, 430, [this](){
            addElementToCanvas("image", generateElementName("Image"));
        });
        
        auto cyclelistPalette = ui->createButton("CycleList", 10, 470, [this](){
            addElementToCanvas("cyclelist", generateElementName("CycleList"));
        });
        
        auto optionselectPalette = ui->createButton("OptionSelect", 10, 510, [this](){
            addElementToCanvas("optionselect", generateElementName("OptionSelect"));
        });
        
        // Canvas area separator (moved down for menu)
        auto separator = ui->createLabel("Canvas (1024x600)", 200, 40);
        
        // Create canvas background (moved down for menu)
        auto canvas = ui->createCanvas(220, 60, 1024, 600);
        
        // Dark background
        canvas->filledRect({0, 0, 1024, 600}, ui::Color(30, 30, 30, 255));
        
        // Draw grid dots every 10px to match snap grid
        for (int x = 0; x <= 1024; x += 10) {
            for (int y = 0; y <= 600; y += 10) {
                canvas->point(x, y, ui::Color(200, 200, 200, 255));
            }
        }
        
        // Properties panel (right side)
        setupPropertiesPanel();
        
        // Export/Save buttons - bottom left
        auto exportBtn = ui->createButton("Export JSON", 10, 740, [this](){
            exportToJSON();
        });
        
        // Element manipulation controls - bottom center (fixed spacing)
        auto moveUpBtn = ui->createButton("Move Up", 400, 740, [this](){
            moveSelectedElement(0, -10);
        });
        
        auto moveDownBtn = ui->createButton("Move Down", 510, 740, [this](){
            moveSelectedElement(0, 10);
        });
        
        auto moveLeftBtn = ui->createButton("Move Left", 620, 740, [this](){
            moveSelectedElement(-10, 0);
        });
        
        auto moveRightBtn = ui->createButton("Move Right", 730, 740, [this](){
            moveSelectedElement(10, 0);
        });
        
        auto deleteBtn = ui->createButton("Delete", 840, 740, [this](){
            deleteSelectedElement();
        });
        
        // Resize controls - bottom right (fixed spacing)
        auto widerBtn = ui->createButton("Wider", 960, 740, [this](){
            resizeSelectedElement(10, 0);
        });
        
        auto narrowerBtn = ui->createButton("Narrower", 1070, 740, [this](){
            resizeSelectedElement(-10, 0);
        });
        
        auto tallerBtn = ui->createButton("Taller", 1180, 740, [this](){
            resizeSelectedElement(0, 10);
        });
        
        auto shorterBtn = ui->createButton("Shorter", 1290, 740, [this](){
            resizeSelectedElement(0, -10);
        });
        
        std::cout << "Layout Editor Started!" << std::endl;
        std::cout << "- Canvas: 1024x600 with 10px grid positioned under menu bar" << std::endl;
        std::cout << "- Element names (Button1, Label1, etc.) displayed above each element" << std::endl;
        std::cout << "- Use File menu for New, Open, Save, Save As operations" << std::endl;
        std::cout << "- Use Edit menu for Undo, Redo, Delete operations" << std::endl;
        std::cout << "- Click palette buttons to add elements" << std::endl;
        std::cout << "- Click buttons/checkboxes directly to select them" << std::endl;
        std::cout << "- Use properties panel to edit selected element" << std::endl;
        std::cout << "- Movement: WASD keys or Numpad (5=up, 2=down, 1=left, 3=right)" << std::endl;
        std::cout << "- Resizing: Shift+WASD keys or Shift+Numpad keys" << std::endl;
        std::cout << "- Keyboard shortcuts: Ctrl+Z (Undo), Ctrl+Shift+Z/Ctrl+Y (Redo)" << std::endl;
        std::cout << "- Use Export menu for JSON wireframe data" << std::endl;
        
        // Setup keyboard shortcuts
        setupKeyboardShortcuts();
    }
    
    // Helper method to check if any text input has focus
    bool isTextInputFocused() {
        auto focusedId = ui->getFocusedElementId();
        return (propNameInput && propNameInput->getId() == focusedId) ||
               (propXInput && propXInput->getId() == focusedId) ||
               (propYInput && propYInput->getId() == focusedId) ||
               (propWidthInput && propWidthInput->getId() == focusedId) ||
               (propHeightInput && propHeightInput->getId() == focusedId) ||
               (propTextInput && propTextInput->getId() == focusedId);
    }

    void setupKeyboardShortcuts() {
        // Numpad keys for movement (easier with shift)
        ui->assignHotKey("", "5", [this](){
            if (isTextInputFocused()) return; // Don't move elements if text input is focused
            if (SDL_GetModState() & KMOD_SHIFT) {
                resizeSelectedElement(0, -10); // Shift+5: Make shorter
            } else {
                moveSelectedElement(0, -10); // 5: Move up
            }
        });
        
        ui->assignHotKey("", "2", [this](){
            if (isTextInputFocused()) return; // Don't move elements if text input is focused
            if (SDL_GetModState() & KMOD_SHIFT) {
                resizeSelectedElement(0, 10); // Shift+2: Make taller
            } else {
                moveSelectedElement(0, 10); // 2: Move down
            }
        });
        
        ui->assignHotKey("", "1", [this](){
            if (isTextInputFocused()) return; // Don't move elements if text input is focused
            if (SDL_GetModState() & KMOD_SHIFT) {
                resizeSelectedElement(-10, 0); // Shift+1: Make narrower
            } else {
                moveSelectedElement(-10, 0); // 1: Move left
            }
        });
        
        ui->assignHotKey("", "3", [this](){
            if (isTextInputFocused()) return; // Don't move elements if text input is focused
            if (SDL_GetModState() & KMOD_SHIFT) {
                resizeSelectedElement(10, 0); // Shift+3: Make wider
            } else {
                moveSelectedElement(10, 0); // 3: Move right
            }
        });
        
        // Undo/Redo shortcuts
        ui->assignHotKey("", "z", [this](){
            if (SDL_GetModState() & KMOD_CTRL) {
                if (SDL_GetModState() & KMOD_SHIFT) {
                    redo(); // Ctrl+Shift+Z: Redo
                } else {
                    undo(); // Ctrl+Z: Undo
                }
            }
        });
        
        ui->assignHotKey("", "y", [this](){
            if (SDL_GetModState() & KMOD_CTRL) {
                redo(); // Ctrl+Y: Redo
            }
        });
        
        // WASD keys for movement and resizing
        ui->assignHotKey("", "w", [this](){
            if (isTextInputFocused()) return; // Don't move elements if text input is focused
            if (SDL_GetModState() & KMOD_SHIFT) {
                resizeSelectedElement(0, -10); // Shift+W: Make shorter
            } else {
                moveSelectedElement(0, -10); // W: Move up
            }
        });
        
        ui->assignHotKey("", "s", [this](){
            if (isTextInputFocused()) return; // Don't move elements if text input is focused
            if (SDL_GetModState() & KMOD_SHIFT) {
                resizeSelectedElement(0, 10); // Shift+S: Make taller
            } else {
                moveSelectedElement(0, 10); // S: Move down
            }
        });
        
        ui->assignHotKey("", "a", [this](){
            if (isTextInputFocused()) return; // Don't move elements if text input is focused
            if (SDL_GetModState() & KMOD_SHIFT) {
                resizeSelectedElement(-10, 0); // Shift+A: Make narrower
            } else {
                moveSelectedElement(-10, 0); // A: Move left
            }
        });
        
        ui->assignHotKey("", "d", [this](){
            if (isTextInputFocused()) return; // Don't move elements if text input is focused
            if (SDL_GetModState() & KMOD_SHIFT) {
                resizeSelectedElement(10, 0); // Shift+D: Make wider
            } else {
                moveSelectedElement(10, 0); // D: Move right
            }
        });
    }
    
    void setupSeparatorLines() {
        // Vertical line between palette and canvas (moved down for menu)
        auto paletteCanvasSeparator = ui->createCanvas(215, 30, 2, 770);
        paletteCanvasSeparator->filledRect({0, 0, 2, 770}, ui::Color(180, 180, 180, 255)); // Gray line
        
        // Vertical line between canvas and properties panel (moved down for menu)
        auto canvasPropertiesSeparator = ui->createCanvas(1250, 30, 2, 770);
        canvasPropertiesSeparator->filledRect({0, 0, 2, 770}, ui::Color(180, 180, 180, 255)); // Gray line
        
        // Horizontal line above bottom toolbar (aligned with vertical separators)
        auto toolbarSeparator = ui->createCanvas(215, 730, 1037, 2);
        toolbarSeparator->filledRect({0, 0, 1037, 2}, ui::Color(180, 180, 180, 255)); // Gray line
        
        // Horizontal line below context menu
        auto menuSeparator = ui->createCanvas(0, 30, 1550, 2);
        menuSeparator->filledRect({0, 0, 1550, 2}, ui::Color(180, 180, 180, 255)); // Gray line
    }
    
    std::string generateElementName(const std::string& type) {
        elementCounters[type]++;
        return type + std::to_string(elementCounters[type]);
    }
    
    void setupPropertiesPanel() {
        int panelX = 1270; // Right side of window
        
        // Properties panel title (moved down for menu)
        auto propTitle = ui->createLabel("Element Properties", panelX, 40);
        
        // Element Info section
        auto infoLabel = ui->createLabel("Element Info:", panelX, 50);
        auto typeLabel = ui->createLabel("Type:", panelX, 80);
        propTypeLabel = std::static_pointer_cast<ui::Label>(ui->createLabel("(none)", panelX + 50, 80));
        
        auto nameLabel = ui->createLabel("Name:", panelX, 110);
        propNameInput = std::static_pointer_cast<ui::TextBox>(ui->createTextBox("", 10, 110));
        propNameInput->setPosition(panelX + 50, 110);
        propNameInput->setSize(180, 25);
        
        // Position & Size section
        auto posLabel = ui->createLabel("Position & Size:", panelX, 160);
        
        auto xLabel = ui->createLabel("X:", panelX, 190);
        propXInput = std::static_pointer_cast<ui::TextBox>(ui->createTextBox("", 10, 190));
        propXInput->setPosition(panelX + 30, 190);
        propXInput->setSize(70, 25);
        
        auto yLabel = ui->createLabel("Y:", panelX + 110, 190);
        propYInput = std::static_pointer_cast<ui::TextBox>(ui->createTextBox("", 10, 190));
        propYInput->setPosition(panelX + 130, 190);
        propYInput->setSize(70, 25);
        
        auto wLabel = ui->createLabel("W:", panelX, 220);
        propWidthInput = std::static_pointer_cast<ui::TextBox>(ui->createTextBox("", 10, 220));
        propWidthInput->setPosition(panelX + 30, 220);
        propWidthInput->setSize(70, 25);
        
        auto hLabel = ui->createLabel("H:", panelX + 110, 220);
        propHeightInput = std::static_pointer_cast<ui::TextBox>(ui->createTextBox("", 10, 220));
        propHeightInput->setPosition(panelX + 130, 220);
        propHeightInput->setSize(70, 25);
        
        // Content section
        auto contentLabel = ui->createLabel("Content:", panelX, 270);
        auto textLabel = ui->createLabel("Text:", panelX, 300);
        propTextInput = std::static_pointer_cast<ui::TextBox>(ui->createTextBox("", 10, 300));
        propTextInput->setPosition(panelX + 50, 300);
        propTextInput->setSize(180, 25);
        
        // Apply button
        auto applyBtn = ui->createButton("Apply Changes", panelX, 340, [this](){
            applyPropertyChanges();
        });
        applyBtn->setSize(120, 30);
        
        // Clone button
        auto cloneBtn = ui->createButton("Clone", panelX + 130, 340, [this](){
            cloneSelectedElement();
        });
        cloneBtn->setSize(80, 30);
        
        // Clear properties initially
        clearPropertiesPanel();
    }
    
    void clearPropertiesPanel() {
        if (propTypeLabel) propTypeLabel->setText("(none)");
        if (propNameInput) propNameInput->setText("");
        if (propXInput) propXInput->setText("");
        if (propYInput) propYInput->setText("");
        if (propWidthInput) propWidthInput->setText("");
        if (propHeightInput) propHeightInput->setText("");
        if (propTextInput) propTextInput->setText("");
    }
    
    void updatePropertiesPanel() {
        if (!selectedElement || !selectedWireframeElement) {
            clearPropertiesPanel();
            return;
        }
        
        // Update properties panel with current values
        propTypeLabel->setText(selectedWireframeElement->type);
        propNameInput->setText(selectedWireframeElement->id);
        propXInput->setText(std::to_string(selectedElement->getX()));
        propYInput->setText(std::to_string(selectedElement->getY()));
        propWidthInput->setText(std::to_string(selectedElement->getWidth()));
        propHeightInput->setText(std::to_string(selectedElement->getHeight()));
        propTextInput->setText(selectedWireframeElement->text);
    }
    
    void applyPropertyChanges() {
        if (!selectedElement) {
            std::cout << "No element selected." << std::endl;
            return;
        }
        
        try {
            // Get values from text inputs
            std::string newName = propNameInput->getText();
            int newX = std::stoi(propXInput->getText());
            int newY = std::stoi(propYInput->getText());
            int newWidth = std::stoi(propWidthInput->getText());
            int newHeight = std::stoi(propHeightInput->getText());
            std::string newText = propTextInput->getText();
            
            // Validate and snap to grid
            newX = ((newX + 5) / 10) * 10;
            newY = ((newY + 5) / 10) * 10;
            newWidth = ((newWidth + 5) / 10) * 10;
            newHeight = ((newHeight + 5) / 10) * 10;
            
            // Validate bounds (relative to canvas area)
            newX = std::max(220, std::min(1220 - newWidth, newX));
            newY = std::max(60, std::min(660 - newHeight, newY));
            newWidth = std::max(20, newWidth);
            newHeight = std::max(20, newHeight);
            
            // Apply changes to UI element
            selectedElement->setPosition(newX, newY);
            selectedElement->setSize(newWidth, newHeight);
            
            // Find wireframe element by matching position (since we need to map UI element to wireframe element)
            for (auto& elem : wireframeElements) {
                if (elem.x + 220 == selectedElement->getX() && elem.y + 60 == selectedElement->getY()) {
                    // Store old values for undo
                    WireframeElement oldElement = elem;
                    
                    // Update wireframe data (store relative to canvas)
                    elem.id = newName;
                    elem.x = newX - 220;
                    elem.y = newY - 60;
                    elem.width = newWidth;
                    elem.height = newHeight;
                    elem.text = newText;
                    
                    // Add to undo stack
                    UndoAction undoAction;
                    undoAction.type = UndoAction::MODIFY_ELEMENT;
                    undoAction.element = elem;
                    undoAction.oldElement = oldElement;
                    pushUndoAction(undoAction);
                    
                    break;
                }
            }
            
            // Update visual elements
            updateSelectionButtonPosition(selectedElement);
            addResizeHandles(selectedElement);
            
            // Update name label with new name
            updateElementNameLabel(selectedElement, newName);
            
            std::cout << "Applied property changes to " << selectedElement->getId() << std::endl;
            
        } catch (const std::exception& e) {
            std::cout << "Invalid property values. Please enter valid numbers." << std::endl;
        }
    }
    
    void cloneSelectedElement() {
        if (!selectedElement || !selectedWireframeElement) {
            std::cout << "No element selected to clone." << std::endl;
            return;
        }
        
        // Create a copy of the wireframe element with offset
        WireframeElement clonedElement = *selectedWireframeElement;
        
        // Generate new name
        clonedElement.id = generateElementName(clonedElement.type);
        
        // Offset position by 20px (2 grid units) so user can select either element
        clonedElement.x += 20;
        clonedElement.y += 20;
        
        // Ensure clone stays within canvas bounds
        if (clonedElement.x + clonedElement.width > 1024) {
            clonedElement.x = 1024 - clonedElement.width;
        }
        if (clonedElement.y + clonedElement.height > 600) {
            clonedElement.y = 600 - clonedElement.height;
        }
        
        // Add to wireframe elements
        wireframeElements.push_back(clonedElement);
        
        // Create UI element at absolute position (canvas offset + relative position)
        int absoluteX = 220 + clonedElement.x;
        int absoluteY = 60 + clonedElement.y;
        
        std::shared_ptr<ui::UIElement> element;
        
        if (clonedElement.type == "button") {
            element = ui->createButton(clonedElement.text, absoluteX, absoluteY, [](){});
            auto buttonElement = std::static_pointer_cast<ui::Button>(element);
            buttonElement->setCallback([this, element](){
                selectElement(element);
            });
        } else if (clonedElement.type == "label") {
            element = ui->createLabel(clonedElement.text, absoluteX, absoluteY);
        } else if (clonedElement.type == "textbox") {
            element = ui->createTextBox(clonedElement.text, absoluteX, absoluteY, clonedElement.width, clonedElement.height);
        } else if (clonedElement.type == "canvas") {
            element = ui->createCanvas(absoluteX, absoluteY, clonedElement.width, clonedElement.height);
        } else if (clonedElement.type == "checkbox") {
            element = ui->createCheckBox(false, absoluteX, absoluteY, [](bool){});
            auto checkboxElement = std::static_pointer_cast<ui::CheckBox>(element);
            checkboxElement->setCallback([this, element](bool checked){
                selectElement(element);
            });
        } else if (clonedElement.type == "hslider") {
            element = ui->createHSlider(absoluteX, absoluteY, clonedElement.width, clonedElement.height, 0.0f, 1.0f, 0.5f);
        } else if (clonedElement.type == "vslider") {
            element = ui->createVSlider(absoluteX, absoluteY, clonedElement.width, clonedElement.height, 0.0f, 1.0f, 0.5f);
        } else if (clonedElement.type == "progressbar") {
            element = ui->createProgressBar(absoluteX, absoluteY, clonedElement.width, (int)(0.5f * 100));
        } else if (clonedElement.type == "image") {
            element = ui->createCanvas(absoluteX, absoluteY, clonedElement.width, clonedElement.height);
        } else if (clonedElement.type == "cyclelist") {
            element = ui->createCycleList(absoluteX, absoluteY, clonedElement.width, clonedElement.height, {"Option1", "Option2", "Option3"});
        } else if (clonedElement.type == "optionselect") {
            element = ui->createOptionSelect({"Option1", "Option2", "Option3"}, 0, absoluteX, absoluteY, [](int){});
        }
        
        if (element) {
            element->setSize(clonedElement.width, clonedElement.height);
            canvasElements.push_back(element);
            
            // Create selection button for non-clickable elements
            std::shared_ptr<ui::UIElement> selectBtn = nullptr;
            if (clonedElement.type != "button" && clonedElement.type != "checkbox") {
                int centerX = absoluteX + clonedElement.width / 2 - 10;
                int centerY = absoluteY + clonedElement.height / 2 - 10;
                selectBtn = ui->createButton("S", centerX, centerY, [this, element](){
                    selectElement(element);
                });
                selectBtn->setSize(20, 20);
            }
            selectionButtons.push_back(selectBtn);
            
            // Add element name label
            addElementNameLabel(element, clonedElement.id);
            
            // Add to undo stack
            UndoAction undoAction;
            undoAction.type = UndoAction::ADD_ELEMENT;
            undoAction.element = clonedElement;
            undoAction.index = wireframeElements.size() - 1;
            pushUndoAction(undoAction);
            
            // Select the cloned element
            selectElement(element);
            
            std::cout << "Cloned element: " << clonedElement.id << " at (" << clonedElement.x << ", " << clonedElement.y << ")" << std::endl;
        }
    }
    
    void addElementToCanvas(const std::string& type, const std::string& text) {
        // Add element to canvas area (starting at x=220 to avoid palette, y=60 for menu)
        int startX = 220 + (wireframeElements.size() * 20) % 400;
        int startY = 80 + (wireframeElements.size() * 30) % 300;
        
        std::shared_ptr<ui::UIElement> element;
        
        if (type == "button") {
            element = ui->createButton(text, startX, startY, [](){});
        } else if (type == "label") {
            element = ui->createLabel(text, startX, startY);
        } else if (type == "textbox") {
            element = ui->createTextBox(text, startX, startY);
        } else if (type == "canvas") {
            element = ui->createCanvas(startX, startY, 100, 80);
        } else if (type == "checkbox") {
            element = ui->createCheckBox(false, startX, startY, [](bool){});
        } else if (type == "hslider") {
            element = ui->createHSlider(startX, startY, 120, 20, 0.0f, 100.0f, 50.0f);
        } else if (type == "vslider") {
            element = ui->createVSlider(startX, startY, 20, 120, 0.0f, 100.0f, 50.0f);
        } else if (type == "progress") {
            element = ui->createProgressBar(startX, startY, 120, 20, 0.5f, true);
        } else if (type == "image") {
            // Create a placeholder image (empty for now)
            element = ui->createCanvas(startX, startY, 80, 60);
        } else if (type == "cyclelist") {
            std::vector<std::string> items = {"Item 1", "Item 2", "Item 3"};
            element = ui->createCycleList(startX, startY, 100, 30, items);
        } else if (type == "optionselect") {
            std::vector<std::string> options = {"Option 1", "Option 2", "Option 3"};
            element = ui->createOptionSelect(options, 0, startX, startY, [](int){});
        }
        
        if (element) {
            // Store element reference for interaction
            canvasElements.push_back(element);
            
            // Create selection button in center of element for non-interactive elements
            std::shared_ptr<ui::UIElement> selectBtn = nullptr;
            
            if (type == "button") {
                // Buttons are directly clickable
                auto buttonElement = std::static_pointer_cast<ui::Button>(element);
                buttonElement->setCallback([this, element](){
                    selectElement(element);
                });
            } else if (type == "checkbox") {
                // Checkboxes are directly clickable
                auto checkboxElement = std::static_pointer_cast<ui::CheckBox>(element);
                checkboxElement->setCallback([this, element](bool){
                    selectElement(element);
                });
            } else {
                // Other elements need selection buttons in their center
                int centerX = startX + element->getWidth() / 2 - 10;
                int centerY = startY + element->getHeight() / 2 - 10;
                selectBtn = ui->createButton("S", centerX, centerY, [this, element](){
                    selectElement(element);
                });
                selectBtn->setSize(20, 20);
            }
            
            // Track selection button (nullptr for directly clickable elements)
            selectionButtons.push_back(selectBtn);
            
            // Store wireframe data
            WireframeElement wfElement;
            wfElement.type = type;
            wfElement.id = text; // Use the meaningful name (Button1, Label1, etc.)
            wfElement.x = element->getX() - 220; // Store relative to canvas
            wfElement.y = element->getY() - 60;  // Store relative to canvas
            wfElement.width = element->getWidth();
            wfElement.height = element->getHeight();
            wfElement.text = text;
            
            wireframeElements.push_back(wfElement);
            
            // Add to undo stack
            UndoAction undoAction;
            undoAction.type = UndoAction::ADD_ELEMENT;
            undoAction.element = wfElement;
            pushUndoAction(undoAction);
            
            // Create name label for the element
            addElementNameLabel(element, text);
            
            // Auto-select the newly added element
            selectElement(element);
            
            std::cout << "Added " << type << " at (" << startX << "," << startY << ")" << std::endl;
        }
    }
    
    void exportToJSON() {
        std::string filePath = binaryDirectory + "wireframe.json";
        std::ofstream file(filePath);
        file << "{\n";
        file << "  \"canvas\": {\"width\": 1024, \"height\": 600},\n";
        file << "  \"elements\": [\n";
        
        for (size_t i = 0; i < wireframeElements.size(); ++i) {
            const auto& elem = wireframeElements[i];
            file << "    {\n";
            file << "      \"type\": \"" << elem.type << "\",\n";
            file << "      \"id\": \"" << elem.id << "\",\n";
            file << "      \"x\": " << elem.x << ",\n";
            file << "      \"y\": " << elem.y << ",\n";
            file << "      \"width\": " << elem.width << ",\n";
            file << "      \"height\": " << elem.height << ",\n";
            file << "      \"text\": \"" << elem.text << "\"\n";
            file << "    }";
            if (i < wireframeElements.size() - 1) file << ",";
            file << "\n";
        }
        
        file << "  ]\n";
        file << "}\n";
        file.close();
        
        std::cout << "Exported wireframe to " << filePath << " (" << wireframeElements.size() << " elements)" << std::endl;
    }
    
    void loadProject() {
        std::cout << "Load functionality not implemented yet" << std::endl;
    }
    
    void selectElement(std::shared_ptr<ui::UIElement> element) {
        selectedElement = element;
        
        // Find corresponding wireframe element
        selectedWireframeElement = nullptr;
        for (auto& elem : wireframeElements) {
            if (elem.x + 220 == element->getX() && elem.y + 60 == element->getY()) {
                selectedWireframeElement = &elem;
                break;
            }
        }
        
        std::cout << "Selected element: " << element->getId() << " at (" 
                  << element->getX() << "," << element->getY() << ") size (" 
                  << element->getWidth() << "x" << element->getHeight() << ")" << std::endl;
        
        // Remove old selection indicator (no more canvas overlay)
        if (selectedIndicator) {
            ui->removeElement(selectedIndicator->getId());
            selectedIndicator = nullptr;
        }
        
        // Add resize handles for visual feedback
        addResizeHandles(element);
        
        // Update properties panel with selected element data
        updatePropertiesPanel();
    }
    
    void moveSelectedElement(int deltaX, int deltaY) {
        if (!selectedElement) {
            std::cout << "No element selected. Click an element first." << std::endl;
            return;
        }
        
        int newX = selectedElement->getX() + deltaX;
        int newY = selectedElement->getY() + deltaY;
        
        // Snap to grid (10px)
        newX = ((newX + 5) / 10) * 10;
        newY = ((newY + 5) / 10) * 10;
        
        // Keep element within canvas bounds (220-1220, 30-630)
        newX = std::max(220, std::min(1220 - selectedElement->getWidth(), newX));
        newY = std::max(30, std::min(630 - selectedElement->getHeight(), newY));
        
        selectedElement->setPosition(newX, newY);
        
        // Update wireframe element directly
        if (selectedWireframeElement) {
            selectedWireframeElement->x = newX - 220;
            selectedWireframeElement->y = newY - 60;
        }
        
        // Update resize handles
        addResizeHandles(selectedElement);
        
        // Update selection button position if it exists
        updateSelectionButtonPosition(selectedElement);
        
        // Update properties panel
        updatePropertiesPanel();
        
        std::cout << "Moved element to (" << newX << "," << newY << ")" << std::endl;
    }
    
    void updateWireframeElementByPosition(std::shared_ptr<ui::UIElement> element, int newX, int newY) {
        for (auto& wfElement : wireframeElements) {
            // Find by matching old position (before the move)
            int oldX = newX - 10; // Reverse the movement to find old position
            int oldY = newY - 10;
            if (wfElement.x + 220 == oldX && wfElement.y + 60 == oldY) {
                wfElement.x = newX - 220; // Store relative to canvas
                wfElement.y = newY - 60;  // Store relative to canvas
                return;
            }
        }
        
        // Fallback: find by current element position
        for (auto& wfElement : wireframeElements) {
            if (wfElement.x + 220 == element->getX() && wfElement.y + 60 == element->getY()) {
                wfElement.x = newX - 220;
                wfElement.y = newY - 60;
                return;
            }
        }
    }
    
    void deleteSelectedElement() {
        if (!selectedElement) {
            std::cout << "No element selected. Click an element first." << std::endl;
            return;
        }
        
        std::string elementId = selectedElement->getId();
        
        // Find wireframe element for undo
        WireframeElement deletedElement;
        auto wfIt = std::find_if(wireframeElements.begin(), wireframeElements.end(),
            [&elementId](const WireframeElement& elem) { return elem.id == elementId; });
        
        if (wfIt != wireframeElements.end()) {
            deletedElement = *wfIt;
        }
        
        // Remove selection indicator
        if (selectedIndicator) {
            ui->removeElement(selectedIndicator->getId());
            selectedIndicator = nullptr;
        }
        
        // Remove resize handles
        clearResizeHandles();
        
        // Remove from UI
        ui->removeElement(elementId);
        
        // Remove from canvas elements and corresponding selection button
        auto canvasIt = std::find(canvasElements.begin(), canvasElements.end(), selectedElement);
        if (canvasIt != canvasElements.end()) {
            size_t elementIndex = std::distance(canvasElements.begin(), canvasIt);
            canvasElements.erase(canvasIt);
            
            // Remove corresponding selection button if it exists
            if (elementIndex < selectionButtons.size() && selectionButtons[elementIndex]) {
                ui->removeElement(selectionButtons[elementIndex]->getId());
            }
            selectionButtons.erase(selectionButtons.begin() + elementIndex);
            
            // Remove corresponding name label if it exists
            if (elementIndex < elementNameLabels.size() && elementNameLabels[elementIndex]) {
                ui->removeElement(elementNameLabels[elementIndex]->getId());
            }
            elementNameLabels.erase(elementNameLabels.begin() + elementIndex);
        }
        
        // Remove from wireframe data
        wireframeElements.erase(
            std::remove_if(wireframeElements.begin(), wireframeElements.end(),
                [&elementId](const WireframeElement& elem) {
                    return elem.id == elementId;
                }),
            wireframeElements.end()
        );
        
        // Add to undo stack
        UndoAction undoAction;
        undoAction.type = UndoAction::REMOVE_ELEMENT;
        undoAction.element = deletedElement;
        pushUndoAction(undoAction);
        
        std::cout << "Deleted element: " << elementId << std::endl;
        selectedElement = nullptr;
        selectedWireframeElement = nullptr;
    }
    
    void addResizeHandles(std::shared_ptr<ui::UIElement> element) {
        // Remove existing handles
        clearResizeHandles();
        
        int x = element->getX();
        int y = element->getY();
        int w = element->getWidth();
        int h = element->getHeight();
        
        // Create clickable resize handles at corners
        auto bottomRight = ui->createButton("+", x + w - 10, y + h - 10, [this](){
            resizeSelectedElement(10, 10);
        });
        bottomRight->setSize(15, 15);
        resizeHandles.push_back(bottomRight);
        
        auto topRight = ui->createButton("+", x + w - 10, y - 5, [this](){
            resizeSelectedElement(10, -10);
        });
        topRight->setSize(15, 15);
        resizeHandles.push_back(topRight);
        
        auto bottomLeft = ui->createButton("+", x - 5, y + h - 10, [this](){
            resizeSelectedElement(-10, 10);
        });
        bottomLeft->setSize(15, 15);
        resizeHandles.push_back(bottomLeft);
        
        auto topLeft = ui->createButton("+", x - 5, y - 5, [this](){
            resizeSelectedElement(-10, -10);
        });
        topLeft->setSize(15, 15);
        resizeHandles.push_back(topLeft);
    }
    
    void clearResizeHandles() {
        for (auto& handle : resizeHandles) {
            if (handle) {
                ui->removeElement(handle->getId());
            }
        }
        resizeHandles.clear();
    }
    
    void resizeSelectedElement(int deltaW, int deltaH) {
        if (!selectedElement) {
            std::cout << "No element selected. Click an element first." << std::endl;
            return;
        }
        
        int newW = selectedElement->getWidth() + deltaW;
        int newH = selectedElement->getHeight() + deltaH;
        
        // Minimum size constraints
        newW = std::max(20, newW);
        newH = std::max(20, newH);
        
        // Snap to grid (10px)
        newW = ((newW + 5) / 10) * 10;
        newH = ((newH + 5) / 10) * 10;
        
        selectedElement->setSize(newW, newH);
        
        // Update wireframe element directly
        if (selectedWireframeElement) {
            selectedWireframeElement->width = newW;
            selectedWireframeElement->height = newH;
        }
        
        // Update visual handles
        addResizeHandles(selectedElement);
        
        // Update selection button position
        updateSelectionButtonPosition(selectedElement);
        
        // Update name label position
        updateElementNameLabel(selectedElement, selectedWireframeElement ? selectedWireframeElement->id : "");
        
        // Update properties panel
        updatePropertiesPanel();
        
        std::cout << "Resized element to " << newW << "x" << newH << std::endl;
    }
    
    void updateWireframeElementSizeByPosition(std::shared_ptr<ui::UIElement> element, int newW, int newH) {
        for (auto& wfElement : wireframeElements) {
            // Find by matching position
            if (wfElement.x + 220 == element->getX() && wfElement.y + 60 == element->getY()) {
                wfElement.width = newW;
                wfElement.height = newH;
                break;
            }
        }
    }
    
    void updateSelectionButtonPosition(std::shared_ptr<ui::UIElement> element) {
        // Find the element's index and update its selection button position
        auto it = std::find(canvasElements.begin(), canvasElements.end(), element);
        if (it != canvasElements.end()) {
            size_t index = std::distance(canvasElements.begin(), it);
            if (index < selectionButtons.size() && selectionButtons[index]) {
                int centerX = element->getX() + element->getWidth() / 2 - 10;
                int centerY = element->getY() + element->getHeight() / 2 - 10;
                selectionButtons[index]->setPosition(centerX, centerY);
            }
        }
    }
    
    void addElementNameLabel(std::shared_ptr<ui::UIElement> element, const std::string& name) {
        // Create small label showing element name at top-left of element
        int labelX = element->getX() - 2;
        int labelY = element->getY() - 15;
        auto nameLabel = ui->createLabel(name, labelX, labelY);
        nameLabel->setSize(name.length() * 8, 12); // Approximate sizing
        elementNameLabels.push_back(nameLabel);
    }
    
    void updateElementNameLabel(std::shared_ptr<ui::UIElement> element, const std::string& newName) {
        // Find the element's index and update its name label
        auto it = std::find(canvasElements.begin(), canvasElements.end(), element);
        if (it != canvasElements.end()) {
            size_t index = std::distance(canvasElements.begin(), it);
            if (index < elementNameLabels.size() && elementNameLabels[index]) {
                // Cast to Label to access setText
                auto label = std::static_pointer_cast<ui::Label>(elementNameLabels[index]);
                label->setText(newName);
                // Update position
                int labelX = element->getX() - 2;
                int labelY = element->getY() - 15;
                elementNameLabels[index]->setPosition(labelX, labelY);
            }
        }
    }
    
    void setupContextMenu() {
        std::vector<ui::TopMenuItem> menuItems;
        
        // File menu
        ui::TopMenuItem fileMenu;
        fileMenu.label = "File";
        fileMenu.subItems = {
            {"New", [this](){ newProject(); }},
            {"Open...", [this](){ openProject(); }},
            {"Save", [this](){ saveProject(); }},
            {"Save As...", [this](){ saveAsProject(); }}
        };
        menuItems.push_back(fileMenu);
        
        // Edit menu
        ui::TopMenuItem editMenu;
        editMenu.label = "Edit";
        editMenu.subItems = {
            {"Undo", [this](){ undo(); }},
            {"Redo", [this](){ redo(); }},
            {"Delete", [this](){ deleteSelectedElement(); }}
        };
        menuItems.push_back(editMenu);
        
        // Export menu
        ui::TopMenuItem exportMenu;
        exportMenu.label = "Export";
        exportMenu.subItems = {
            {"Export JSON", [this](){ exportToJSON(); }}
        };
        menuItems.push_back(exportMenu);
        
        auto contextMenu = ui->createContextMenu(menuItems);
    }
    
    void newProject() {
        clearAll();
        currentProjectPath = "";
        hasUnsavedChanges = false;
        std::cout << "New project created" << std::endl;
    }
    
    void openProject() {
        std::cout << "Enter project file path: ";
        std::string path;
        std::getline(std::cin, path);
        if (!path.empty()) {
            loadProjectFromFileInternal(path);
        }
    }
    
    void saveProject() {
        if (currentProjectPath.empty()) {
            saveAsProject();
        } else {
            saveProjectToFile(currentProjectPath);
        }
    }
    
    void saveAsProject() {
        std::cout << "Enter project file path (.layout): ";
        std::string path;
        std::getline(std::cin, path);
        if (!path.empty()) {
            if (path.find(".layout") == std::string::npos) {
                path += ".layout";
            }
            saveProjectToFile(path);
            currentProjectPath = path;
        }
    }
    
    void saveProjectToFile(const std::string& path) {
        std::ofstream file(path);
        if (!file.is_open()) {
            std::cerr << "Failed to save project to " << path << std::endl;
            return;
        }
        
        file << "{\n";
        file << "  \"version\": \"1.0\",\n";
        file << "  \"canvas\": {\"width\": 1024, \"height\": 600},\n";
        file << "  \"elements\": [\n";
        
        for (size_t i = 0; i < wireframeElements.size(); ++i) {
            const auto& elem = wireframeElements[i];
            file << "    {\n";
            file << "      \"type\": \"" << elem.type << "\",\n";
            file << "      \"id\": \"" << elem.id << "\",\n";
            file << "      \"x\": " << elem.x << ",\n";
            file << "      \"y\": " << elem.y << ",\n";
            file << "      \"width\": " << elem.width << ",\n";
            file << "      \"height\": " << elem.height << ",\n";
            file << "      \"text\": \"" << elem.text << "\"\n";
            file << "    }";
            if (i < wireframeElements.size() - 1) file << ",";
            file << "\n";
        }
        
        file << "  ]\n";
        file << "}\n";
        
        file.close();
        hasUnsavedChanges = false;
        std::cout << "Project saved to " << path << std::endl;
    }
    
    void loadProjectFromFileInternal(const std::string& path) {
        std::ifstream file(path);
        if (!file.is_open()) {
            std::cerr << "Failed to open project file " << path << std::endl;
            return;
        }
        
        clearAll();
        
        // Simple JSON parsing for .layout files
        std::string line;
        bool inElements = false;
        WireframeElement currentElement;
        
        while (std::getline(file, line)) {
            if (line.find("\"elements\"") != std::string::npos) {
                inElements = true;
                continue;
            }
            
            if (inElements && line.find("\"type\"") != std::string::npos) {
                size_t start = line.find(":") + 1;
                start = line.find("\"", start) + 1;
                size_t end = line.find("\"", start);
                currentElement.type = line.substr(start, end - start);
            }
            
            if (inElements && line.find("\"id\"") != std::string::npos) {
                size_t start = line.find(":") + 1;
                start = line.find("\"", start) + 1;
                size_t end = line.find("\"", start);
                currentElement.id = line.substr(start, end - start);
            }
            
            if (inElements && line.find("\"x\"") != std::string::npos) {
                size_t pos = line.find(":") + 1;
                currentElement.x = std::stoi(line.substr(pos));
            }
            
            if (inElements && line.find("\"y\"") != std::string::npos) {
                size_t pos = line.find(":") + 1;
                currentElement.y = std::stoi(line.substr(pos));
            }
            
            if (inElements && line.find("\"width\"") != std::string::npos) {
                size_t pos = line.find(":") + 1;
                currentElement.width = std::stoi(line.substr(pos));
            }
            
            if (inElements && line.find("\"height\"") != std::string::npos) {
                size_t pos = line.find(":") + 1;
                currentElement.height = std::stoi(line.substr(pos));
            }
            
            if (inElements && line.find("\"text\"") != std::string::npos) {
                size_t start = line.find(":") + 1;
                start = line.find("\"", start) + 1;
                size_t end = line.find("\"", start);
                currentElement.text = line.substr(start, end - start);
                
                // Element complete, add to canvas
                addLoadedElementToCanvas(currentElement);
                currentElement = WireframeElement(); // Reset
            }
        }
        
        file.close();
        currentProjectPath = path;
        hasUnsavedChanges = false;
        std::cout << "Project loaded from " << path << std::endl;
    }
    
    void addLoadedElementToCanvas(const WireframeElement& elem) {
        // Create UI element based on type
        std::shared_ptr<ui::UIElement> element;
        
        if (elem.type == "button") {
            element = ui->createButton(elem.text, 220 + elem.x, 60 + elem.y, [](){});
        } else if (elem.type == "label") {
            element = ui->createLabel(elem.text, 220 + elem.x, 60 + elem.y);
        } else if (elem.type == "textbox") {
            element = ui->createTextBox("", 220 + elem.x, 60 + elem.y, elem.width, elem.height);
        } else if (elem.type == "canvas") {
            element = ui->createCanvas(220 + elem.x, 60 + elem.y, elem.width, elem.height);
        } else if (elem.type == "checkbox") {
            element = ui->createCheckBox(false, 220 + elem.x, 60 + elem.y, [](bool){});
        } else if (elem.type == "hslider") {
            element = ui->createHSlider(220 + elem.x, 60 + elem.y, elem.width, elem.height, 0.0f, 1.0f, 0.5f);
        } else if (elem.type == "vslider") {
            element = ui->createVSlider(220 + elem.x, 60 + elem.y, elem.width, elem.height, 0.0f, 1.0f, 0.5f);
        } else if (elem.type == "progress") {
            element = ui->createProgressBar(220 + elem.x, 60 + elem.y, elem.width, elem.height);
        } else if (elem.type == "image") {
            element = ui->createImage("", 220 + elem.x, 60 + elem.y, elem.width, elem.height);
        } else if (elem.type == "cyclelist") {
            element = ui->createCycleList(220 + elem.x, 60 + elem.y, elem.width, elem.height, {"Option1", "Option2"});
        } else if (elem.type == "optionselect") {
            element = ui->createOptionSelect({"Option1", "Option2"}, 0, 220 + elem.x, 60 + elem.y, [](int){});
        }
        
        if (element) {
            element->setSize(elem.width, elem.height);
            canvasElements.push_back(element);
            wireframeElements.push_back(elem);
            
            // Set up selection for buttons and checkboxes
            if (elem.type == "button") {
                auto buttonElement = std::static_pointer_cast<ui::Button>(element);
                buttonElement->setCallback([this, element](){
                    selectElement(element);
                });
                selectionButtons.push_back(nullptr);
            } else if (elem.type == "checkbox") {
                auto checkboxElement = std::static_pointer_cast<ui::CheckBox>(element);
                checkboxElement->setCallback([this, element](bool){
                    selectElement(element);
                });
                selectionButtons.push_back(nullptr);
            } else {
                // Add selection button for non-clickable elements
                int centerX = element->getX() + element->getWidth() / 2 - 10;
                int centerY = element->getY() + element->getHeight() / 2 - 10;
                auto selectBtn = ui->createButton("S", centerX, centerY, [this, element](){
                    selectElement(element);
                });
                selectBtn->setSize(20, 20);
                selectionButtons.push_back(selectBtn);
            }
            
            // Create name label for loaded element
            addElementNameLabel(element, elem.id);
            
            // Update element counters
            std::string baseType = elem.id;
            size_t pos = baseType.find_last_not_of("0123456789");
            if (pos != std::string::npos) {
                baseType = baseType.substr(0, pos + 1);
                std::string numStr = elem.id.substr(pos + 1);
                if (!numStr.empty()) {
                    int num = std::stoi(numStr);
                    elementCounters[baseType] = std::max(elementCounters[baseType], num);
                }
            }
        }
    }
    
    void clearAll() {
        // Clear wireframe data
        wireframeElements.clear();
        
        // Remove canvas elements
        for (auto& elem : canvasElements) {
            ui->removeElement(elem->getId());
        }
        canvasElements.clear();
        
        // Remove selection buttons
        for (auto& btn : selectionButtons) {
            if (btn) ui->removeElement(btn->getId());
        }
        selectionButtons.clear();
        
        // Remove name labels
        for (auto& label : elementNameLabels) {
            if (label) ui->removeElement(label->getId());
        }
        elementNameLabels.clear();
        
        // Remove resize handles
        for (auto& handle : resizeHandles) {
            if (handle) ui->removeElement(handle->getId());
        }
        resizeHandles.clear();
        
        // Clear selection
        selectedElement = nullptr;
        selectedWireframeElement = nullptr;
        
        // Reset counters
        elementCounters.clear();
        
        // Clear undo/redo stacks
        undoStack.clear();
        redoStack.clear();
        
        hasUnsavedChanges = false;
    }
    
    void pushUndoAction(const UndoAction& action) {
        undoStack.push_back(action);
        if (undoStack.size() > MAX_UNDO_STACK) {
            undoStack.erase(undoStack.begin());
        }
        redoStack.clear(); // Clear redo stack when new action is performed
        hasUnsavedChanges = true;
    }
    
    void undo() {
        if (undoStack.empty()) {
            std::cout << "Nothing to undo" << std::endl;
            return;
        }
        
        UndoAction action = undoStack.back();
        undoStack.pop_back();
        
        // Create redo action before applying undo
        UndoAction redoAction;
        
        switch (action.type) {
            case UndoAction::ADD_ELEMENT:
                // Undo add: remove element
                redoAction.type = UndoAction::ADD_ELEMENT;
                redoAction.element = action.element;
                removeElementById(action.element.id);
                break;
                
            case UndoAction::REMOVE_ELEMENT:
                // Undo remove: add element back
                redoAction.type = UndoAction::REMOVE_ELEMENT;
                redoAction.element = action.element;
                redoAction.index = wireframeElements.size();
                addLoadedElementToCanvas(action.element);
                break;
                
            case UndoAction::MODIFY_ELEMENT:
                // Undo modify: restore old values
                redoAction.type = UndoAction::MODIFY_ELEMENT;
                redoAction.element = action.oldElement;
                redoAction.oldElement = action.element;
                modifyElement(action.element.id, action.oldElement);
                break;
                
            case UndoAction::CLEAR_ALL:
                // Undo clear: restore all elements (not implemented for simplicity)
                break;
        }
        
        redoStack.push_back(redoAction);
        std::cout << "Undo completed" << std::endl;
    }
    
    void redo() {
        if (redoStack.empty()) {
            std::cout << "Nothing to redo" << std::endl;
            return;
        }
        
        UndoAction action = redoStack.back();
        redoStack.pop_back();
        
        switch (action.type) {
            case UndoAction::ADD_ELEMENT:
                addLoadedElementToCanvas(action.element);
                break;
                
            case UndoAction::REMOVE_ELEMENT:
                removeElementById(action.element.id);
                break;
                
            case UndoAction::MODIFY_ELEMENT:
                modifyElement(action.element.id, action.element);
                break;
                
            case UndoAction::CLEAR_ALL:
                break;
        }
        
        undoStack.push_back(action);
        std::cout << "Redo completed" << std::endl;
    }
    
    void removeElementById(const std::string& id) {
        // Find and remove from wireframe elements
        auto it = std::find_if(wireframeElements.begin(), wireframeElements.end(),
            [&id](const WireframeElement& elem) { return elem.id == id; });
        
        if (it != wireframeElements.end()) {
            wireframeElements.erase(it);
        }
        
        // Find and remove from canvas elements
        for (size_t i = 0; i < canvasElements.size(); ++i) {
            // Check if this element matches by comparing properties
            auto& elem = canvasElements[i];
            bool matches = false;
            
            // Simple matching by position (since we don't store ID in UI elements)
            for (const auto& wf : wireframeElements) {
                if (elem->getX() == 220 + wf.x && elem->getY() == 30 + wf.y) {
                    matches = false; // Still exists in wireframe
                    break;
                }
            }
            
            if (!matches) {
                ui->removeElement(elem->getId());
                canvasElements.erase(canvasElements.begin() + i);
                
                // Remove corresponding selection button
                if (i < selectionButtons.size() && selectionButtons[i]) {
                    ui->removeElement(selectionButtons[i]->getId());
                    selectionButtons.erase(selectionButtons.begin() + i);
                }
                break;
            }
        }
    }
    
    void modifyElement(const std::string& id, const WireframeElement& newData) {
        // Find wireframe element and update
        for (auto& elem : wireframeElements) {
            if (elem.id == id) {
                elem = newData;
                break;
            }
        }
        
        // Update corresponding UI element
        for (auto& elem : canvasElements) {
            if (elem->getX() == 220 + newData.x && elem->getY() == 30 + newData.y) {
                elem->setPosition(220 + newData.x, 30 + newData.y);
                elem->setSize(newData.width, newData.height);
                break;
            }
        }
        
        updatePropertiesPanel();
    }
};

int main(int argc, char* argv[]) {
    try {
        UI ui("UI Layout Editor", 1550, 800);  // Expanded width for properties panel
        LayoutEditor editor(&ui);
        
        std::string wireframeFile;
        
        // Get the directory where the binary is located
        std::string binaryPath = argv[0];
        size_t lastSlash = binaryPath.find_last_of("/\\");
        std::string binaryDir = (lastSlash != std::string::npos) ? binaryPath.substr(0, lastSlash + 1) : "./";
        
        // Set binary directory for the editor
        editor.setBinaryDirectory(binaryDir);
        
        // Parse command line arguments
        if (argc >= 2 && (std::string(argv[1]) == "--help" || std::string(argv[1]) == "-h")) {
            std::cout << "UI Layout Editor - Visual wireframing tool" << std::endl;
            std::cout << "Usage: " << argv[0] << " [--file <path>]" << std::endl;
            std::cout << "  --file <path>  Load specific wireframe file" << std::endl;
            std::cout << "  (no args)      Load wireframe.json from binary directory" << std::endl;
            return 0;
        }
        
        if (argc >= 3 && std::string(argv[1]) == "--file") {
            wireframeFile = argv[2];
        } else {
            // Default to wireframe.json in same directory as binary
            wireframeFile = binaryDir + "wireframe.json";
        }
        
        // Check if file exists and load it
        std::ifstream testFile(wireframeFile);
        if (testFile.good()) {
            testFile.close();
            editor.loadProjectFromFile(wireframeFile);
            std::cout << "Loaded wireframe from: " << wireframeFile << std::endl;
        } else {
            // Create empty wireframe.json if it doesn't exist (only for default file)
            if (wireframeFile == binaryDir + "wireframe.json") {
                std::ofstream newFile(wireframeFile);
                newFile << "{\n";
                newFile << "  \"canvas\": {\"width\": 1024, \"height\": 600},\n";
                newFile << "  \"elements\": []\n";
                newFile << "}\n";
                newFile.close();
                std::cout << "Created new wireframe.json at: " << wireframeFile << std::endl;
            } else {
                std::cout << "File not found: " << wireframeFile << std::endl;
            }
        }
        
        ui.run();
        
    } catch (const ui::UIException& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    
    return 0;
}
