#include <catch2/catch_test_macros.hpp>
#include "uiframework/RealTime/UIUpdateQueue.h"
#include <unordered_map>

TEST_CASE("High-Frequency Update Batching System", "[batching]") {
    ui::UIUpdateQueue queue;
    std::unordered_map<std::string, ui::BatchedUpdate> batchMap;
    
    SECTION("Coalesces multiple position updates") {
        // Enqueue multiple position updates for same element
        ui::UIUpdate update1(ui::UIUpdate::SET_POSITION, "element1");
        update1.data.position.x = 10;
        update1.data.position.y = 20;
        
        ui::UIUpdate update2(ui::UIUpdate::SET_POSITION, "element1");
        update2.data.position.x = 30;
        update2.data.position.y = 40;
        
        ui::UIUpdate update3(ui::UIUpdate::SET_POSITION, "element1");
        update3.data.position.x = 50;
        update3.data.position.y = 60;
        
        REQUIRE(queue.tryEnqueue(update1));
        REQUIRE(queue.tryEnqueue(update2));
        REQUIRE(queue.tryEnqueue(update3));
        
        // Process batch - should coalesce to final position
        size_t processed = queue.processBatch(batchMap);
        REQUIRE(processed == 3);
        REQUIRE(batchMap.size() == 1);
        
        const auto& batch = batchMap["element1"];
        REQUIRE(batch.hasPosition);
        REQUIRE(batch.position.x == 50);  // Latest value
        REQUIRE(batch.position.y == 60);  // Latest value
    }
    
    SECTION("Coalesces different update types for same element") {
        ui::UIUpdate posUpdate(ui::UIUpdate::SET_POSITION, "element1");
        posUpdate.data.position.x = 100;
        posUpdate.data.position.y = 200;
        
        ui::UIUpdate sizeUpdate(ui::UIUpdate::SET_SIZE, "element1");
        sizeUpdate.data.size.width = 300;
        sizeUpdate.data.size.height = 400;
        
        ui::UIUpdate valueUpdate(ui::UIUpdate::SET_VALUE, "element1");
        valueUpdate.data.floatValue.value = 0.75f;
        
        ui::UIUpdate textUpdate(ui::UIUpdate::SET_TEXT, "element1");
        textUpdate.textValue = "Batched Text";
        
        REQUIRE(queue.tryEnqueue(posUpdate));
        REQUIRE(queue.tryEnqueue(sizeUpdate));
        REQUIRE(queue.tryEnqueue(valueUpdate));
        REQUIRE(queue.tryEnqueue(textUpdate));
        
        batchMap.clear();
        size_t processed = queue.processBatch(batchMap);
        REQUIRE(processed == 4);
        REQUIRE(batchMap.size() == 1);
        
        const auto& batch = batchMap["element1"];
        REQUIRE(batch.hasPosition);
        REQUIRE(batch.hasSize);
        REQUIRE(batch.hasValue);
        REQUIRE(batch.hasText);
        
        REQUIRE(batch.position.x == 100);
        REQUIRE(batch.position.y == 200);
        REQUIRE(batch.size.width == 300);
        REQUIRE(batch.size.height == 400);
        REQUIRE(batch.value == 0.75f);
        REQUIRE(batch.textValue == "Batched Text");
    }
    
    SECTION("Handles multiple elements independently") {
        ui::UIUpdate update1(ui::UIUpdate::SET_POSITION, "element1");
        update1.data.position.x = 10;
        update1.data.position.y = 20;
        
        ui::UIUpdate update2(ui::UIUpdate::SET_POSITION, "element2");
        update2.data.position.x = 30;
        update2.data.position.y = 40;
        
        ui::UIUpdate update3(ui::UIUpdate::SET_VALUE, "element1");
        update3.data.floatValue.value = 0.5f;
        
        REQUIRE(queue.tryEnqueue(update1));
        REQUIRE(queue.tryEnqueue(update2));
        REQUIRE(queue.tryEnqueue(update3));
        
        batchMap.clear();
        size_t processed = queue.processBatch(batchMap);
        REQUIRE(processed == 3);
        REQUIRE(batchMap.size() == 2);
        
        const auto& batch1 = batchMap["element1"];
        REQUIRE(batch1.hasPosition);
        REQUIRE(batch1.hasValue);
        REQUIRE(batch1.position.x == 10);
        REQUIRE(batch1.value == 0.5f);
        
        const auto& batch2 = batchMap["element2"];
        REQUIRE(batch2.hasPosition);
        REQUIRE(!batch2.hasValue);
        REQUIRE(batch2.position.x == 30);
    }
    
    SECTION("High-frequency batching performance") {
        const int NUM_UPDATES = 1000;
        
        // Enqueue many rapid updates
        for (int i = 0; i < NUM_UPDATES; ++i) {
            ui::UIUpdate update(ui::UIUpdate::SET_POSITION, "highfreq");
            update.data.position.x = i;
            update.data.position.y = i * 2;
            REQUIRE(queue.tryEnqueue(update));
        }
        
        batchMap.clear();
        auto start = std::chrono::high_resolution_clock::now();
        size_t processed = queue.processBatch(batchMap);
        auto end = std::chrono::high_resolution_clock::now();
        
        REQUIRE(processed == NUM_UPDATES);
        REQUIRE(batchMap.size() == 1);
        
        // Should coalesce to final values
        const auto& batch = batchMap["highfreq"];
        REQUIRE(batch.position.x == NUM_UPDATES - 1);
        REQUIRE(batch.position.y == (NUM_UPDATES - 1) * 2);
        
        // Performance check - should be very fast
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        REQUIRE(duration.count() < 1000); // Less than 1ms for 1000 updates
    }
    
    SECTION("Empty queue returns zero processed") {
        batchMap.clear();
        size_t processed = queue.processBatch(batchMap);
        REQUIRE(processed == 0);
        REQUIRE(batchMap.empty());
    }
}
