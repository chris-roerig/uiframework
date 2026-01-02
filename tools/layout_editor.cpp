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
        
        // Canvas area separator
        auto separator = ui->createLabel("Canvas (1024x600)", 200, 10);
        
        // Export/Save buttons
        auto exportBtn = ui->createButton("Export JSON", 10, 200, [this](){
            exportToJSON();
        });
        
        auto saveBtn = ui->createButton("Save Project", 10, 240, [this](){
            saveProject();
        });
        
        auto loadBtn = ui->createButton("Load Project", 10, 280, [this](){
            loadProject();
        });
        
        // Element manipulation controls
        auto selectLabel = ui->createLabel("Element Controls:", 10, 320);
        
        auto moveUpBtn = ui->createButton("Move Up", 10, 350, [this](){
            moveSelectedElement(0, -10);
        });
        
        auto moveDownBtn = ui->createButton("Move Down", 10, 390, [this](){
            moveSelectedElement(0, 10);
        });
        
        auto moveLeftBtn = ui->createButton("Move Left", 10, 430, [this](){
            moveSelectedElement(-10, 0);
        });
        
        auto moveRightBtn = ui->createButton("Move Right", 10, 470, [this](){
            moveSelectedElement(10, 0);
        });
        
        auto deleteBtn = ui->createButton("Delete", 10, 510, [this](){
            deleteSelectedElement();
        });
        
        std::cout << "Layout Editor Started!" << std::endl;
        std::cout << "- Canvas: 1024x600 with 10px grid" << std::endl;
        std::cout << "- Click palette buttons to add elements" << std::endl;
        std::cout << "- Click elements to select them" << std::endl;
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
        }
        
        if (element) {
            // Make element clickable for selection
            if (type == "button") {
                // For buttons, we need to override the callback to handle selection
                auto buttonElement = std::static_pointer_cast<ui::Button>(element);
                buttonElement->setCallback([this, element](){
                    selectElement(element);
                });
            }
            
            // Store element reference for interaction
            canvasElements.push_back(element);
            
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
                  << element->getX() << "," << element->getY() << ")" << std::endl;
        
        // Visual feedback - create selection indicator
        if (selectedIndicator) {
            ui->removeElement(selectedIndicator->getId());
        }
        
        // Create a border around selected element (simple label for now)
        std::string indicatorText = "[SELECTED: " + element->getId() + "]";
        selectedIndicator = ui->createLabel(indicatorText, 
                                          element->getX() - 5, 
                                          element->getY() - 20);
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
        
        // Remove from UI
        ui->removeElement(elementId);
        
        // Remove from canvas elements
        canvasElements.erase(
            std::remove(canvasElements.begin(), canvasElements.end(), selectedElement),
            canvasElements.end()
        );
        
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
};

int main() {
    try {
        UI ui("UI Layout Editor", 1200, 700);
        LayoutEditor editor(&ui);
        
        ui.run();
        
    } catch (const ui::UIException& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    
    return 0;
}
