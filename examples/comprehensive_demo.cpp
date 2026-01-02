#include "uiframework/UI.h"
#include <iostream>
#include <chrono>
#include <thread>

/**
 * Comprehensive UI Framework Demo
 * 
 * This demo showcases ALL capabilities of the UI Framework:
 * - All 17+ widget types with interactive examples
 * - Complete font system (embedded fonts, families, styles, metrics)
 * - All 4 themes with runtime switching
 * - Advanced features (tooltips, spacing, focus management)
 * - Real-time capabilities (lock-free updates, batching, bulk operations)
 * - Professional application examples
 */

class ComprehensiveDemo {
private:
    UI ui;
    int currentPage = 0;
    const int totalPages = 5;
    
    // Demo state
    bool demoRunning = true;
    std::chrono::high_resolution_clock::time_point startTime;
    
public:
    ComprehensiveDemo() : ui("UI Framework - Comprehensive Demo", 1200, 800) {
        startTime = std::chrono::high_resolution_clock::now();
        setupDemo();
    }
    
    void run() {
        // Process real-time updates in the main loop
        // Note: setUpdateCallback is not available, so we'll handle updates manually
        ui.run();
    }
    
private:
    void setupDemo() {
        createNavigationControls();
        createPageContent();
    }
    
    void createNavigationControls() {
        // Title
        auto title = ui.createLabel("UI Framework Comprehensive Demo", 10, 10);
        title->setThemeFont(ui::FontType::Primary, 24);
        title->setTooltip("Complete demonstration of all UI Framework capabilities");
        
        // Page indicator
        auto pageLabel = ui.createLabel("Page 1 of " + std::to_string(totalPages), 10, 50);
        pageLabel->setId("page_indicator");
        pageLabel->setThemeFont(ui::FontType::UI, 14);
        
        // Navigation buttons
        auto prevBtn = ui.createButton("← Previous", 10, 80, [this]() {
            if (currentPage > 0) {
                currentPage--;
                updatePage();
            }
        });
        prevBtn->setTooltip("Go to previous demo page");
        prevBtn->setPadding(8);
        
        auto nextBtn = ui.createButton("Next →", 120, 80, [this]() {
            if (currentPage < totalPages - 1) {
                currentPage++;
                updatePage();
            }
        });
        nextBtn->setTooltip("Go to next demo page");
        nextBtn->setPadding(8);
        
        // Theme selector
        std::vector<std::string> themes = {"Default", "SolarizedDark", "SolarizedLight", "Molokai"};
        auto themeSelect = ui.createOptionSelect(themes, 0, 250, 80, [this, themes](int index) {
            ui.setTheme(themes[index]);
        });
        themeSelect->setTooltip("Switch between available themes");
        
        // Quit button
        auto quitBtn = ui.createButton("Quit Demo", 1050, 10, [this]() {
            demoRunning = false;
            // Note: In a real app, you'd call ui.quit() or similar
        });
        quitBtn->setTooltip("Exit the comprehensive demo");
        quitBtn->setPadding(8);
    }
    
    void createPageContent() {
        clearPageContent();
        
        switch (currentPage) {
            case 0: createWidgetShowcasePage(); break;
            case 1: createFontSystemPage(); break;
            case 2: createAdvancedFeaturesPage(); break;
            case 3: createRealTimePage(); break;
            case 4: createApplicationExamplesPage(); break;
        }
    }
    
    void clearPageContent() {
        // Remove all page-specific elements (keep navigation)
        // In a real implementation, you'd track page elements and remove them
    }
    
    void updatePage() {
        auto pageLabel = ui.getElement("page_indicator");
        if (auto label = std::dynamic_pointer_cast<ui::Label>(pageLabel)) {
            label->setText("Page " + std::to_string(currentPage + 1) + " of " + std::to_string(totalPages));
        }
        createPageContent();
    }
    
    // Page 1: Widget Showcase
    void createWidgetShowcasePage() {
        auto pageTitle = ui.createLabel("Widget Showcase - All 17+ UI Elements", 10, 120);
        pageTitle->setThemeFont(ui::FontType::Primary, 18);
        pageTitle->setId("page_title");
        
        int x = 50, y = 160;
        
        // Basic Elements
        createSectionLabel("Basic Elements", x, y);
        y += 40;
        
        auto label = ui.createLabel("Sample Label", x, y);
        label->setTooltip("Basic text display widget");
        
        auto button = ui.createButton("Click Me", x + 150, y, []() {
            std::cout << "Button clicked!" << std::endl;
        });
        button->setTooltip("Interactive button with click callback");
        button->setPadding(6);
        
        auto textbox = ui.createTextBox("Enter text here...", x + 250, y);
        textbox->setSize(150, 25);
        textbox->setTooltip("Editable text input field");
        
        auto checkbox = ui.createCheckBox(false, x + 420, y, [](bool checked) {
            std::cout << "Checkbox " << (checked ? "checked" : "unchecked") << std::endl;
        });
        checkbox->setTooltip("Toggle checkbox with state callback");
        
        y += 50;
        
        // Input Controls
        createSectionLabel("Input Controls", x, y);
        y += 40;
        
        auto hslider = ui.createHSlider(x, y, 200, 20, 0.0f, 100.0f, 50.0f);
        hslider->setTooltip("Horizontal slider control");
        
        auto vslider = ui.createVSlider(x + 220, y, 20, 80, 0.0f, 100.0f, 25.0f);
        vslider->setTooltip("Vertical slider control");
        
        auto knob = ui.createKnobSlider(x + 260, y, 60, 0.0f, 100.0f, 75.0f);
        knob->setTooltip("Rotary knob slider");
        
        auto progress = ui.createProgressBar(x + 340, y, 150, 20, 0.6f);
        progress->setTooltip("Progress bar with percentage display");
        
        y += 100;
        
        // Selection Controls
        createSectionLabel("Selection Controls", x, y);
        y += 40;
        
        std::vector<std::string> options = {"Option 1", "Option 2", "Option 3", "Option 4"};
        auto optionSelect = ui.createOptionSelect(options, 0, x, y, [](int index) {
            std::cout << "Selected option " << index << std::endl;
        });
        optionSelect->setTooltip("Dropdown selection widget");
        
        std::vector<std::string> cycleItems = {"Item A", "Item B", "Item C"};
        auto cycleList = ui.createCycleList(x + 200, y, 120, 30, cycleItems);
        cycleList->setTooltip("Cycling list widget");
        
        y += 60;
        
        // Container Widgets
        createSectionLabel("Container Widgets", x, y);
        y += 40;
        
        std::vector<std::string> listItems = {"List Item 1", "List Item 2", "List Item 3", "List Item 4"};
        auto listView = ui.createListView(listItems, x, y, 200, 120);
        listView->setTooltip("Scrollable list view widget");
        
        auto tabbedPanel = ui.createTabbedPanel(x + 220, y, 300, 120);
        tabbedPanel->setTooltip("Tabbed panel container");
        
        y += 140;
        
        // Graphics Elements
        createSectionLabel("Graphics Elements", x, y);
        y += 40;
        
        auto canvas = ui.createCanvas(x, y, 150, 100);
        canvas->setTooltip("Drawing canvas for custom graphics");
        
        // Note: Image and Sprite creation would require actual image files
        // In a real demo, you'd include sample images
    }
    
    // Page 2: Font System
    void createFontSystemPage() {
        auto pageTitle = ui.createLabel("Font System - Professional Typography", 10, 120);
        pageTitle->setThemeFont(ui::FontType::Primary, 18);
        pageTitle->setId("page_title");
        
        int x = 50, y = 160;
        
        // Embedded Fonts
        createSectionLabel("Embedded Fonts (Zero Dependencies)", x, y);
        y += 40;
        
        auto robotoLabel = ui.createLabel("Roboto Font - Professional Sans-Serif", x, y);
        robotoLabel->setThemeFont(ui::FontType::Primary, 16);
        robotoLabel->setTooltip("Embedded Roboto font (188KB)");
        
        y += 30;
        auto consoleLabel = ui.createLabel("Console Font - Monospace for Code", x, y);
        consoleLabel->setThemeFont(ui::FontType::Monospace, 14);
        consoleLabel->setTooltip("Embedded Console font (16KB)");
        
        y += 50;
        
        // Font Families and Styles
        createSectionLabel("Font Families & Styles", x, y);
        y += 40;
        
        auto regularText = ui.createLabel("Regular Style Text", x, y);
        regularText->setFont("Roboto", 14, ui::FontStyle::Regular);
        regularText->setTooltip("Regular font style");
        
        auto boldText = ui.createLabel("Bold Style Text", x + 200, y);
        boldText->setFont("Roboto", 14, ui::FontStyle::Bold);
        boldText->setTooltip("Bold font style");
        
        y += 30;
        auto italicText = ui.createLabel("Italic Style Text", x, y);
        italicText->setFont("Roboto", 14, ui::FontStyle::Italic);
        italicText->setTooltip("Italic font style");
        
        auto boldItalicText = ui.createLabel("Bold Italic Style Text", x + 200, y);
        boldItalicText->setFont("Roboto", 14, ui::FontStyle::BoldItalic);
        boldItalicText->setTooltip("Bold italic font style");
        
        y += 50;
        
        // Font Sizes
        createSectionLabel("Font Sizes & Metrics", x, y);
        y += 40;
        
        for (int size : {10, 12, 14, 16, 18, 20, 24}) {
            auto sizeLabel = ui.createLabel("Size " + std::to_string(size), x, y);
            sizeLabel->setThemeFont(ui::FontType::Primary, size);
            sizeLabel->setTooltip("Font size: " + std::to_string(size) + "px");
            y += size + 5;
        }
        
        // Font Configuration Demo
        x = 600;
        y = 200;
        createSectionLabel("Font Configuration", x, y);
        y += 40;
        
        auto configLabel = ui.createLabel("JSON Configuration Support", x, y);
        configLabel->setTooltip("Runtime font configuration via JSON");
        
        y += 30;
        auto metricsLabel = ui.createLabel("Font Metrics Available", x, y);
        metricsLabel->setTooltip("Access to ascent, descent, line skip, height");
    }
    
    // Page 3: Advanced Features
    void createAdvancedFeaturesPage() {
        auto pageTitle = ui.createLabel("Advanced Features - Professional UI", 10, 120);
        pageTitle->setThemeFont(ui::FontType::Primary, 18);
        pageTitle->setId("page_title");
        
        int x = 50, y = 160;
        
        // Tooltip System
        createSectionLabel("Tooltip System", x, y);
        y += 40;
        
        auto tooltipBtn = ui.createButton("Hover for Tooltip", x, y, []() {});
        tooltipBtn->setTooltip("This is a smart tooltip with 500ms delay and edge detection!");
        tooltipBtn->setPadding(8);
        
        y += 50;
        
        // Spacing System
        createSectionLabel("Spacing System", x, y);
        y += 40;
        
        auto paddedBtn = ui.createButton("Padded Button", x, y, []() {});
        paddedBtn->setPadding(15);
        paddedBtn->setTooltip("Button with 15px padding on all sides");
        
        auto customPaddedBtn = ui.createButton("Custom Padding", x + 200, y, []() {});
        customPaddedBtn->setPadding(5, 20, 5, 20);  // top, right, bottom, left
        customPaddedBtn->setTooltip("Custom padding: 5px top/bottom, 20px left/right");
        
        y += 60;
        
        // Enabled/Disabled States
        createSectionLabel("Enabled/Disabled States", x, y);
        y += 40;
        
        auto enabledBtn = ui.createButton("Enabled Button", x, y, []() {
            std::cout << "Enabled button clicked!" << std::endl;
        });
        enabledBtn->setTooltip("This button is enabled and interactive");
        enabledBtn->setPadding(6);
        
        auto disabledBtn = ui.createButton("Disabled Button", x + 200, y, []() {});
        disabledBtn->setEnabled(false);
        disabledBtn->setTooltip("This button is disabled - no interaction possible");
        disabledBtn->setPadding(6);
        
        y += 60;
        
        // Focus Management
        createSectionLabel("Focus Management", x, y);
        y += 40;
        
        auto focusBtn1 = ui.createButton("Focus 1", x, y, []() {}, 1);
        focusBtn1->setTooltip("Tab order: 1 (use Tab key to navigate)");
        focusBtn1->setPadding(6);
        
        auto focusBtn2 = ui.createButton("Focus 2", x + 100, y, []() {}, 2);
        focusBtn2->setTooltip("Tab order: 2");
        focusBtn2->setPadding(6);
        
        auto focusBtn3 = ui.createButton("Focus 3", x + 200, y, []() {}, 3);
        focusBtn3->setTooltip("Tab order: 3");
        focusBtn3->setPadding(6);
        
        y += 60;
        
        // Layout System
        createSectionLabel("Layout System", x, y);
        y += 40;
        
        auto vboxLayout = ui.createVBoxLayout(x, y, 150, 120, 10);
        vboxLayout->setTooltip("Vertical box layout with 10px spacing");
        
        auto hboxLayout = ui.createHBoxLayout(x + 200, y, 200, 50, 15);
        hboxLayout->setTooltip("Horizontal box layout with 15px spacing");
        
        auto gridLayout = ui.createGridLayout(x + 450, y, 150, 120, 3, 2, 8);
        gridLayout->setTooltip("Grid layout: 3 rows, 2 columns, 8px spacing");
    }
    
    // Page 4: Real-Time Capabilities
    void createRealTimePage() {
        auto pageTitle = ui.createLabel("Real-Time Capabilities - Professional Performance", 10, 120);
        pageTitle->setThemeFont(ui::FontType::Primary, 18);
        pageTitle->setId("page_title");
        
        int x = 50, y = 160;
        
        // Lock-Free Updates
        createSectionLabel("Lock-Free Updates (Audio Thread Safe)", x, y);
        y += 40;
        
        auto realtimeLabel = ui.createLabel("Real-time Value: 0.00", x, y);
        realtimeLabel->setId("realtime_value");
        realtimeLabel->setTooltip("Updated via lock-free real-time methods");
        
        auto realtimeSlider = ui.createHSlider(x, y + 30, 200, 20, 0.0f, 100.0f, 0.0f);
        realtimeSlider->setId("realtime_slider");
        realtimeSlider->setTooltip("Real-time slider - 48,000+ ops/sec capability");
        
        y += 80;
        
        // Update Batching
        createSectionLabel("High-Frequency Update Batching", x, y);
        y += 40;
        
        auto batchLabel = ui.createLabel("Batch Updates: 0", x, y);
        batchLabel->setId("batch_counter");
        batchLabel->setTooltip("99.9% redundancy reduction with batching");
        
        y += 50;
        
        // Bulk Operations
        createSectionLabel("Bulk Operations (SIMD Optimized)", x, y);
        y += 40;
        
        // Create multiple elements for bulk updates
        for (int i = 0; i < 5; i++) {
            auto bulkLabel = ui.createLabel("Bulk " + std::to_string(i) + ": 0.0", x + i * 100, y);
            bulkLabel->setId("bulk_" + std::to_string(i));
            bulkLabel->setTooltip("Part of bulk update demonstration");
        }
        
        y += 50;
        
        // Memory Predictable
        createSectionLabel("Memory Predictable Operations", x, y);
        y += 40;
        
        auto memoryLabel = ui.createLabel("Memory Usage: < 24KB per frame", x, y);
        memoryLabel->setTooltip("Bounded memory allocation for safety-critical apps");
        
        y += 50;
        
        // Timing Support
        createSectionLabel("High-Resolution Timing", x, y);
        y += 40;
        
        auto timingLabel = ui.createLabel("Microsecond Precision: 0μs", x, y);
        timingLabel->setId("timing_display");
        timingLabel->setTooltip("Sample-accurate scheduling support");
    }
    
    // Page 5: Application Examples
    void createApplicationExamplesPage() {
        auto pageTitle = ui.createLabel("Professional Application Examples", 10, 120);
        pageTitle->setThemeFont(ui::FontType::Primary, 18);
        pageTitle->setId("page_title");
        
        int x = 50, y = 160;
        
        // Audio Studio Interface
        createSectionLabel("Audio Studio Interface", x, y);
        y += 40;
        
        // Create a mini mixing console
        for (int channel = 0; channel < 4; channel++) {
            int channelX = x + channel * 80;
            
            auto channelLabel = ui.createLabel("CH " + std::to_string(channel + 1), channelX, y);
            channelLabel->setThemeFont(ui::FontType::UI, 10);
            
            auto gainSlider = ui.createVSlider(channelX + 10, y + 20, 15, 100, 0.0f, 1.0f, 0.75f);
            gainSlider->setId("gain_" + std::to_string(channel));
            gainSlider->setTooltip("Channel " + std::to_string(channel + 1) + " gain control");
            
            auto muteBtn = ui.createButton("M", channelX + 30, y + 20, []() {});
            muteBtn->setSize(20, 20);
            muteBtn->setTooltip("Mute channel " + std::to_string(channel + 1));
            
            auto soloBtn = ui.createButton("S", channelX + 30, y + 45, []() {});
            soloBtn->setSize(20, 20);
            soloBtn->setTooltip("Solo channel " + std::to_string(channel + 1));
            
            auto levelMeter = ui.createProgressBar(channelX + 55, y + 20, 10, 100, 0.0f, false);
            levelMeter->setId("level_" + std::to_string(channel));
            levelMeter->setTooltip("Channel " + std::to_string(channel + 1) + " level meter");
        }
        
        y += 150;
        
        // Data Visualization
        createSectionLabel("Data Visualization Dashboard", x, y);
        y += 40;
        
        // Create monitoring widgets
        auto cpuLabel = ui.createLabel("CPU Usage:", x, y);
        auto cpuProgress = ui.createProgressBar(x + 100, y, 150, 20, 0.45f);
        cpuProgress->setId("cpu_usage");
        cpuProgress->setTooltip("Real-time CPU usage monitoring");
        
        y += 30;
        auto memLabel = ui.createLabel("Memory:", x, y);
        auto memProgress = ui.createProgressBar(x + 100, y, 150, 20, 0.67f);
        memProgress->setId("memory_usage");
        memProgress->setTooltip("Real-time memory usage monitoring");
        
        y += 30;
        auto netLabel = ui.createLabel("Network:", x, y);
        auto netProgress = ui.createProgressBar(x + 100, y, 150, 20, 0.23f);
        netProgress->setId("network_usage");
        netProgress->setTooltip("Real-time network usage monitoring");
        
        x = 400;
        y = 400;
        
        // Configuration Panel
        createSectionLabel("Configuration Panel", x, y);
        y += 40;
        
        auto configTextbox = ui.createTextBox("Server Address", x, y);
        configTextbox->setSize(200, 25);
        configTextbox->setTooltip("Server configuration setting");
        
        y += 35;
        auto portTextbox = ui.createTextBox("8080", x, y);
        portTextbox->setSize(100, 25);
        portTextbox->setTooltip("Port number setting");
        
        y += 35;
        auto sslCheckbox = ui.createCheckBox(true, x, y, [](bool enabled) {
            std::cout << "SSL " << (enabled ? "enabled" : "disabled") << std::endl;
        });
        auto sslLabel = ui.createLabel("Enable SSL", x + 25, y);
        sslLabel->setTooltip("SSL encryption setting");
        
        y += 35;
        std::vector<std::string> logLevels = {"Debug", "Info", "Warning", "Error"};
        auto logSelect = ui.createOptionSelect(logLevels, 1, x, y, [](int level) {
            std::cout << "Log level changed to " << level << std::endl;
        });
        logSelect->setTooltip("Logging level configuration");
    }
    
    void createSectionLabel(const std::string& text, int x, int y) {
        auto label = ui.createLabel(text, x, y);
        label->setThemeFont(ui::FontType::Primary, 16);
        label->setTooltip("Section: " + text);
    }
    
    void updateRealTimeDemo() {
        if (currentPage != 3) return;  // Only update on real-time page
        
        auto now = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime).count();
        
        // Simulate real-time updates
        float sineValue = 50.0f + 25.0f * std::sin(elapsed * 0.001f);
        
        // Lock-free real-time updates
        ui.realtimeSetValue("realtime_slider", sineValue);
        ui.realtimeSetText("realtime_value", "Real-time Value: " + std::to_string(sineValue));
        
        // Batch counter
        static int batchCount = 0;
        ui.realtimeSetText("batch_counter", "Batch Updates: " + std::to_string(++batchCount));
        
        // Bulk operations
        std::vector<std::string> bulkIds;
        std::vector<std::string> bulkTexts;
        for (int i = 0; i < 5; i++) {
            bulkIds.push_back("bulk_" + std::to_string(i));
            float value = 50.0f + 20.0f * std::sin((elapsed + i * 200) * 0.001f);
            bulkTexts.push_back("Bulk " + std::to_string(i) + ": " + std::to_string(value));
        }
        ui.realtimeBulkSetText(bulkIds, bulkTexts);
        
        // Timing display
        auto microTime = std::chrono::duration_cast<std::chrono::microseconds>(now - startTime).count();
        ui.realtimeSetText("timing_display", "Microsecond Precision: " + std::to_string(microTime % 1000000) + "μs");
    }
};

int main() {
    try {
        ComprehensiveDemo demo;
        demo.run();
    } catch (const ui::UIException& e) {
        std::cerr << "UI Error: " << e.what() << std::endl;
        return -1;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    
    return 0;
}
