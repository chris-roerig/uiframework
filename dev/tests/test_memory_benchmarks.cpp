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
        
        BENCHMARK("Memory usage: 50 Labels") {
            size_t startMemory = getCurrentMemoryUsage();
            
            std::vector<std::shared_ptr<ui::Label>> widgets;
            widgets.reserve(50);
            
            for (int i = 0; i < 50; ++i) {
                widgets.push_back(ui.createLabel("Label " + std::to_string(i), i % 25 * 15, i / 25 * 20));
            }
            
            size_t endMemory = getCurrentMemoryUsage();
            size_t memoryUsed = endMemory > startMemory ? endMemory - startMemory : 0;
            
            return memoryUsed;
        };
        
        BENCHMARK("Memory usage: 50 Buttons") {
            size_t startMemory = getCurrentMemoryUsage();
            
            std::vector<std::shared_ptr<ui::Button>> widgets;
            widgets.reserve(50);
            
            for (int i = 0; i < 50; ++i) {
                widgets.push_back(ui.createButton("Button " + std::to_string(i), i % 25 * 15, i / 25 * 20, [](){}));
            }
            
            size_t endMemory = getCurrentMemoryUsage();
            size_t memoryUsed = endMemory > startMemory ? endMemory - startMemory : 0;
            
            return memoryUsed;
        };
        
        BENCHMARK("Memory usage: 50 TextBoxes") {
            size_t startMemory = getCurrentMemoryUsage();
            
            std::vector<std::shared_ptr<ui::TextBox>> widgets;
            widgets.reserve(50);
            
            for (int i = 0; i < 50; ++i) {
                widgets.push_back(ui.createTextBox("TextBox " + std::to_string(i), i % 25 * 15, i / 25 * 20));
            }
            
            size_t endMemory = getCurrentMemoryUsage();
            size_t memoryUsed = endMemory > startMemory ? endMemory - startMemory : 0;
            
            return memoryUsed;
        };
    }
    
    SECTION("Memory leak detection") {
        UI ui("Leak Test", 400, 300);
        
        BENCHMARK("Widget lifecycle memory") {
            size_t initialMemory = getCurrentMemoryUsage();
            
            // Test widget creation/destruction in batches
            for (int cycle = 0; cycle < 5; ++cycle) {
                std::vector<std::shared_ptr<ui::UIElement>> widgets;
                widgets.reserve(20);
                
                // Create widgets
                for (int i = 0; i < 4; ++i) {
                    widgets.push_back(ui.createLabel("Label", i * 30, 10));
                    widgets.push_back(ui.createButton("Button", i * 30, 40, [](){}));
                    widgets.push_back(ui.createTextBox("Text", i * 30, 70));
                    widgets.push_back(ui.createCheckBox(false, i * 30, 100, [](bool){}));
                    
                    if (i < 2) {
                        std::vector<std::string> options = {"A", "B"};
                        widgets.push_back(ui.createOptionSelect(options, 0, i * 60, 130, [](int){}));
                    }
                }
                
                // Clear widgets to test cleanup
                widgets.clear();
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
            widgets.reserve(100);
            
            // Create widgets well within window boundaries
            for (int i = 0; i < 25; ++i) {
                int x = (i % 5) * 150;  // 5 columns
                int y = (i / 5) * 100;  // 5 rows
                
                widgets.push_back(ui.createLabel("L" + std::to_string(i), x, y));
                widgets.push_back(ui.createButton("B" + std::to_string(i), x, y + 25, [](){}));
                widgets.push_back(ui.createTextBox("T" + std::to_string(i), x, y + 50));
                widgets.push_back(ui.createCheckBox(false, x, y + 75, [](bool){}));
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
            
            // Create widgets in smaller batches
            for (int batch = 0; batch < 10; ++batch) {
                for (int i = 0; i < 20; ++i) {
                    int x = (i % 10) * 80;
                    int y = (i / 10) * 50 + batch * 60;
                    
                    widgets.push_back(ui.createButton("Btn" + std::to_string(batch * 20 + i), x, y, [](){}));
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
            
            // UI should use reasonable amount of memory (less than 50MB for empty window with SDL/fonts)
            REQUIRE(uiFootprint < 50 * 1024 * 1024);
            
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
        for (int count = 0; count <= 200; count += 25) {
            std::vector<std::shared_ptr<ui::Label>> labels;
            labels.reserve(count);
            
            for (int i = 0; i < count; ++i) {
                int x = (i % 20) * 35;  // Stay within 800px window
                int y = (i / 20) * 25;  // Stay within 600px window
                labels.push_back(ui.createLabel("Label", x, y));
            }
            
            memorySamples.push_back(getCurrentMemoryUsage());
        }
        
        // Memory should scale reasonably (not exponentially)
        for (size_t i = 1; i < memorySamples.size(); ++i) {
            size_t growth = memorySamples[i] > memorySamples[i-1] ? 
                           memorySamples[i] - memorySamples[i-1] : 0;
            
            // Each batch of 25 widgets should not use more than 512KB
            REQUIRE(growth < 512 * 1024);
            
            INFO("Memory growth for 25 widgets: " << growth << " bytes");
        }
    }
}
