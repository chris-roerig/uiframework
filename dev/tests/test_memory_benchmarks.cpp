#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>
#include "uiframework/UI.h"
#include <vector>
#include <memory>

#ifdef __APPLE__
#include <mach/mach.h>
#elif defined(__linux__)
#include <fstream>
#include <string>
#elif defined(_WIN32)
#include <windows.h>
#include <psapi.h>
#endif

/**
 * @file test_memory_benchmarks.cpp
 * @brief Memory usage benchmarks for UI Framework
 * 
 * Measures memory consumption and detects memory leaks
 */

// Cross-platform memory usage function
size_t getCurrentMemoryUsage() {
#ifdef __APPLE__
    struct mach_task_basic_info info;
    mach_msg_type_number_t infoCount = MACH_TASK_BASIC_INFO_COUNT;
    if (task_info(mach_task_self(), MACH_TASK_BASIC_INFO, (task_info_t)&info, &infoCount) != KERN_SUCCESS) {
        return 0;
    }
    return info.resident_size;
#elif defined(__linux__)
    std::ifstream file("/proc/self/status");
    std::string line;
    while (std::getline(file, line)) {
        if (line.substr(0, 6) == "VmRSS:") {
            size_t kb = std::stoul(line.substr(7));
            return kb * 1024; // Convert KB to bytes
        }
    }
    return 0;
#elif defined(_WIN32)
    PROCESS_MEMORY_COUNTERS pmc;
    if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc))) {
        return pmc.WorkingSetSize;
    }
    return 0;
#else
    return 0; // Unsupported platform
#endif
}

TEST_CASE("Memory Usage Benchmarks", "[benchmark][memory]") {
    
    SECTION("Memory per widget type") {
        UI ui("Memory Test", 800, 600);
        
        BENCHMARK("Memory usage: 100 Labels") {
            size_t startMemory = getCurrentMemoryUsage();
            
            std::vector<std::shared_ptr<ui::Label>> widgets;
            widgets.reserve(100);
            
            for (int i = 0; i < 100; ++i) {
                widgets.push_back(ui.createLabel("Label " + std::to_string(i), i % 50 * 10, i / 50 * 20));
            }
            
            size_t endMemory = getCurrentMemoryUsage();
            size_t memoryUsed = endMemory > startMemory ? endMemory - startMemory : 0;
            
            return memoryUsed;
        };
        
        BENCHMARK("Memory usage: 100 Buttons") {
            size_t startMemory = getCurrentMemoryUsage();
            
            std::vector<std::shared_ptr<ui::Button>> widgets;
            widgets.reserve(100);
            
            for (int i = 0; i < 100; ++i) {
                widgets.push_back(ui.createButton("Button " + std::to_string(i), i % 50 * 10, i / 50 * 20, [](){}));
            }
            
            size_t endMemory = getCurrentMemoryUsage();
            size_t memoryUsed = endMemory > startMemory ? endMemory - startMemory : 0;
            
            return memoryUsed;
        };
        
        BENCHMARK("Memory usage: 100 TextBoxes") {
            size_t startMemory = getCurrentMemoryUsage();
            
            std::vector<std::shared_ptr<ui::TextBox>> widgets;
            widgets.reserve(100);
            
            for (int i = 0; i < 100; ++i) {
                widgets.push_back(ui.createTextBox("TextBox " + std::to_string(i), i % 50 * 10, i / 50 * 20));
            }
            
            size_t endMemory = getCurrentMemoryUsage();
            size_t memoryUsed = endMemory > startMemory ? endMemory - startMemory : 0;
            
            return memoryUsed;
        };
    }
    
    SECTION("Memory leak detection") {
        
        BENCHMARK("Widget creation/destruction cycles") {
            size_t initialMemory = getCurrentMemoryUsage();
            
            for (int cycle = 0; cycle < 10; ++cycle) {
                UI ui("Leak Test", 400, 300);
                
                std::vector<std::shared_ptr<ui::UIElement>> widgets;
                widgets.reserve(50);
                
                // Create various widgets
                for (int i = 0; i < 10; ++i) {
                    widgets.push_back(ui.createLabel("Label", i * 20, 10));
                    widgets.push_back(ui.createButton("Button", i * 20, 40, [](){}));
                    widgets.push_back(ui.createTextBox("Text", i * 20, 70));
                    widgets.push_back(ui.createCheckBox(false, i * 20, 100, [](bool){}));
                    
                    if (i < 5) {
                        std::vector<std::string> options = {"Option1", "Option2"};
                        widgets.push_back(ui.createOptionSelect(0, options, i * 40, 130, [](int){}));
                    }
                }
                
                // Widgets automatically destroyed when UI goes out of scope
            }
            
            size_t finalMemory = getCurrentMemoryUsage();
            size_t memoryDiff = finalMemory > initialMemory ? finalMemory - initialMemory : 0;
            
            return memoryDiff;
        };
    }
    
    SECTION("Memory efficiency metrics") {
        
        BENCHMARK("Memory per widget (average)") {
            UI ui("Efficiency Test", 800, 600);
            size_t startMemory = getCurrentMemoryUsage();
            
            std::vector<std::shared_ptr<ui::UIElement>> widgets;
            widgets.reserve(1000);
            
            // Create widgets well within window boundaries (800x600)
            for (int i = 0; i < 25; ++i) {
                int x = (i % 2) * 300;  // 2 columns: 0, 300 (TextBox: 300+200=500 < 800)
                int y = (i / 2) * 40;   // Rows: 0, 40, 80... (max: 12*40=480, CheckBox: 480+105=585 < 600)
                
                widgets.push_back(ui.createLabel("L" + std::to_string(i), x, y));
                widgets.push_back(ui.createButton("B" + std::to_string(i), x, y + 35, [](){}));
                widgets.push_back(ui.createTextBox("T" + std::to_string(i), x, y + 70));
                widgets.push_back(ui.createCheckBox(false, x, y + 105, [](bool){}));
            }
            
            size_t endMemory = getCurrentMemoryUsage();
            size_t totalMemory = endMemory > startMemory ? endMemory - startMemory : 0;
            size_t memoryPerWidget = widgets.size() > 0 ? totalMemory / widgets.size() : 0;
            
            return memoryPerWidget;
        };
        
        BENCHMARK("Peak memory usage") {
            UI ui("Peak Test", 1024, 768);
            
            size_t peakMemory = getCurrentMemoryUsage();
            std::vector<std::shared_ptr<ui::UIElement>> widgets;
            
            // Gradually create widgets and track peak memory
            for (int batch = 0; batch < 20; ++batch) {
                for (int i = 0; i < 50; ++i) {
                    int x = (i % 10) * 80;
                    int y = (i / 10) * 60 + batch * 30;
                    
                    widgets.push_back(ui.createButton("Btn" + std::to_string(batch * 50 + i), x, y, [](){}));
                }
                
                size_t currentMemory = getCurrentMemoryUsage();
                if (currentMemory > peakMemory) {
                    peakMemory = currentMemory;
                }
            }
            
            return peakMemory;
        };
    }
}

TEST_CASE("Memory Baseline Measurements", "[memory][baseline]") {
    
    SECTION("Empty UI memory footprint") {
        size_t beforeUI = getCurrentMemoryUsage();
        
        {
            UI ui("Baseline Test", 800, 600);
            size_t withUI = getCurrentMemoryUsage();
            
            size_t uiFootprint = withUI > beforeUI ? withUI - beforeUI : 0;
            
            // UI should use reasonable amount of memory (less than 10MB for empty window)
            REQUIRE(uiFootprint < 10 * 1024 * 1024);
            
            INFO("Empty UI memory footprint: " << uiFootprint << " bytes");
        }
        
        size_t afterUI = getCurrentMemoryUsage();
        
        // Memory should be mostly freed after UI destruction
        // Allow some tolerance for memory fragmentation
        size_t memoryLeak = afterUI > beforeUI ? afterUI - beforeUI : 0;
        
        INFO("Memory after UI destruction: " << memoryLeak << " bytes difference");
        
        // Should not leak more than 2MB (SDL initialization overhead)
        REQUIRE(memoryLeak < 2 * 1024 * 1024);
    }
    
    SECTION("Widget memory scaling") {
        UI ui("Scaling Test", 800, 600);
        
        std::vector<size_t> memorySamples;
        memorySamples.reserve(10);
        
        // Measure memory at different widget counts
        for (int count = 0; count <= 500; count += 50) {
            std::vector<std::shared_ptr<ui::Label>> labels;
            labels.reserve(count);
            
            for (int i = 0; i < count; ++i) {
                int x = (i % 35) * 20;  // Stay within 800px window
                int y = (i / 35) * 15;  // Stay within 600px window
                labels.push_back(ui.createLabel("Label", x, y));
            }
            
            memorySamples.push_back(getCurrentMemoryUsage());
        }
        
        // Memory should scale reasonably (not exponentially)
        for (size_t i = 1; i < memorySamples.size(); ++i) {
            size_t growth = memorySamples[i] > memorySamples[i-1] ? 
                           memorySamples[i] - memorySamples[i-1] : 0;
            
            // Each batch of 50 widgets should not use more than 1MB
            REQUIRE(growth < 1024 * 1024);
            
            INFO("Memory growth for 50 widgets: " << growth << " bytes");
        }
    }
}
