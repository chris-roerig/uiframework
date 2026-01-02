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

class LayoutEditor {
private:
    UI* ui;
    std::vector<WireframeElement> wireframeElements;
    std::shared_ptr<ui::UIElement> selectedElement;
    std::map<std::string, int> elementCounters; // For auto-naming
    std::vector<std::shared_ptr<ui::UIElement>> canvasElements;
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

public:
    LayoutEditor(UI* uiInstance) : ui(uiInstance) {
        setupUI();
    }

    void setupUI() {
        // Enable 10px grid for precise positioning
        ui->setGridSize(10);
        
        // Create separator lines for visual separation
        setupSeparatorLines();
        
        // Create palette area (left side)
        auto paletteTitle = ui->createLabel("Element Palette", 10, 10);
        
        // Palette buttons - vertical list
        auto buttonPalette = ui->createButton("Button", 10, 40, [this](){
            addElementToCanvas("button", generateElementName("Button"));
        });
        
        auto labelPalette = ui->createButton("Label", 10, 80, [this](){
            addElementToCanvas("label", generateElementName("Label"));
        });
        
        auto textboxPalette = ui->createButton("TextBox", 10, 120, [this](){
            addElementToCanvas("textbox", generateElementName("TextBox"));
        });
        
        auto canvasPalette = ui->createButton("Canvas", 10, 160, [this](){
            addElementToCanvas("canvas", generateElementName("Canvas"));
        });
        
        auto checkboxPalette = ui->createButton("CheckBox", 10, 200, [this](){
            addElementToCanvas("checkbox", generateElementName("CheckBox"));
        });
        
        auto hsliderPalette = ui->createButton("HSlider", 10, 240, [this](){
            addElementToCanvas("hslider", generateElementName("HSlider"));
        });
        
        auto vsliderPalette = ui->createButton("VSlider", 10, 280, [this](){
            addElementToCanvas("vslider", generateElementName("VSlider"));
        });
        
        auto progressPalette = ui->createButton("Progress", 10, 320, [this](){
            addElementToCanvas("progress", generateElementName("ProgressBar"));
        });
        
        auto imagePalette = ui->createButton("Image", 10, 360, [this](){
            addElementToCanvas("image", generateElementName("Image"));
        });
        
        auto cyclelistPalette = ui->createButton("CycleList", 10, 400, [this](){
            addElementToCanvas("cyclelist", generateElementName("CycleList"));
        });
        
        auto optionselectPalette = ui->createButton("OptionSelect", 10, 440, [this](){
            addElementToCanvas("optionselect", generateElementName("OptionSelect"));
        });
        
        // Canvas area separator
        auto separator = ui->createLabel("Canvas (1024x600)", 200, 10);
        
        // Create canvas background
        auto canvas = ui->createCanvas(220, 30, 1024, 600);
        
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
        
        auto saveBtn = ui->createButton("Save Project", 130, 740, [this](){
            saveProject();
        });
        
        auto loadBtn = ui->createButton("Load Project", 250, 740, [this](){
            loadProject();
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
        std::cout << "- Canvas: 1024x600 with 10px grid" << std::endl;
        std::cout << "- Click palette buttons to add elements" << std::endl;
        std::cout << "- Click buttons/checkboxes directly to select them" << std::endl;
        std::cout << "- Use properties panel to edit selected element" << std::endl;
        std::cout << "- Numpad keys: Move selected element (5=up, 2=down, 1=left, 3=right)" << std::endl;
        std::cout << "- Shift+Numpad keys: Resize selected element" << std::endl;
        std::cout << "- Use Export JSON to get wireframe data" << std::endl;
        
        // Setup keyboard shortcuts
        setupKeyboardShortcuts();
    }
    
    void setupKeyboardShortcuts() {
        // Numpad keys for movement (easier with shift)
        ui->assignHotKey("", "5", [this](){
            if (SDL_GetModState() & KMOD_SHIFT) {
                resizeSelectedElement(0, -10); // Shift+5: Make shorter
            } else {
                moveSelectedElement(0, -10); // 5: Move up
            }
        });
        
        ui->assignHotKey("", "2", [this](){
            if (SDL_GetModState() & KMOD_SHIFT) {
                resizeSelectedElement(0, 10); // Shift+2: Make taller
            } else {
                moveSelectedElement(0, 10); // 2: Move down
            }
        });
        
        ui->assignHotKey("", "1", [this](){
            if (SDL_GetModState() & KMOD_SHIFT) {
                resizeSelectedElement(-10, 0); // Shift+1: Make narrower
            } else {
                moveSelectedElement(-10, 0); // 1: Move left
            }
        });
        
        ui->assignHotKey("", "3", [this](){
            if (SDL_GetModState() & KMOD_SHIFT) {
                resizeSelectedElement(10, 0); // Shift+3: Make wider
            } else {
                moveSelectedElement(10, 0); // 3: Move right
            }
        });
    }
    
    void setupSeparatorLines() {
        // Vertical line between palette and canvas
        auto paletteCanvasSeparator = ui->createCanvas(215, 0, 2, 800);
        paletteCanvasSeparator->filledRect({0, 0, 2, 800}, ui::Color(180, 180, 180, 255)); // Gray line
        
        // Vertical line between canvas and properties panel
        auto canvasPropertiesSeparator = ui->createCanvas(1250, 0, 2, 800);
        canvasPropertiesSeparator->filledRect({0, 0, 2, 800}, ui::Color(180, 180, 180, 255)); // Gray line
        
        // Horizontal line above bottom toolbar (aligned with vertical separators)
        auto toolbarSeparator = ui->createCanvas(215, 730, 1037, 2);
        toolbarSeparator->filledRect({0, 0, 1037, 2}, ui::Color(180, 180, 180, 255)); // Gray line
    }
    
    std::string generateElementName(const std::string& type) {
        elementCounters[type]++;
        return type + std::to_string(elementCounters[type]);
    }
    
    void setupPropertiesPanel() {
        int panelX = 1270; // Right side of window
        
        // Properties panel title
        auto propTitle = ui->createLabel("Element Properties", panelX, 10);
        
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
        if (!selectedElement) {
            clearPropertiesPanel();
            return;
        }
        
        // Find wireframe element for this UI element
        WireframeElement* wfElement = nullptr;
        for (auto& elem : wireframeElements) {
            if (elem.id == selectedElement->getId()) {
                wfElement = &elem;
                break;
            }
        }
        
        if (!wfElement) return;
        
        // Update properties panel
        propTypeLabel->setText(wfElement->type);
        propNameInput->setText(wfElement->text);
        propXInput->setText(std::to_string(selectedElement->getX()));
        propYInput->setText(std::to_string(selectedElement->getY()));
        propWidthInput->setText(std::to_string(selectedElement->getWidth()));
        propHeightInput->setText(std::to_string(selectedElement->getHeight()));
        propTextInput->setText(wfElement->text);
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
            
            // Validate bounds
            newX = std::max(220, std::min(1220 - newWidth, newX));
            newY = std::max(30, std::min(630 - newHeight, newY));
            newWidth = std::max(20, newWidth);
            newHeight = std::max(20, newHeight);
            
            // Apply changes to UI element
            selectedElement->setPosition(newX, newY);
            selectedElement->setSize(newWidth, newHeight);
            
            // Update wireframe data
            for (auto& elem : wireframeElements) {
                if (elem.id == selectedElement->getId()) {
                    elem.x = newX;
                    elem.y = newY;
                    elem.width = newWidth;
                    elem.height = newHeight;
                    elem.text = newText;
                    break;
                }
            }
            
            // Update visual elements
            updateSelectionButtonPosition(selectedElement);
            addResizeHandles(selectedElement);
            
            std::cout << "Applied property changes to " << selectedElement->getId() << std::endl;
            
        } catch (const std::exception& e) {
            std::cout << "Invalid property values. Please enter valid numbers." << std::endl;
        }
    }
    
    void addElementToCanvas(const std::string& type, const std::string& text) {
        // Add element to canvas area (starting at x=220 to avoid palette)
        int startX = 220 + (wireframeElements.size() * 20) % 400;
        int startY = 50 + (wireframeElements.size() * 30) % 300;
        
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
            wfElement.id = element->getId();
            wfElement.x = element->getX();
            wfElement.y = element->getY();
            wfElement.width = element->getWidth();
            wfElement.height = element->getHeight();
            wfElement.text = text;
            
            wireframeElements.push_back(wfElement);
            
            // Auto-select the newly added element
            selectElement(element);
            
            std::cout << "Added " << type << " at (" << startX << "," << startY << ")" << std::endl;
        }
    }
    
    void exportToJSON() {
        std::ofstream file("wireframe.json");
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
        
        std::cout << "Exported wireframe to wireframe.json (" << wireframeElements.size() << " elements)" << std::endl;
    }
    
    void saveProject() {
        exportToJSON();
        std::cout << "Project saved as wireframe.json" << std::endl;
    }
    
    void loadProject() {
        std::cout << "Load functionality not implemented yet" << std::endl;
    }
    
    void selectElement(std::shared_ptr<ui::UIElement> element) {
        selectedElement = element;
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
        updateWireframeElement(selectedElement->getId(), newX, newY);
        
        // Update resize handles
        addResizeHandles(selectedElement);
        
        // Update selection button position if it exists
        updateSelectionButtonPosition(selectedElement);
        
        // Update properties panel
        updatePropertiesPanel();
        
        std::cout << "Moved element to (" << newX << "," << newY << ")" << std::endl;
    }
    
    void updateWireframeElement(const std::string& elementId, int newX, int newY) {
        for (auto& wfElement : wireframeElements) {
            if (wfElement.id == elementId) {
                wfElement.x = newX;
                wfElement.y = newY;
                break;
            }
        }
    }
    
    void deleteSelectedElement() {
        if (!selectedElement) {
            std::cout << "No element selected. Click an element first." << std::endl;
            return;
        }
        
        std::string elementId = selectedElement->getId();
        
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
        }
        
        // Remove from wireframe data
        wireframeElements.erase(
            std::remove_if(wireframeElements.begin(), wireframeElements.end(),
                [&elementId](const WireframeElement& elem) {
                    return elem.id == elementId;
                }),
            wireframeElements.end()
        );
        
        std::cout << "Deleted element: " << elementId << std::endl;
        selectedElement = nullptr;
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
        updateWireframeElementSize(selectedElement->getId(), newW, newH);
        
        // Update visual handles
        addResizeHandles(selectedElement);
        
        // Update selection button position
        updateSelectionButtonPosition(selectedElement);
        
        // Update properties panel
        updatePropertiesPanel();
        
        std::cout << "Resized element to " << newW << "x" << newH << std::endl;
    }
    
    void updateWireframeElementSize(const std::string& elementId, int newW, int newH) {
        for (auto& wfElement : wireframeElements) {
            if (wfElement.id == elementId) {
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
};

int main() {
    try {
        UI ui("UI Layout Editor", 1550, 800);  // Expanded width for properties panel
        LayoutEditor editor(&ui);
        
        ui.run();
        
    } catch (const ui::UIException& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    
    return 0;
}
