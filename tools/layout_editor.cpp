#include "uiframework/UI.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
#include <algorithm>

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
    int nextElementId = 1;
    std::vector<std::shared_ptr<ui::UIElement>> canvasElements;
    std::shared_ptr<ui::UIElement> selectedIndicator;
    std::vector<std::shared_ptr<ui::UIElement>> resizeHandles;
    std::vector<std::shared_ptr<ui::UIElement>> selectionButtons;

public:
    LayoutEditor(UI* uiInstance) : ui(uiInstance) {
        setupUI();
    }

    void setupUI() {
        // Enable 10px grid for precise positioning
        ui->setGridSize(10);
        
        // Create palette area (left side)
        auto paletteTitle = ui->createLabel("Element Palette", 10, 10);
        
        // Palette buttons
        auto buttonPalette = ui->createButton("Button", 10, 40, [this](){
            addElementToCanvas("button", "Button " + std::to_string(nextElementId++));
        });
        
        auto labelPalette = ui->createButton("Label", 10, 80, [this](){
            addElementToCanvas("label", "Label " + std::to_string(nextElementId++));
        });
        
        auto textboxPalette = ui->createButton("TextBox", 10, 120, [this](){
            addElementToCanvas("textbox", "Text " + std::to_string(nextElementId++));
        });
        
        auto canvasPalette = ui->createButton("Canvas", 10, 160, [this](){
            addElementToCanvas("canvas", "Canvas " + std::to_string(nextElementId++));
        });
        
        auto checkboxPalette = ui->createButton("CheckBox", 10, 200, [this](){
            addElementToCanvas("checkbox", "Check " + std::to_string(nextElementId++));
        });
        
        auto hsliderPalette = ui->createButton("HSlider", 100, 40, [this](){
            addElementToCanvas("hslider", "Slider " + std::to_string(nextElementId++));
        });
        
        auto vsliderPalette = ui->createButton("VSlider", 100, 80, [this](){
            addElementToCanvas("vslider", "VSlider " + std::to_string(nextElementId++));
        });
        
        auto progressPalette = ui->createButton("Progress", 100, 120, [this](){
            addElementToCanvas("progress", "Progress " + std::to_string(nextElementId++));
        });
        
        auto imagePalette = ui->createButton("Image", 100, 160, [this](){
            addElementToCanvas("image", "Image " + std::to_string(nextElementId++));
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
        
        // Export/Save buttons
        auto exportBtn = ui->createButton("Export JSON", 10, 240, [this](){
            exportToJSON();
        });
        
        auto saveBtn = ui->createButton("Save Project", 10, 280, [this](){
            saveProject();
        });
        
        auto loadBtn = ui->createButton("Load Project", 10, 320, [this](){
            loadProject();
        });
        
        // Element manipulation controls
        auto selectLabel = ui->createLabel("Element Controls:", 10, 360);
        
        auto moveUpBtn = ui->createButton("Move Up", 10, 390, [this](){
            moveSelectedElement(0, -10);
        });
        
        auto moveDownBtn = ui->createButton("Move Down", 10, 430, [this](){
            moveSelectedElement(0, 10);
        });
        
        auto moveLeftBtn = ui->createButton("Move Left", 10, 470, [this](){
            moveSelectedElement(-10, 0);
        });
        
        auto moveRightBtn = ui->createButton("Move Right", 10, 510, [this](){
            moveSelectedElement(10, 0);
        });
        
        auto deleteBtn = ui->createButton("Delete", 10, 550, [this](){
            deleteSelectedElement();
        });
        
        // Resize controls
        auto resizeLabel = ui->createLabel("Resize:", 10, 590);
        
        auto widerBtn = ui->createButton("Wider", 10, 620, [this](){
            resizeSelectedElement(10, 0);
        });
        
        auto narrowerBtn = ui->createButton("Narrower", 10, 660, [this](){
            resizeSelectedElement(-10, 0);
        });
        
        auto tallerBtn = ui->createButton("Taller", 100, 620, [this](){
            resizeSelectedElement(0, 10);
        });
        
        auto shorterBtn = ui->createButton("Shorter", 100, 660, [this](){
            resizeSelectedElement(0, -10);
        });
        
        std::cout << "Layout Editor Started!" << std::endl;
        std::cout << "- Canvas: 1024x600 with 10px grid" << std::endl;
        std::cout << "- Click palette buttons to add elements" << std::endl;
        std::cout << "- Click buttons/checkboxes directly to select them" << std::endl;
        std::cout << "- Use Move buttons to position selected element" << std::endl;
        std::cout << "- Use Export JSON to get wireframe data" << std::endl;
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
        
        // Visual feedback - create selection indicator with resize handles
        if (selectedIndicator) {
            ui->removeElement(selectedIndicator->getId());
        }
        
        std::string indicatorText = "[SELECTED: " + element->getId() + "]";
        selectedIndicator = ui->createLabel(indicatorText, 
                                          element->getX() - 5, 
                                          element->getY() - 20);
        
        // Add resize handles (visual indicators)
        addResizeHandles(element);
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
        
        // Update selection indicator
        if (selectedIndicator) {
            selectedIndicator->setPosition(newX - 5, newY - 20);
        }
        
        // Update resize handles
        addResizeHandles(selectedElement);
        
        // Update selection button position if it exists
        updateSelectionButtonPosition(selectedElement);
        
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
        
        // Create small visual handles at corners and edges
        resizeHandles.push_back(ui->createLabel("□", x + w - 5, y + h - 5)); // Bottom-right
        resizeHandles.push_back(ui->createLabel("□", x + w - 5, y - 5));     // Top-right
        resizeHandles.push_back(ui->createLabel("□", x - 5, y + h - 5));     // Bottom-left
        resizeHandles.push_back(ui->createLabel("□", x - 5, y - 5));         // Top-left
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
        UI ui("UI Layout Editor", 1300, 700);  // Increased width to accommodate 1024px canvas
        LayoutEditor editor(&ui);
        
        ui.run();
        
    } catch (const ui::UIException& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    
    return 0;
}
