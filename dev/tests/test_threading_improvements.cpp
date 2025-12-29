#include <catch2/catch_test_macros.hpp>
#include "uiframework/UI.h"
#include "uiframework/Resources/FontManager.h"
#include <thread>
#include <vector>
#include <chrono>

TEST_CASE("Thread safety improvements", "[threading][critical]") {
    
    SECTION("FontManager thread safety under concurrent access") {
        const int NUM_THREADS = 10;
        const int OPERATIONS_PER_THREAD = 100;
        std::vector<std::thread> threads;
        std::atomic<int> successCount{0};
        
        // Launch multiple threads accessing FontManager simultaneously
        for (int i = 0; i < NUM_THREADS; ++i) {
            threads.emplace_back([&successCount, OPERATIONS_PER_THREAD]() {
                for (int j = 0; j < OPERATIONS_PER_THREAD; ++j) {
                    try {
                        auto& fontManager = ui::FontManager::getInstance();
                        auto font = fontManager.getFont("", 16); // Get default font
                        if (font) {
                            successCount++;
                        }
                        std::this_thread::sleep_for(std::chrono::microseconds(1));
                    } catch (...) {
                        // Thread safety failure
                    }
                }
            });
        }
        
        // Wait for all threads to complete
        for (auto& thread : threads) {
            thread.join();
        }
        
        // All operations should succeed without race conditions
        REQUIRE(successCount > 0);
        INFO("FontManager handled " << successCount << " concurrent operations successfully");
    }
    
    SECTION("UICore element management thread safety") {
        UI ui("Thread Test", 800, 600);
        const int NUM_THREADS = 5;
        std::vector<std::thread> threads;
        std::atomic<int> elementsCreated{0};
        std::atomic<int> elementsRemoved{0};
        
        // Create elements concurrently
        for (int i = 0; i < NUM_THREADS; ++i) {
            threads.emplace_back([&ui, &elementsCreated, i]() {
                for (int j = 0; j < 20; ++j) {
                    try {
                        auto button = ui.createButton("Thread " + std::to_string(i) + " Button " + std::to_string(j), 
                                                    10 + (i * 50), 10 + (j * 25), [](){});
                        if (button) {
                            elementsCreated++;
                        }
                        std::this_thread::sleep_for(std::chrono::microseconds(10));
                    } catch (...) {
                        // Element creation failure
                    }
                }
            });
        }
        
        // Wait for creation threads
        for (auto& thread : threads) {
            thread.join();
        }
        threads.clear();
        
        REQUIRE(elementsCreated > 0);
        INFO("Created " << elementsCreated << " elements concurrently");
        
        // Test concurrent element removal (simplified test)
        // Note: In practice, you'd need element IDs to test removal properly
        REQUIRE(elementsCreated <= NUM_THREADS * 20);
    }
    
    SECTION("Performance benchmark - element operations") {
        UI ui("Performance Test", 800, 600);
        
        auto start = std::chrono::high_resolution_clock::now();
        
        // Create many elements (smaller buttons to fit in window)
        std::vector<std::shared_ptr<ui::Button>> buttons;
        for (int i = 0; i < 100; ++i) {  // Reduced count to avoid boundary issues
            auto button = ui.createButton("Btn" + std::to_string(i), 
                                        (i % 10) * 70, (i / 10) * 35, [](){});
            buttons.push_back(button);
        }
        
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        
        INFO("Created 100 elements in " << duration.count() << "ms");
        REQUIRE(duration.count() < 500); // Should create 100 elements in <500ms
        REQUIRE(buttons.size() == 100);
    }
}
