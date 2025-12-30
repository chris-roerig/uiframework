#include "uiframework/UI.h"
#include <iostream>
#include <vector>

int main() {
    try {
        UI ui("Custom Focus Order Demo", 900, 700);
        
        // Create elements in visual layout order (left-to-right, top-to-bottom)
        auto field1 = ui.createTextBox("Field 1", 50, 50);
        field1->setSize(120, 30);
        
        auto field2 = ui.createTextBox("Field 2", 200, 50);
        field2->setSize(120, 30);
        
        auto field3 = ui.createTextBox("Field 3", 350, 50);
        field3->setSize(120, 30);
        
        auto field4 = ui.createTextBox("Field 4", 500, 50);
        field4->setSize(120, 30);
        
        auto field5 = ui.createTextBox("Field 5", 50, 100);
        field5->setSize(120, 30);
        
        auto field6 = ui.createTextBox("Field 6", 200, 100);
        field6->setSize(120, 30);
        
        auto field7 = ui.createTextBox("Field 7", 350, 100);
        field7->setSize(120, 30);
        
        auto field8 = ui.createTextBox("Field 8", 500, 100);
        field8->setSize(120, 30);
        
        // Additional elements for more complex ordering
        auto priority = ui.createCycleList(50, 200, 120, 30, {"Low", "Medium", "High"});
        auto urgent = ui.createCheckBox(false, 200, 200, [](bool checked) {
            std::cout << "Urgent: " << (checked ? "Yes" : "No") << std::endl;
        });
        auto submit = ui.createButton("Submit", 350, 200, []() {
            std::cout << "Form submitted!" << std::endl;
        });
        auto cancel = ui.createButton("Cancel", 500, 200, []() {
            std::cout << "Form cancelled!" << std::endl;
        });
        
        // Labels for clarity
        ui.createLabel("=== DEFAULT TAB ORDER (Registration Order) ===", 50, 250);
        ui.createLabel("Press TAB to see default order:", 50, 270);
        ui.createLabel("Field1 → Field2 → Field3 → Field4 → Field5 → Field6 → Field7 → Field8 → Priority → Urgent → Submit → Cancel", 50, 290);
        
        ui.createLabel("=== CUSTOM TAB ORDER EXAMPLES ===", 50, 330);
        ui.createLabel("Press 'c' for Column-wise order", 50, 350);
        ui.createLabel("Press 'z' for Z-pattern order", 50, 370);
        ui.createLabel("Press 'f' for Form-logical order", 50, 390);
        ui.createLabel("Press 'r' to Reset to default order", 50, 410);
        
        // Instructions
        ui.createLabel("=== INSTRUCTIONS ===", 50, 450);
        ui.createLabel("• TAB/SHIFT+TAB to navigate", 50, 470);
        ui.createLabel("• Press hotkeys (c/z/f/r) to change focus order", 50, 490);
        ui.createLabel("• Yellow border shows focused element", 50, 510);
        ui.createLabel("• Notice how TAB order changes with each pattern", 50, 530);
        
        // Current order display
        auto orderLabel = ui.createLabel("Current Order: DEFAULT (registration order)", 50, 570);
        
        // Hotkey handlers for different focus orders
        ui.assignHotKey(field1, "c", [&ui, &orderLabel, field1, field5, field2, field6, field3, field7, field4, field8, priority, urgent, submit, cancel]() {
            // Column-wise order: top-to-bottom, left-to-right
            std::vector<std::string> columnOrder = {
                field1->getId(), field5->getId(),  // Column 1
                field2->getId(), field6->getId(),  // Column 2
                field3->getId(), field7->getId(),  // Column 3
                field4->getId(), field8->getId(),  // Column 4
                priority->getId(), urgent->getId(), submit->getId(), cancel->getId()
            };
            ui.setFocusOrder(columnOrder);
            orderLabel->setText("Current Order: COLUMN-WISE (top-to-bottom, left-to-right)");
            std::cout << "Focus order changed to: Column-wise" << std::endl;
        });
        
        ui.assignHotKey(field1, "z", [&ui, &orderLabel, field1, field2, field3, field4, field8, field7, field6, field5, priority, urgent, submit, cancel]() {
            // Z-pattern order: left-to-right top row, right-to-left bottom row
            std::vector<std::string> zOrder = {
                field1->getId(), field2->getId(), field3->getId(), field4->getId(),  // Top row L→R
                field8->getId(), field7->getId(), field6->getId(), field5->getId(),  // Bottom row R→L
                priority->getId(), urgent->getId(), submit->getId(), cancel->getId()
            };
            ui.setFocusOrder(zOrder);
            orderLabel->setText("Current Order: Z-PATTERN (L→R top, R→L bottom)");
            std::cout << "Focus order changed to: Z-pattern" << std::endl;
        });
        
        ui.assignHotKey(field1, "f", [&ui, &orderLabel, field1, field2, field5, field6, field3, field4, field7, field8, priority, urgent, submit, cancel]() {
            // Form-logical order: related fields grouped together
            std::vector<std::string> formOrder = {
                field1->getId(), field2->getId(),  // Name fields
                field5->getId(), field6->getId(),  // Address fields  
                field3->getId(), field4->getId(),  // Contact fields
                field7->getId(), field8->getId(),  // Additional info
                priority->getId(), urgent->getId(),  // Options
                submit->getId(), cancel->getId()   // Actions
            };
            ui.setFocusOrder(formOrder);
            orderLabel->setText("Current Order: FORM-LOGICAL (grouped by purpose)");
            std::cout << "Focus order changed to: Form-logical" << std::endl;
        });
        
        ui.assignHotKey(field1, "r", [&ui, &orderLabel]() {
            // Reset to default (empty focus order uses registration order)
            std::vector<std::string> emptyOrder;
            ui.setFocusOrder(emptyOrder);
            orderLabel->setText("Current Order: DEFAULT (registration order)");
            std::cout << "Focus order reset to: Default" << std::endl;
        });
        
        // Field labels for context
        ui.createLabel("First Name", 50, 30);
        ui.createLabel("Last Name", 200, 30);
        ui.createLabel("Email", 350, 30);
        ui.createLabel("Phone", 500, 30);
        ui.createLabel("Address", 50, 80);
        ui.createLabel("City", 200, 80);
        ui.createLabel("State", 350, 80);
        ui.createLabel("ZIP", 500, 80);
        ui.createLabel("Priority", 50, 180);
        ui.createLabel("Urgent", 220, 200);
        
        std::cout << "=== Custom Focus Order Demo ===" << std::endl;
        std::cout << "Default order: Field1→Field2→Field3→Field4→Field5→Field6→Field7→Field8→Priority→Urgent→Submit→Cancel" << std::endl;
        std::cout << "Press 'c' for column-wise order" << std::endl;
        std::cout << "Press 'z' for Z-pattern order" << std::endl;
        std::cout << "Press 'f' for form-logical order" << std::endl;
        std::cout << "Press 'r' to reset to default" << std::endl;
        std::cout << "Use TAB/SHIFT+TAB to navigate and see the difference!" << std::endl;
        std::cout << "=========================" << std::endl;
        
        ui.run();
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    
    return 0;
}
