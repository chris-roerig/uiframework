// Generated from dev/examples/table_grid.ui
// DO NOT EDIT - This file is auto-generated

#pragma once
#include "uiframework/UI.h"
#include "uiframework/Layout/VBoxLayout.h"
#include "uiframework/Layout/HBoxLayout.h"
#include "uiframework/Layout/GridLayout.h"
#include "uiframework/UIElements/LayoutContainer.h"
#include <memory>

namespace ui_generated {

struct Table_GridWidgets {
    std::shared_ptr<ui::GridLayout> tableGrid;
    std::shared_ptr<ui::Label> header1;
    std::shared_ptr<ui::Label> header2;
    std::shared_ptr<ui::Label> header3;
    std::shared_ptr<ui::TextBox> nameInput;
    std::shared_ptr<ui::Button> saveBtn;
    std::shared_ptr<ui::CheckBox> enabledCheck;
    std::shared_ptr<ui::Label> footer1;
    std::shared_ptr<ui::Button> clearBtn;
    std::shared_ptr<ui::Label> status;
};

inline Table_GridWidgets createTable_Grid(UI& ui) {
    Table_GridWidgets widgets;
    
    try {
        // Create layouts first
        widgets.tableGrid = std::make_shared<ui::GridLayout>(3, 3);
        
        // Create widgets
        widgets.header1 = ui.createLabel("Name", 10, 10);
        widgets.header2 = ui.createLabel("Action", 10, 10);
        widgets.header3 = ui.createLabel("Status", 10, 10);
        widgets.nameInput = ui.createTextBox("Enter name", 10, 10);
        widgets.saveBtn = ui.createButton("Save", 10, 10, nullptr);
        widgets.enabledCheck = ui.createCheckBox(true, 10, 10, nullptr);
        widgets.footer1 = ui.createLabel("Total: 1", 10, 10);
        widgets.clearBtn = ui.createButton("Clear All", 10, 10, nullptr);
        widgets.status = ui.createLabel("Ready", 10, 10);
        
        // Setup layout hierarchy
        widgets.tableGrid->addItem(widgets.header1, 0, 0);
        widgets.tableGrid->addItem(widgets.header2, 0, 1);
        widgets.tableGrid->addItem(widgets.header3, 0, 2);
        widgets.tableGrid->addItem(widgets.nameInput, 1, 0);
        widgets.tableGrid->addItem(widgets.saveBtn, 1, 1);
        widgets.tableGrid->addItem(widgets.enabledCheck, 1, 2);
        widgets.tableGrid->addItem(widgets.footer1, 2, 0);
        widgets.tableGrid->addItem(widgets.clearBtn, 2, 1);
        widgets.tableGrid->addItem(widgets.status, 2, 2);
    } catch (const std::exception& e) {
        throw ui::UIException("Failed to create table_grid widgets: " + std::string(e.what()));
    }
    
    return widgets;
}

} // namespace ui_generated
