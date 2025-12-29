// Generated from dev/examples/advanced.ui
// DO NOT EDIT - This file is auto-generated

#pragma once
#include "uiframework/UI.h"
#include "uiframework/Layout/VBoxLayout.h"
#include "uiframework/Layout/HBoxLayout.h"
#include "uiframework/Layout/GridLayout.h"
#include "uiframework/UIElements/LayoutContainer.h"
#include <memory>

namespace ui_generated {

struct AdvancedWidgets {
    std::shared_ptr<ui::VBoxLayout> mainLayout;
    std::shared_ptr<ui::Label> header;
    std::shared_ptr<ui::HBoxLayout> inputRow;
    std::shared_ptr<ui::Label> nameLabel;
    std::shared_ptr<ui::TextBox> nameField;
    std::shared_ptr<ui::GridLayout> buttonGrid;
    std::shared_ptr<ui::Button> saveBtn;
    std::shared_ptr<ui::Button> loadBtn;
    std::shared_ptr<ui::Button> clearBtn;
    std::shared_ptr<ui::CheckBox> autoSave;
    std::shared_ptr<ui::Label> footer;
};

inline AdvancedWidgets createAdvanced(UI& ui) {
    AdvancedWidgets widgets;
    
    try {
        // Create layouts first
        widgets.mainLayout = std::make_shared<ui::VBoxLayout>();
        widgets.inputRow = std::make_shared<ui::HBoxLayout>();
        widgets.buttonGrid = std::make_shared<ui::GridLayout>(2, 2);
        
        // Create widgets
        widgets.header = ui.createLabel("Advanced DSL Demo", 10, 10);
        widgets.nameLabel = ui.createLabel("Name:", 10, 10);
        widgets.nameField = ui.createTextBox("Enter name", 10, 10);
        widgets.saveBtn = ui.createButton("Save", 10, 10, nullptr);
        widgets.loadBtn = ui.createButton("Load", 10, 10, nullptr);
        widgets.clearBtn = ui.createButton("Clear", 10, 10, nullptr);
        widgets.autoSave = ui.createCheckBox(true, 10, 10, nullptr);
        widgets.footer = ui.createLabel("Status: Ready", 10, 10);
        
        // Setup layout hierarchy
        widgets.mainLayout->addItem(widgets.header);
        widgets.inputRow->addItem(widgets.nameLabel);
        widgets.inputRow->addItem(widgets.nameField);
        widgets.buttonGrid->addItem(widgets.saveBtn, 0, 0);
        widgets.buttonGrid->addItem(widgets.loadBtn, 0, 1);
        widgets.buttonGrid->addItem(widgets.clearBtn, 1, 0);
        widgets.buttonGrid->addItem(widgets.autoSave, 1, 1);
        widgets.mainLayout->addItem(widgets.footer);
    } catch (const std::exception& e) {
        throw ui::UIException("Failed to create advanced widgets: " + std::string(e.what()));
    }
    
    return widgets;
}

} // namespace ui_generated
