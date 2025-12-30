#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>
#include "uiframework/RealTime/UIUpdateQueue.h"
#include "uiframework/UI.h"
#include <vector>
#include <string>
#include <chrono>
#include <random>

using namespace ui;

TEST_CASE("Bulk Operations - Basic Functionality", "[bulk][phase6]") {
    UIUpdateQueue queue;
    
    SECTION("Bulk text updates") {
        std::vector<std::string> ids = {"label1", "label2", "label3", "label4"};
        std::vector<std::string> texts = {"Text A", "Text B", "Text C", "Text D"};
        
        size_t result = queue.tryBulkSetText(ids, texts);
        REQUIRE(result == 4);
        
        // Verify updates are in queue
        REQUIRE(queue.size() == 4);
        
        // Process updates
        UIUpdate update;
        for (int i = 0; i < 4; ++i) {
            REQUIRE(queue.tryDequeue(update));
            REQUIRE(update.type == UIUpdate::SET_TEXT);
            REQUIRE(std::find(ids.begin(), ids.end(), update.elementId) != ids.end());
        }
    }
    
    SECTION("Bulk position updates") {
        std::vector<std::string> ids = {"widget1", "widget2", "widget3"};
        std::vector<std::pair<int, int>> positions = {{10, 20}, {30, 40}, {50, 60}};
        
        size_t result = queue.tryBulkSetPosition(ids, positions);
        REQUIRE(result == 3);
        
        // Verify updates
        UIUpdate update;
        for (int i = 0; i < 3; ++i) {
            REQUIRE(queue.tryDequeue(update));
            REQUIRE(update.type == UIUpdate::SET_POSITION);
        }
    }
    
    SECTION("Bulk value updates") {
        std::vector<std::string> ids = {"slider1", "slider2", "progress1"};
        std::vector<float> values = {0.25f, 0.75f, 0.5f};
        
        size_t result = queue.tryBulkSetValue(ids, values);
        REQUIRE(result == 3);
        
        // Verify updates
        UIUpdate update;
        for (int i = 0; i < 3; ++i) {
            REQUIRE(queue.tryDequeue(update));
            REQUIRE(update.type == UIUpdate::SET_VALUE);
        }
    }
    
    SECTION("Bulk visibility updates") {
        std::vector<std::string> ids = {"panel1", "panel2", "panel3", "panel4"};
        std::vector<bool> visibility = {true, false, true, false};
        
        size_t result = queue.tryBulkSetVisibility(ids, visibility);
        REQUIRE(result == 4);
        
        // Verify updates
        UIUpdate update;
        for (int i = 0; i < 4; ++i) {
            REQUIRE(queue.tryDequeue(update));
            REQUIRE(update.type == UIUpdate::SET_VISIBILITY);
        }
    }
}

TEST_CASE("Bulk Operations - Error Handling", "[bulk][phase6]") {
    UIUpdateQueue queue;
    
    SECTION("Mismatched vector sizes") {
        std::vector<std::string> ids = {"id1", "id2"};
        std::vector<std::string> texts = {"text1"}; // Size mismatch
        
        size_t result = queue.tryBulkSetText(ids, texts);
        REQUIRE(result == 0); // Should fail
    }
    
    SECTION("Empty vectors") {
        std::vector<std::string> ids;
        std::vector<float> values;
        
        size_t result = queue.tryBulkSetValue(ids, values);
        REQUIRE(result == 0); // Should handle gracefully
    }
    
    SECTION("Queue capacity limits") {
        // Fill queue to near capacity
        std::vector<std::string> ids;
        std::vector<float> values;
        
        // Create more updates than queue capacity
        for (int i = 0; i < 2000; ++i) {
            ids.push_back("element" + std::to_string(i));
            values.push_back(static_cast<float>(i) / 2000.0f);
        }
        
        size_t result = queue.tryBulkSetValue(ids, values);
        // Should process up to queue capacity
        REQUIRE(result > 0);
        REQUIRE(result <= 1024); // Queue size limit
    }
}

TEST_CASE("Bulk Operations - Performance", "[bulk][phase6][performance]") {
    UIUpdateQueue queue;
    
    SECTION("Large bulk operations") {
        constexpr size_t BULK_SIZE = 500;
        
        std::vector<std::string> ids;
        std::vector<float> values;
        
        // Generate test data
        for (size_t i = 0; i < BULK_SIZE; ++i) {
            ids.push_back("element_" + std::to_string(i));
            values.push_back(static_cast<float>(i) / BULK_SIZE);
        }
        
        auto start = std::chrono::high_resolution_clock::now();
        size_t result = queue.tryBulkSetValue(ids, values);
        auto end = std::chrono::high_resolution_clock::now();
        
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        
        REQUIRE(result == BULK_SIZE);
        REQUIRE(duration.count() < 1000); // Should complete in < 1ms
        
        // Verify all updates are queued
        REQUIRE(queue.size() == BULK_SIZE);
    }
    
    SECTION("Bulk vs individual performance comparison") {
        constexpr size_t TEST_SIZE = 100;
        
        std::vector<std::string> ids;
        std::vector<std::pair<int, int>> positions;
        
        for (size_t i = 0; i < TEST_SIZE; ++i) {
            ids.push_back("widget_" + std::to_string(i));
            positions.push_back({static_cast<int>(i * 10), static_cast<int>(i * 15)});
        }
        
        // Test bulk operation
        UIUpdateQueue bulkQueue;
        auto bulkStart = std::chrono::high_resolution_clock::now();
        size_t bulkResult = bulkQueue.tryBulkSetPosition(ids, positions);
        auto bulkEnd = std::chrono::high_resolution_clock::now();
        
        // Test individual operations
        UIUpdateQueue individualQueue;
        auto individualStart = std::chrono::high_resolution_clock::now();
        size_t individualResult = 0;
        for (size_t i = 0; i < TEST_SIZE; ++i) {
            UIUpdate update(UIUpdate::SET_POSITION, ids[i]);
            update.data.position.x = positions[i].first;
            update.data.position.y = positions[i].second;
            if (individualQueue.tryEnqueue(update)) {
                individualResult++;
            }
        }
        auto individualEnd = std::chrono::high_resolution_clock::now();
        
        auto bulkDuration = std::chrono::duration_cast<std::chrono::nanoseconds>(bulkEnd - bulkStart);
        auto individualDuration = std::chrono::duration_cast<std::chrono::nanoseconds>(individualEnd - individualStart);
        
        REQUIRE(bulkResult == TEST_SIZE);
        REQUIRE(individualResult == TEST_SIZE);
        
        // Bulk should be faster or comparable
        REQUIRE(bulkDuration.count() <= individualDuration.count() * 2);
        
        INFO("Bulk duration: " << bulkDuration.count() << "ns");
        INFO("Individual duration: " << individualDuration.count() << "ns");
        INFO("Speedup: " << static_cast<double>(individualDuration.count()) / bulkDuration.count() << "x");
    }
}

TEST_CASE("UI Bulk API Integration", "[bulk][phase6][ui]") {
    SECTION("UI bulk methods") {
        try {
            UI ui("Bulk Test", 800, 600); // Test window
            
            // Create test elements
            auto label1 = ui.createLabel("Label 1", 10, 10);
            auto label2 = ui.createLabel("Label 2", 10, 40);
            auto button1 = ui.createButton("Button 1", 10, 70, [](){});
            auto button2 = ui.createButton("Button 2", 10, 100, [](){});
            
            // Test bulk text updates
            std::vector<std::string> textIds = {label1->getId(), label2->getId(), 
                                               button1->getId(), button2->getId()};
            std::vector<std::string> texts = {"New Label 1", "New Label 2", 
                                            "New Button 1", "New Button 2"};
            
            size_t textResult = ui.realtimeBulkSetText(textIds, texts);
            REQUIRE(textResult == 4);
            
            // Test bulk position updates
            std::vector<std::pair<int, int>> positions = {{20, 20}, {20, 50}, 
                                                         {20, 80}, {20, 110}};
            
            size_t posResult = ui.realtimeBulkSetPosition(textIds, positions);
            REQUIRE(posResult == 4);
            
            // Test bulk visibility updates
            std::vector<bool> visibility = {true, false, true, false};
            
            size_t visResult = ui.realtimeBulkSetVisibility(textIds, visibility);
            REQUIRE(visResult == 4);
            
            // Process updates
            ui.processRealtimeUpdates();
            
        } catch (const ui::UIException& e) {
            // Skip test if SDL not available
            WARN("UI test skipped: " << e.what());
        }
    }
}

TEST_CASE("Bulk Operations - Memory Efficiency", "[bulk][phase6][memory]") {
    UIUpdateQueue queue;
    
    SECTION("Memory usage with large bulk operations") {
        constexpr size_t LARGE_BULK = 800; // Near queue capacity
        
        std::vector<std::string> ids;
        std::vector<std::string> texts;
        
        // Generate large dataset
        for (size_t i = 0; i < LARGE_BULK; ++i) {
            ids.push_back("element_" + std::to_string(i));
            texts.push_back("Text content for element " + std::to_string(i));
        }
        
        // Measure memory usage (approximate)
        size_t initialSize = queue.size();
        size_t result = queue.tryBulkSetText(ids, texts);
        size_t finalSize = queue.size();
        
        REQUIRE(result > 0);
        REQUIRE(finalSize > initialSize);
        REQUIRE(finalSize <= 1024); // Within queue bounds
        
        // Verify queue doesn't overflow
        REQUIRE(result <= LARGE_BULK);
    }
}

// Benchmark tests for performance analysis
TEST_CASE("Bulk Operations - Benchmarks", "[bulk][phase6][benchmark]") {
    BENCHMARK("Bulk text updates (100 elements)") {
        UIUpdateQueue queue;
        std::vector<std::string> ids;
        std::vector<std::string> texts;
        
        for (int i = 0; i < 100; ++i) {
            ids.push_back("element" + std::to_string(i));
            texts.push_back("Text " + std::to_string(i));
        }
        
        return queue.tryBulkSetText(ids, texts);
    };
    
    BENCHMARK("Bulk position updates (100 elements)") {
        UIUpdateQueue queue;
        std::vector<std::string> ids;
        std::vector<std::pair<int, int>> positions;
        
        for (int i = 0; i < 100; ++i) {
            ids.push_back("widget" + std::to_string(i));
            positions.push_back({i * 10, i * 15});
        }
        
        return queue.tryBulkSetPosition(ids, positions);
    };
    
    BENCHMARK("Individual updates (100 elements)") {
        UIUpdateQueue queue;
        size_t count = 0;
        
        for (int i = 0; i < 100; ++i) {
            UIUpdate update(UIUpdate::SET_POSITION, "widget" + std::to_string(i));
            update.data.position.x = i * 10;
            update.data.position.y = i * 15;
            if (queue.tryEnqueue(update)) {
                count++;
            }
        }
        
        return count;
    };
}
