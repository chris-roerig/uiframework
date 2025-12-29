// Generated from dev/examples/demo.ui
// DO NOT EDIT - This file is auto-generated

#pragma once
#include "uiframework/UI.h"
#include <memory>

namespace ui_generated {

struct DemoWidgets {
    std::shared_ptr<ui::Label> title;
    std::shared_ptr<ui::Button> clickButton;
    std::shared_ptr<ui::TextBox> nameInput;
    std::shared_ptr<ui::CheckBox> enableFeature;
    std::shared_ptr<ui::Label> status;
};

inline DemoWidgets createDemo(UI& ui) {
    DemoWidgets widgets;
    
    try {
        widgets.title = ui.createLabel("DSL Demo Application", 10, 10);
        widgets.clickButton = ui.createButton("Click Me!", 10, 50, nullptr);
        widgets.nameInput = ui.createTextBox("Enter your name", 10, 90);
        widgets.enableFeature = ui.createCheckBox(false, 10, 130, nullptr);
        widgets.status = ui.createLabel("Ready", 10, 170);
    } catch (const std::exception& e) {
        throw ui::UIException("Failed to create demo widgets: " + std::string(e.what()));
    }
    
    return widgets;
}

} // namespace ui_generated
