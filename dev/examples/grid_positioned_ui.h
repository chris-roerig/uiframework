// Generated from dev/examples/grid_positioned.ui
// DO NOT EDIT - This file is auto-generated

#pragma once
#include "uiframework/UI.h"
#include "uiframework/Layout/VBoxLayout.h"
#include "uiframework/Layout/HBoxLayout.h"
#include "uiframework/Layout/GridLayout.h"
#include "uiframework/UIElements/LayoutContainer.h"
#include <memory>

namespace ui_generated {

struct Grid_PositionedWidgets {
    std::shared_ptr<ui::GridLayout> mainGrid;
    std::shared_ptr<ui::Button> btn1;
    std::shared_ptr<ui::Button> btn2;
    std::shared_ptr<ui::Label> label1;
    std::shared_ptr<ui::CheckBox> check1;
    std::shared_ptr<ui::Button> btn3;
};

inline Grid_PositionedWidgets createGrid_Positioned(UI& ui) {
    Grid_PositionedWidgets widgets;
    
    try {
        // Create layouts first
        widgets.mainGrid = std::make_shared<ui::GridLayout>(3, 3);
        
        // Create widgets
        widgets.btn1 = ui.createButton("Top Left", 10, 10, nullptr);
        widgets.btn2 = ui.createButton("Top Right", 10, 10, nullptr);
        widgets.label1 = ui.createLabel("Middle Center", 10, 10);
        widgets.check1 = ui.createCheckBox(true, 10, 10, nullptr);
        widgets.btn3 = ui.createButton("Bottom Right", 10, 10, nullptr);
        
        // Setup layout hierarchy
        widgets.mainGrid->addItem(widgets.btn1, 0, 0);
        widgets.mainGrid->addItem(widgets.btn2, 0, 2);
        widgets.mainGrid->addItem(widgets.label1, 1, 1);
        widgets.mainGrid->addItem(widgets.check1, 2, 0);
        widgets.mainGrid->addItem(widgets.btn3, 2, 2);
    } catch (const std::exception& e) {
        throw ui::UIException("Failed to create grid_positioned widgets: " + std::string(e.what()));
    }
    
    return widgets;
}

} // namespace ui_generated
