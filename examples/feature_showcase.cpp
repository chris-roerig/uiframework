#include "uiframework/UI.h"
#include <iostream>

int main() {
    try {
        UI ui("UI Framework Showcase - All New Features", 1000, 700);
        
        // === Phase 1.4: Consistent Sizing API ===
        auto titleLabel = ui.createLabel("UI Framework Feature Showcase", 20, 20);
        // Note: autoSize is called automatically by createLabel
        titleLabel->setTooltip("Auto-sized title using consistent sizing API");
        
        // === Phase 1.5: Enabled/Disabled State ===
        auto enabledButton = ui.createButton("Enabled Button", 20, 60, []() {
            std::cout << "Enabled button clicked!" << std::endl;
        });
        enabledButton->setTooltip("This button is enabled and clickable");
        
        auto disabledButton = ui.createButton("Disabled Button", 180, 60, []() {
            std::cout << "This shouldn't print!" << std::endl;
        });
        disabledButton->setEnabled(false);
        disabledButton->setTooltip("This button is disabled - notice the grayed appearance");
        
        // === Phase 2.3: Margin/Padding Support ===
        auto noPaddingButton = ui.createButton("No Padding", 20, 120, []() {
            std::cout << "No padding button" << std::endl;
        });
        noPaddingButton->setTooltip("Default button with no padding");
        
        auto paddedButton = ui.createButton("With Padding", 180, 120, []() {
            std::cout << "Padded button" << std::endl;
        });
        paddedButton->setPadding(12);
        paddedButton->setTooltip("Button with 12px padding - notice text positioning");
        
        auto customPaddingButton = ui.createButton("Custom Padding", 360, 120, []() {
            std::cout << "Custom padding button" << std::endl;
        });
        customPaddingButton->setPadding(4, 20, 4, 20); // top, right, bottom, left
        customPaddingButton->setTooltip("Custom padding: 4px top/bottom, 20px left/right");
        
        // === Phase 2.2: Comprehensive Tooltip System ===
        auto tooltipLabel = ui.createLabel("Hover elements for tooltips →", 20, 180);
        tooltipLabel->setTooltip("This label demonstrates the tooltip system!\n• 500ms hover delay\n• Smart positioning\n• Theme integration");
        
        // === Text Elements with Spacing ===
        auto normalTextBox = ui.createTextBox("Normal textbox", 20, 220, 150);
        normalTextBox->setTooltip("Standard textbox with no padding");
        
        auto paddedTextBox = ui.createTextBox("Padded textbox", 200, 220, 150);
        paddedTextBox->setPadding(8);
        paddedTextBox->setTooltip("Textbox with 8px padding - text positioned inside padding");
        
        auto disabledTextBox = ui.createTextBox("Disabled textbox", 380, 220, 150);
        disabledTextBox->setEnabled(false);
        disabledTextBox->setTooltip("Disabled textbox - no input allowed, grayed appearance");
        
        // === Labels with Different Spacing ===
        auto label1 = ui.createLabel("No padding", 20, 280);
        label1->setTooltip("Label with default spacing");
        
        auto label2 = ui.createLabel("Small padding", 150, 280);
        label2->setPadding(4);
        label2->setTooltip("Label with 4px padding");
        
        auto label3 = ui.createLabel("Large padding", 280, 280);
        label3->setPadding(12);
        label3->setTooltip("Label with 12px padding");
        
        auto label4 = ui.createLabel("Asymmetric", 420, 280);
        label4->setPadding(2, 16, 2, 16);
        label4->setTooltip("Asymmetric padding: 2px top/bottom, 16px left/right");
        
        // === Interactive Elements Showcase ===
        auto checkbox1 = ui.createCheckBox(false, 20, 320, [](bool checked) {
            std::cout << "Checkbox 1 " << (checked ? "checked" : "unchecked") << std::endl;
        });
        checkbox1->setTooltip("Enabled checkbox - click to toggle");
        
        auto checkbox2 = ui.createCheckBox(false, 200, 320, [](bool checked) {
            std::cout << "This shouldn't print!" << std::endl;
        });
        checkbox2->setEnabled(false);
        checkbox2->setTooltip("Disabled checkbox - cannot be toggled");
        
        // === Theme Integration Demo ===
        auto themeLabel = ui.createLabel("Theme Integration:", 20, 360);
        themeLabel->setTooltip("All tooltips use theme colors automatically");
        
        auto themeButton1 = ui.createButton("Framework Theme", 20, 390, [&ui]() {
            ui.setTheme("framework");
            std::cout << "Switched to Framework theme" << std::endl;
        });
        themeButton1->setTooltip("Switch to Framework (dark) theme");
        
        auto themeButton2 = ui.createButton("Solarized Light", 180, 390, [&ui]() {
            ui.setTheme("solarized_light");
            std::cout << "Switched to Solarized Light theme" << std::endl;
        });
        themeButton2->setTooltip("Switch to Solarized Light theme");
        
        auto themeButton3 = ui.createButton("Solarized Dark", 340, 390, [&ui]() {
            ui.setTheme("solarized_dark");
            std::cout << "Switched to Solarized Dark theme" << std::endl;
        });
        themeButton3->setTooltip("Switch to Solarized Dark theme");
        
        auto themeButton4 = ui.createButton("Molokai", 500, 390, [&ui]() {
            ui.setTheme("molokai");
            std::cout << "Switched to Molokai theme" << std::endl;
        });
        themeButton4->setTooltip("Switch to Molokai theme");
        
        // === Feature Summary ===
        auto summaryLabel = ui.createLabel("New Features Demonstrated:", 20, 450);
        summaryLabel->setTooltip("Summary of all implemented features");
        
        auto feature1 = ui.createLabel("✓ Consistent Sizing API (auto-sizing)", 40, 480);
        feature1->setTooltip("Phase 1.4: getPreferredSize(), autoSize(), getMinimumSize()");
        
        auto feature2 = ui.createLabel("✓ Enabled/Disabled State Management", 40, 500);
        feature2->setTooltip("Phase 1.5: setEnabled(), visual disabled states");
        
        auto feature3 = ui.createLabel("✓ Comprehensive Tooltip System", 40, 520);
        feature3->setTooltip("Phase 2.2: Smart positioning, theme integration, hover delay");
        
        auto feature4 = ui.createLabel("✓ Margin/Padding Support", 40, 540);
        feature4->setTooltip("Phase 2.3: setPadding(), setMargin(), getContentRect()");
        
        auto feature5 = ui.createLabel("✓ Unified Rendering & Event Handling", 40, 560);
        feature5->setTooltip("Phase 1.1-1.2: RenderContext, InteractiveElement base classes");
        
        auto feature6 = ui.createLabel("✓ Reusable Utilities (Text, Borders)", 40, 580);
        feature6->setTooltip("Phase 1.3, 2.1: TextUtils, BorderRenderer - 347+ lines eliminated");
        
        // === Instructions ===
        auto instructionLabel = ui.createLabel("Instructions:", 550, 450);
        instructionLabel->setTooltip("How to use this demo");
        
        auto inst1 = ui.createLabel("• Hover over any element for tooltips", 570, 480);
        inst1->setTooltip("Tooltips appear after 500ms delay");
        
        auto inst2 = ui.createLabel("• Click buttons to test functionality", 570, 500);
        inst2->setTooltip("Enabled buttons respond, disabled ones don't");
        
        auto inst3 = ui.createLabel("• Try different themes", 570, 520);
        inst3->setTooltip("Notice how tooltips change colors with themes");
        
        auto inst4 = ui.createLabel("• Compare padding effects", 570, 540);
        inst4->setTooltip("See how text positioning changes with padding");
        
        auto inst5 = ui.createLabel("• Test disabled elements", 570, 560);
        inst5->setTooltip("Disabled elements show grayed appearance");
        
        std::cout << "=== UI Framework Feature Showcase ===" << std::endl;
        std::cout << "This demo showcases all new features from Phases 1-2:" << std::endl;
        std::cout << "• Consistent Sizing API (Phase 1.4)" << std::endl;
        std::cout << "• Enabled/Disabled States (Phase 1.5)" << std::endl;
        std::cout << "• Comprehensive Tooltips (Phase 2.2)" << std::endl;
        std::cout << "• Margin/Padding Support (Phase 2.3)" << std::endl;
        std::cout << "• Unified Architecture (Phase 1.1-1.2, 2.1)" << std::endl;
        std::cout << "• 347+ lines of duplicated code eliminated" << std::endl;
        std::cout << "=====================================" << std::endl;
        
        ui.run();
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    
    return 0;
}
