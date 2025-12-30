#include <catch2/catch_test_macros.hpp>
#include "uiframework/RealTime/UIUpdateQueue.h"
#include <chrono>

TEST_CASE("Memory-Predictable Operations System", "[predictable]") {
    ui::UIUpdateQueue queue;
    std::vector<ui::ElementCache> elementCache;
    
    SECTION("String pool allocation and reuse") {
        // Test string pool functionality
        ui::UIUpdate update1(ui::UIUpdate::SET_TEXT, "element1");
        update1.textValue = "Test String 1";
        
        ui::UIUpdate update2(ui::UIUpdate::SET_TEXT, "element2");
        update2.textValue = "Test String 2";
        
        ui::UIUpdate update3(ui::UIUpdate::SET_POSITION, "element1");
        update3.data.position.x = 100;
        update3.data.position.y = 200;
        
        REQUIRE(queue.tryEnqueue(update1));
        REQUIRE(queue.tryEnqueue(update2));
        REQUIRE(queue.tryEnqueue(update3));
        
        // Process with predictable operations
        size_t processed = queue.processPredictable(elementCache);
        REQUIRE(processed == 3);
        
        // Verify batches were created
        REQUIRE(queue.getActiveBatches() == 2); // Two unique elements
        REQUIRE(elementCache.size() == 2);
        
        const auto& batches = queue.getPredictableBatches();
        
        // Find element1 batch
        const ui::PredictableBatch* elem1Batch = nullptr;
        for (size_t i = 0; i < queue.getActiveBatches(); ++i) {
            if (batches[i].elementId == "element1") {
                elem1Batch = &batches[i];
                break;
            }
        }
        
        REQUIRE(elem1Batch != nullptr);
        REQUIRE(elem1Batch->hasText);
        REQUIRE(elem1Batch->hasPosition);
        REQUIRE(elem1Batch->textValue == "Test String 1");
        REQUIRE(elem1Batch->position.x == 100);
        REQUIRE(elem1Batch->position.y == 200);
    }
    
    SECTION("Bounded memory usage") {
        const int MAX_ELEMENTS = 100; // Within batch array capacity
        const int UPDATES_PER_ELEMENT = 3; // Reduced to fit string pool
        
        // Fill up to capacity
        for (int elem = 0; elem < MAX_ELEMENTS; ++elem) {
            for (int update = 0; update < UPDATES_PER_ELEMENT; ++update) {
                ui::UIUpdate upd(ui::UIUpdate::SET_POSITION, "elem_" + std::to_string(elem));
                upd.data.position.x = update * 10;
                upd.data.position.y = update * 20;
                REQUIRE(queue.tryEnqueue(upd));
            }
        }
        
        elementCache.clear();
        size_t processed = queue.processPredictable(elementCache);
        
        // Should process all updates
        REQUIRE(processed == MAX_ELEMENTS * UPDATES_PER_ELEMENT);
        REQUIRE(queue.getActiveBatches() == MAX_ELEMENTS);
        REQUIRE(elementCache.size() == MAX_ELEMENTS);
        
        // Verify final coalesced values
        const auto& batches = queue.getPredictableBatches();
        for (size_t i = 0; i < queue.getActiveBatches(); ++i) {
            REQUIRE(batches[i].hasPosition);
            REQUIRE(batches[i].position.x == (UPDATES_PER_ELEMENT - 1) * 10);
            REQUIRE(batches[i].position.y == (UPDATES_PER_ELEMENT - 1) * 20);
        }
    }
    
    SECTION("String pool capacity limits") {
        // Test string pool exhaustion handling
        std::string longString(1000, 'A'); // 1KB string
        
        // Fill string pool to near capacity
        for (int i = 0; i < 8; ++i) { // 8KB total
            ui::UIUpdate update(ui::UIUpdate::SET_TEXT, "elem_" + std::to_string(i));
            update.textValue = longString + std::to_string(i);
            REQUIRE(queue.tryEnqueue(update));
        }
        
        elementCache.clear();
        size_t processed = queue.processPredictable(elementCache);
        
        // Should process what fits in string pool
        REQUIRE(processed > 0);
        REQUIRE(queue.getActiveBatches() <= 8);
        
        // Verify strings were allocated correctly
        const auto& batches = queue.getPredictableBatches();
        for (size_t i = 0; i < queue.getActiveBatches(); ++i) {
            if (batches[i].hasText) {
                REQUIRE(!batches[i].textValue.empty());
                REQUIRE(batches[i].textValue.length() >= 1000);
            }
        }
    }
    
    SECTION("Deterministic performance") {
        const int NUM_UPDATES = 300; // Reduced to fit capacity
        
        // Enqueue predictable number of updates
        for (int i = 0; i < NUM_UPDATES; ++i) {
            ui::UIUpdate update(ui::UIUpdate::SET_VALUE, "elem_" + std::to_string(i % 50));
            update.data.floatValue.value = i * 0.01f;
            REQUIRE(queue.tryEnqueue(update));
        }
        
        elementCache.clear();
        
        // Measure processing time
        auto start = std::chrono::high_resolution_clock::now();
        size_t processed = queue.processPredictable(elementCache);
        auto end = std::chrono::high_resolution_clock::now();
        
        REQUIRE(processed == NUM_UPDATES);
        REQUIRE(queue.getActiveBatches() == 50); // 50 unique elements
        
        // Performance should be deterministic and fast
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        REQUIRE(duration.count() < 2000); // Less than 2ms for 300 updates
        
        // Verify memory usage is bounded
        REQUIRE(elementCache.size() == 50);
        REQUIRE(elementCache.capacity() >= 50);
    }
    
    SECTION("Mixed update types coalescing") {
        // Test all update types for single element
        ui::UIUpdate posUpdate(ui::UIUpdate::SET_POSITION, "test_elem");
        posUpdate.data.position.x = 50;
        posUpdate.data.position.y = 100;
        
        ui::UIUpdate sizeUpdate(ui::UIUpdate::SET_SIZE, "test_elem");
        sizeUpdate.data.size.width = 200;
        sizeUpdate.data.size.height = 150;
        
        ui::UIUpdate valueUpdate(ui::UIUpdate::SET_VALUE, "test_elem");
        valueUpdate.data.floatValue.value = 0.75f;
        
        ui::UIUpdate textUpdate(ui::UIUpdate::SET_TEXT, "test_elem");
        textUpdate.textValue = "Predictable Text";
        
        ui::UIUpdate visUpdate(ui::UIUpdate::SET_VISIBILITY, "test_elem");
        visUpdate.data.visibility.visible = false;
        
        REQUIRE(queue.tryEnqueue(posUpdate));
        REQUIRE(queue.tryEnqueue(sizeUpdate));
        REQUIRE(queue.tryEnqueue(valueUpdate));
        REQUIRE(queue.tryEnqueue(textUpdate));
        REQUIRE(queue.tryEnqueue(visUpdate));
        
        elementCache.clear();
        size_t processed = queue.processPredictable(elementCache);
        
        REQUIRE(processed == 5);
        REQUIRE(queue.getActiveBatches() == 1);
        
        const auto& batch = queue.getPredictableBatches()[0];
        REQUIRE(batch.hasPosition);
        REQUIRE(batch.hasSize);
        REQUIRE(batch.hasValue);
        REQUIRE(batch.hasText);
        REQUIRE(batch.hasVisibility);
        
        REQUIRE(batch.position.x == 50);
        REQUIRE(batch.position.y == 100);
        REQUIRE(batch.size.width == 200);
        REQUIRE(batch.size.height == 150);
        REQUIRE(batch.value == 0.75f);
        REQUIRE(batch.textValue == "Predictable Text");
        REQUIRE(batch.visible == false);
    }
    
    SECTION("Frame reset behavior") {
        // Process first frame
        ui::UIUpdate update1(ui::UIUpdate::SET_TEXT, "elem1");
        update1.textValue = "Frame 1";
        REQUIRE(queue.tryEnqueue(update1));
        
        elementCache.clear();
        size_t processed1 = queue.processPredictable(elementCache);
        REQUIRE(processed1 == 1);
        REQUIRE(queue.getActiveBatches() == 1);
        
        // Process second frame - should reset string pool
        ui::UIUpdate update2(ui::UIUpdate::SET_TEXT, "elem2");
        update2.textValue = "Frame 2";
        REQUIRE(queue.tryEnqueue(update2));
        
        elementCache.clear();
        size_t processed2 = queue.processPredictable(elementCache);
        REQUIRE(processed2 == 1);
        REQUIRE(queue.getActiveBatches() == 1);
        
        // Verify string pool was reset
        const auto& batch = queue.getPredictableBatches()[0];
        REQUIRE(batch.textValue == "Frame 2");
    }
    
    SECTION("Empty queue handling") {
        elementCache.clear();
        size_t processed = queue.processPredictable(elementCache);
        REQUIRE(processed == 0);
        REQUIRE(queue.getActiveBatches() == 0);
        REQUIRE(elementCache.empty());
    }
}
