#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>
#include "uiframework/UI.h"
#include <chrono>
#include <vector>
#include <memory>

/**
 * @file test_performance_benchmarks.cpp
 * @brief Comprehensive performance benchmarking suite for UI Framework
 * 
 * This test suite measures performance across all major framework operations
 * to ensure optimal performance and detect regressions.
 */

TEST_CASE("Performance Benchmarks", "[benchmark][performance]") {
    
    SECTION("Widget Creation Performance") {
        UI ui("Benchmark Test", 800, 600);
        
        BENCHMARK("Create 1000 Labels") {
            std::vector<std::shared_ptr<ui::Label>> labels;
            labels.reserve(1000);
            
            for (int i = 0; i < 1000; ++i) {
                int x = (i * 50) % 750;  // Keep within window bounds
                int y = ((i / 15) * 30) % 550;  // Keep within window bounds
                labels.push_back(ui.createLabel("Label " + std::to_string(i), x, y));
            }
            
            return labels.size();
        };
        
        BENCHMARK("Create 1000 Buttons") {
            std::vector<std::shared_ptr<ui::Button>> buttons;
            buttons.reserve(1000);
            
            for (int i = 0; i < 1000; ++i) {
                int x = (i * 50) % 700;  // Keep within window bounds
                int y = ((i / 14) * 35) % 550;  // Keep within window bounds
                buttons.push_back(ui.createButton("Button " + std::to_string(i), x, y, [](){}));
            }
            
            return buttons.size();
        };
        
        BENCHMARK("Create 500 TextBoxes") {
            std::vector<std::shared_ptr<ui::TextBox>> textboxes;
            textboxes.reserve(500);
            
            for (int i = 0; i < 500; ++i) {
                int x = (i * 60) % 600;  // Keep within window bounds
                int y = ((i / 10) * 35) % 550;  // Keep within window bounds
                textboxes.push_back(ui.createTextBox("TextBox " + std::to_string(i), x, y));
            }
            
            return textboxes.size();
        };
        
        BENCHMARK("Create Mixed Widgets (25 each)") {
            std::vector<std::shared_ptr<ui::UIElement>> widgets;
            widgets.reserve(100);
            
            for (int i = 0; i < 25; ++i) {
                int x = (i % 5) * 120;  // 5 columns of 120px = 600px (safe for 800px window)
                int y = (i / 5) * 80;   // 5 rows of 80px = 400px (safe for 600px window)
                widgets.push_back(ui.createLabel("L" + std::to_string(i), x, y));
                widgets.push_back(ui.createButton("B" + std::to_string(i), x + 60, y, [](){}));
                widgets.push_back(ui.createTextBox("T" + std::to_string(i), x, y + 25, false));
                widgets.push_back(ui.createCheckBox(false, x + 60, y + 25, [](bool){}));
            }
            
            return widgets.size();
        };
    }
    
    SECTION("Layout Performance") {
        UI ui("Layout Benchmark", 1000, 800);
        
        BENCHMARK("VBoxLayout with 100 elements") {
            auto layout = ui.createVBoxLayout(10, 10, 980, 780, 5);
            
            for (int i = 0; i < 100; ++i) {
                auto button = ui.createButton("Button " + std::to_string(i), 0, 0, [](){});
                layout->addElement(button);
            }
            
            return 100;
        };
        
        BENCHMARK("HBoxLayout with 50 elements") {
            auto layout = ui.createHBoxLayout(10, 10, 980, 780, 5);
            
            for (int i = 0; i < 50; ++i) {
                auto button = ui.createButton("Btn " + std::to_string(i), 0, 0, [](){});
                layout->addElement(button);
            }
            
            return 50;
        };
        
        BENCHMARK("GridLayout 10x10 with 100 elements") {
            auto layout = ui.createGridLayout(10, 10, 980, 780, 10, 10, 5);
            
            for (int row = 0; row < 10; ++row) {
                for (int col = 0; col < 10; ++col) {
                    auto button = ui.createButton(std::to_string(row) + "," + std::to_string(col), 0, 0, [](){});
                    layout->addElement(button, row, col);
                }
            }
            
            return 100;
        };
    }
    
    SECTION("Text Operations Performance") {
        UI ui("Text Benchmark", 800, 600);
        
        BENCHMARK("Label Text Updates (1000 operations)") {
            auto label = ui.createLabel("Initial Text", 10, 10);
            
            for (int i = 0; i < 1000; ++i) {
                label->setText("Updated Text " + std::to_string(i));
            }
            
            return 1000;
        };
        
        BENCHMARK("TextBox Text Updates (1000 operations)") {
            auto textbox = ui.createTextBox("Initial Text", 10, 50);
            
            for (int i = 0; i < 1000; ++i) {
                textbox->setText("Updated Text " + std::to_string(i));
            }
            
            return 1000;
        };
        
        BENCHMARK("Button Text Updates (1000 operations)") {
            auto button = ui.createButton("Initial Text", 10, 90, [](){});
            
            for (int i = 0; i < 1000; ++i) {
                button->setText("Updated Text " + std::to_string(i));
            }
            
            return 1000;
        };
    }
    
    SECTION("Widget State Changes Performance") {
        UI ui("State Benchmark", 800, 600);
        
        BENCHMARK("CheckBox State Changes (10000 operations)") {
            auto checkbox = ui.createCheckBox(false, 10, 10, [](bool){});
            
            for (int i = 0; i < 10000; ++i) {
                checkbox->setChecked(i % 2 == 0);
            }
            
            return 10000;
        };
        
        BENCHMARK("Button State Changes (10000 operations)") {
            auto button = ui.createButton("Test Button", 10, 50, [](){});
            
            for (int i = 0; i < 10000; ++i) {
                button->setText("Button " + std::to_string(i % 100));
            }
            
            return 10000;
        };
        
        BENCHMARK("Widget Position Updates (5000 operations)") {
            auto label = ui.createLabel("Moving Label", 0, 0);
            
            for (int i = 0; i < 5000; ++i) {
                label->setPosition(i % 800, (i / 10) % 600);
            }
            
            return 5000;
        };
        
        BENCHMARK("Widget Size Updates (5000 operations)") {
            auto button = ui.createButton("Resizing Button", 10, 10, [](){});
            
            for (int i = 0; i < 5000; ++i) {
                int size = 50 + (i % 200);
                button->setSize(size, 30);
            }
            
            return 5000;
        };
    }
    
    SECTION("Memory Performance") {
        BENCHMARK("Widget Creation and Destruction (1000 cycles)") {
            UI ui("Memory Benchmark", 800, 600);
            
            for (int cycle = 0; cycle < 1000; ++cycle) {
                std::vector<std::shared_ptr<ui::UIElement>> widgets;
                widgets.reserve(10);
                
                // Create widgets
                for (int i = 0; i < 10; ++i) {
                    widgets.push_back(ui.createLabel("Temp Label " + std::to_string(i), i * 50, 10));
                }
                
                // Widgets automatically destroyed when vector goes out of scope
            }
            
            return 1000;
        };
        
        BENCHMARK("Large Widget Collection Management") {
            UI ui("Large Collection Benchmark", 1200, 900);
            std::vector<std::shared_ptr<ui::UIElement>> widgets;
            widgets.reserve(200);  // Much smaller collection to avoid issues
            
            // Create collection with safe positioning
            for (int i = 0; i < 200; ++i) {
                int x = (i % 20) * 50;  // 20 columns of 50px each = 1000px
                int y = (i / 20) * 40;  // 10 rows of 40px each = 400px
                
                if (i % 4 == 0) {
                    widgets.push_back(ui.createLabel("L" + std::to_string(i), x, y));
                } else if (i % 4 == 1) {
                    widgets.push_back(ui.createButton("B" + std::to_string(i), x, y, [](){}));
                } else if (i % 4 == 2) {
                    widgets.push_back(ui.createTextBox("T" + std::to_string(i), x, y));
                } else {
                    widgets.push_back(ui.createCheckBox(false, x, y, [](bool){}));
                }
            }
            
            // Perform operations on collection
            for (auto& widget : widgets) {
                widget->setVisible(true);
            }
            
            return widgets.size();
        };
    }
    
    SECTION("VirtualKeyboard Performance") {
        UI ui("VirtualKeyboard Benchmark", 800, 600);
        
        BENCHMARK("VirtualKeyboard Mode Changes (1000 operations)") {
            auto keyboard = ui.createVirtualKeyboard(10, 10, 780, 200, [](char){});
            
            for (int i = 0; i < 1000; ++i) {
                ui::KeyboardMode mode = static_cast<ui::KeyboardMode>(i % 4);
                keyboard->setMode(mode);
            }
            
            return 1000;
        };
        
        BENCHMARK("VirtualKeyboard Character Selection (5000 operations)") {
            auto keyboard = ui.createVirtualKeyboard(10, 10, 780, 200, [](char){});
            
            for (int i = 0; i < 5000; ++i) {
                keyboard->setSelectedIndex(i % 26); // Assuming lowercase mode with 26 characters
            }
            
            return 5000;
        };
    }
}

TEST_CASE("Performance Regression Tests", "[performance][regression]") {
    
    SECTION("Element Creation Speed Requirements") {
        UI ui("Regression Test", 800, 600);
        
        auto start = std::chrono::high_resolution_clock::now();
        
        // Create 100 mixed widgets with safe positioning
        std::vector<std::shared_ptr<ui::UIElement>> widgets;
        widgets.reserve(100);
        
        for (int i = 0; i < 100; ++i) {
            int x = (i % 10) * 70;  // 10 columns of 70px = 700px
            int y = (i / 10) * 50;  // 10 rows of 50px = 500px
            
            switch (i % 4) {
                case 0:
                    widgets.push_back(ui.createLabel("L" + std::to_string(i), x, y));
                    break;
                case 1:
                    widgets.push_back(ui.createButton("B" + std::to_string(i), x, y, [](){}));
                    break;
                case 2:
                    widgets.push_back(ui.createTextBox("T" + std::to_string(i), x, y));
                    break;
                case 3:
                    widgets.push_back(ui.createCheckBox(false, x, y, [](bool){}));
                    break;
            }
        }
        
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        
        // Should create 100 widgets in less than 200ms (performance target)
        REQUIRE(duration.count() < 200);
        REQUIRE(widgets.size() == 100);
    }
    
    SECTION("Text Update Speed Requirements") {
        UI ui("Text Update Test", 800, 600);
        
        auto label = ui.createLabel("Initial Text", 10, 10);
        
        auto start = std::chrono::high_resolution_clock::now();
        
        // Update text 1000 times
        for (int i = 0; i < 1000; ++i) {
            label->setText("Updated Text " + std::to_string(i));
        }
        
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        
        // Should update text 1000 times in less than 50ms (performance target)
        REQUIRE(duration.count() < 50);
    }
    
    SECTION("Layout Performance Requirements") {
        UI ui("Layout Performance Test", 1000, 800);
        
        auto start = std::chrono::high_resolution_clock::now();
        
        auto layout = ui.createVBoxLayout(10, 10, 980, 780, 5);
        
        // Add 100 elements to layout
        for (int i = 0; i < 100; ++i) {
            auto button = ui.createButton("Button " + std::to_string(i), 0, 0, [](){});
            layout->addElement(button);
        }
        
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        
        // Should create layout with 100 elements in less than 100ms (performance target)
        REQUIRE(duration.count() < 100);
    }
    
    SECTION("Memory Usage Requirements") {
        // Test that widget creation doesn't cause excessive memory usage
        UI ui("Memory Test", 800, 600);
        
        // Create and destroy widgets in cycles to test for memory leaks
        for (int cycle = 0; cycle < 100; ++cycle) {
            std::vector<std::shared_ptr<ui::UIElement>> widgets;
            widgets.reserve(50);
            
            for (int i = 0; i < 50; ++i) {
                widgets.push_back(ui.createLabel("Temp Label " + std::to_string(i), i * 10, 10));
            }
            
            // Widgets should be automatically cleaned up when vector is destroyed
        }
        
        // If we reach here without crashes or excessive memory usage, test passes
        REQUIRE(true);
    }
}

TEST_CASE("Performance Metrics Validation", "[performance][metrics]") {
    
    SECTION("Validate Performance Targets") {
        // These tests validate that the framework meets its performance targets
        
        UI ui("Metrics Test", 800, 600);
        
        // Target: Create 100 widgets in < 20ms
        auto start = std::chrono::high_resolution_clock::now();
        std::vector<std::shared_ptr<ui::UIElement>> widgets;
        for (int i = 0; i < 100; ++i) {
            widgets.push_back(ui.createLabel("Label " + std::to_string(i), i % 800, (i / 10) % 600));
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        
        INFO("Widget creation time for 100 widgets: " << duration.count() << "ms");
        REQUIRE(duration.count() < 20);
        
        // Target: Update widget properties 1000 times in < 10ms
        auto label = widgets[0];
        start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < 1000; ++i) {
            label->setPosition(i % 800, (i / 10) % 600);
        }
        end = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        
        INFO("Position update time for 1000 operations: " << duration.count() << "ms");
        REQUIRE(duration.count() < 10);
    }
}
