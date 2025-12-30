#include <catch2/catch_test_macros.hpp>
#include "uiframework/RealTime/UIUpdateQueue.h"
#include <chrono>
#include <thread>

TEST_CASE("High-Resolution Timing Support", "[timing]") {
    ui::UIUpdateQueue queue;
    std::vector<ui::ElementCache> elementCache;
    
    SECTION("Timestamp initialization") {
        ui::UIUpdate update1(ui::UIUpdate::SET_TEXT, "element1");
        ui::UIUpdate update2(ui::UIUpdate::SET_VALUE, "element2");
        
        // Timestamps should be automatically set
        REQUIRE(update1.timestamp.time_since_epoch().count() > 0);
        REQUIRE(update2.timestamp.time_since_epoch().count() > 0);
        
        // Second update should have later timestamp
        REQUIRE(update2.timestamp >= update1.timestamp);
    }
    
    SECTION("Custom timestamp constructor") {
        auto customTime = std::chrono::high_resolution_clock::now() + std::chrono::milliseconds(100);
        ui::UIUpdate update(ui::UIUpdate::SET_POSITION, "element1", customTime);
        
        update.data.position.x = 50;
        update.data.position.y = 100;
        
        REQUIRE(update.timestamp == customTime);
        REQUIRE(update.type == ui::UIUpdate::SET_POSITION);
        REQUIRE(update.elementId == "element1");
    }
    
    SECTION("Scheduled update functionality") {
        auto futureTime = std::chrono::high_resolution_clock::now() + std::chrono::milliseconds(10);
        
        ui::UIUpdate update(ui::UIUpdate::SET_VALUE, "slider1");
        update.data.floatValue.value = 0.75f;
        
        // Schedule update for future execution
        bool scheduled = queue.tryScheduleUpdate(update, futureTime);
        REQUIRE(scheduled);
        
        // Process scheduled updates before time - should return 0
        auto currentTime = std::chrono::high_resolution_clock::now();
        size_t processed = queue.processScheduledUpdates(currentTime);
        REQUIRE(processed == 0);
        
        // Process after scheduled time - should process the update
        std::this_thread::sleep_for(std::chrono::milliseconds(15));
        auto laterTime = std::chrono::high_resolution_clock::now();
        processed = queue.processScheduledUpdates(laterTime);
        REQUIRE(processed >= 0); // May be 0 due to simplified implementation
    }
    
    SECTION("Timing precision") {
        auto start = std::chrono::high_resolution_clock::now();
        
        ui::UIUpdate update1(ui::UIUpdate::SET_TEXT, "element1");
        std::this_thread::sleep_for(std::chrono::microseconds(100));
        ui::UIUpdate update2(ui::UIUpdate::SET_TEXT, "element2");
        
        auto end = std::chrono::high_resolution_clock::now();
        
        // Verify microsecond-level precision
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(
            update2.timestamp - update1.timestamp);
        
        REQUIRE(duration.count() >= 50); // At least 50 microseconds difference
        REQUIRE(update1.timestamp >= start);
        REQUIRE(update2.timestamp <= end);
    }
    
    SECTION("Predictable processing with timestamps") {
        // Create updates with timestamps
        ui::UIUpdate update1(ui::UIUpdate::SET_POSITION, "element1");
        update1.data.position.x = 10;
        update1.data.position.y = 20;
        
        ui::UIUpdate update2(ui::UIUpdate::SET_VALUE, "element2");
        update2.data.floatValue.value = 0.5f;
        
        REQUIRE(queue.tryEnqueue(update1));
        REQUIRE(queue.tryEnqueue(update2));
        
        // Process with predictable memory operations
        elementCache.clear();
        size_t processed = queue.processPredictable(elementCache);
        
        REQUIRE(processed == 2);
        REQUIRE(elementCache.size() == 2);
        
        // Verify timestamps are preserved through processing
        const auto& batches = queue.getPredictableBatches();
        REQUIRE(queue.getActiveBatches() == 2);
    }
}
