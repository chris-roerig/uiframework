// Generated from dev/examples/grid_test.ui
// DO NOT EDIT - This file is auto-generated

#pragma once
#include "uiframework/UI.h"
#include "uiframework/Layout/VBoxLayout.h"
#include "uiframework/Layout/HBoxLayout.h"
#include "uiframework/Layout/GridLayout.h"
#include "uiframework/UIElements/LayoutContainer.h"
#include <memory>

namespace ui_generated {

struct Grid_TestWidgets {
    std::shared_ptr<ui::GridLayout> mainGrid;
    std::shared_ptr<ui::Button> btn1;
    std::shared_ptr<ui::Button> btn2;
    std::shared_ptr<ui::Label> label1;
    std::shared_ptr<ui::CheckBox> check1;
};

inline Grid_TestWidgets createGrid_Test(UI& ui) {
    Grid_TestWidgets widgets;
    
    try {
        // Create layouts first
        widgets.mainGrid = std::make_shared<ui::GridLayout>(2, 2);
        
        // Create widgets
        widgets.btn1 = ui.createButton("Button 1", 10, 10, nullptr);
        widgets.btn2 = ui.createButton("Button 2", 10, 10, nullptr);
        widgets.label1 = ui.createLabel("Label 1", 10, 10);
        widgets.check1 = ui.createCheckBox(true, 10, 10, nullptr);
        
        // Setup layout hierarchy
        widgets.mainGrid->addItem(widgets.btn1, 0, 0);
        widgets.mainGrid->addItem(widgets.btn2, 0, 1);
        widgets.mainGrid->addItem(widgets.label1, 1, 0);
        widgets.mainGrid->addItem(widgets.check1, 1, 1);
    } catch (const std::exception& e) {
        throw ui::UIException("Failed to create grid_test widgets: " + std::string(e.what()));
    }
    
    return widgets;
}

} // namespace ui_generated
