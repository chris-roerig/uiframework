#include <catch2/catch_test_macros.hpp>
#include "uiframework/RealTime/UIUpdateQueue.h"
#include <unordered_map>
#include <chrono>
#include <set>

TEST_CASE("Non-Blocking Element Access System", "[nonblocking]") {
    ui::UIUpdateQueue queue;
    std::unordered_map<std::string, ui::BatchedUpdate> batchMap;
    std::vector<ui::ElementCache> elementCache;
    
    SECTION("Element cache pre-allocation") {
        // Enqueue updates for multiple elements
        ui::UIUpdate update1(ui::UIUpdate::SET_POSITION, "element1");
        update1.data.position.x = 10;
        update1.data.position.y = 20;
        
        ui::UIUpdate update2(ui::UIUpdate::SET_VALUE, "element2");
        update2.data.floatValue.value = 0.5f;
        
        ui::UIUpdate update3(ui::UIUpdate::SET_TEXT, "element3");
        update3.textValue = "Test Text";
        
        REQUIRE(queue.tryEnqueue(update1));
        REQUIRE(queue.tryEnqueue(update2));
        REQUIRE(queue.tryEnqueue(update3));
        
        // Process with cache
        size_t processed = queue.processBatchWithCache(batchMap, elementCache);
        REQUIRE(processed == 3);
        REQUIRE(batchMap.size() == 3);
        REQUIRE(elementCache.size() == 3);
        
        // Verify cache entries are pre-allocated
        std::set<std::string> cacheIds;
        for (const auto& cache : elementCache) {
            cacheIds.insert(cache.elementId);
            REQUIRE(!cache.elementId.empty());
            // Element pointers are null until populated by UI thread
            REQUIRE(cache.element == nullptr);
        }
        
        REQUIRE(cacheIds.count("element1") == 1);
        REQUIRE(cacheIds.count("element2") == 1);
        REQUIRE(cacheIds.count("element3") == 1);
    }
    
    SECTION("Cache efficiency with repeated elements") {
        // Multiple updates to same elements
        for (int i = 0; i < 10; ++i) {
            ui::UIUpdate update(ui::UIUpdate::SET_POSITION, "element1");
            update.data.position.x = i * 10;
            update.data.position.y = i * 20;
            REQUIRE(queue.tryEnqueue(update));
        }
        
        for (int i = 0; i < 5; ++i) {
            ui::UIUpdate update(ui::UIUpdate::SET_VALUE, "element2");
            update.data.floatValue.value = i * 0.1f;
            REQUIRE(queue.tryEnqueue(update));
        }
        
        batchMap.clear();
        elementCache.clear();
        size_t processed = queue.processBatchWithCache(batchMap, elementCache);
        
        REQUIRE(processed == 15); // Total updates processed
        REQUIRE(batchMap.size() == 2); // Only 2 unique elements
        REQUIRE(elementCache.size() == 2); // Cache matches unique elements
        
        // Verify final coalesced values
        REQUIRE(batchMap["element1"].hasPosition);
        REQUIRE(batchMap["element1"].position.x == 90); // Last value
        REQUIRE(batchMap["element1"].position.y == 180);
        
        REQUIRE(batchMap["element2"].hasValue);
        REQUIRE(batchMap["element2"].value == 0.4f); // Last value
    }
    
    SECTION("Cache move semantics") {
        ui::UIUpdate update(ui::UIUpdate::SET_TEXT, "test_element");
        update.textValue = "Move Test";
        REQUIRE(queue.tryEnqueue(update));
        
        batchMap.clear();
        elementCache.clear();
        queue.processBatchWithCache(batchMap, elementCache);
        
        REQUIRE(elementCache.size() == 1);
        
        // Test move constructor
        ui::ElementCache moved = std::move(elementCache[0]);
        REQUIRE(moved.elementId == "test_element");
        REQUIRE(moved.valid == true);
        
        // Original should be in moved-from state (either empty or unchanged)
        bool isMovedFrom = elementCache[0].elementId.empty() || 
                          (elementCache[0].elementId == "test_element");
        REQUIRE(isMovedFrom);
    }
    
    SECTION("Performance: Cache vs individual lookups") {
        const int NUM_ELEMENTS = 100;
        const int UPDATES_PER_ELEMENT = 10;
        
        // Enqueue many updates
        for (int elem = 0; elem < NUM_ELEMENTS; ++elem) {
            for (int update = 0; update < UPDATES_PER_ELEMENT; ++update) {
                ui::UIUpdate upd(ui::UIUpdate::SET_POSITION, "element_" + std::to_string(elem));
                upd.data.position.x = update * 10;
                upd.data.position.y = update * 20;
                REQUIRE(queue.tryEnqueue(upd));
            }
        }
        
        batchMap.clear();
        elementCache.clear();
        
        auto start = std::chrono::high_resolution_clock::now();
        size_t processed = queue.processBatchWithCache(batchMap, elementCache);
        auto end = std::chrono::high_resolution_clock::now();
        
        REQUIRE(processed == NUM_ELEMENTS * UPDATES_PER_ELEMENT);
        REQUIRE(batchMap.size() == NUM_ELEMENTS);
        REQUIRE(elementCache.size() == NUM_ELEMENTS);
        
        // Performance check - should be very fast
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        REQUIRE(duration.count() < 5000); // Less than 5ms for 1000 updates
        
        // Verify cache is properly sized and populated
        for (const auto& cache : elementCache) {
            REQUIRE(!cache.elementId.empty());
            REQUIRE(cache.elementId.find("element_") == 0);
        }
    }
    
    SECTION("Memory efficiency") {
        // Test that cache doesn't grow beyond batch size
        ui::UIUpdate update1(ui::UIUpdate::SET_POSITION, "elem1");
        ui::UIUpdate update2(ui::UIUpdate::SET_VALUE, "elem2");
        ui::UIUpdate update3(ui::UIUpdate::SET_TEXT, "elem3");
        
        REQUIRE(queue.tryEnqueue(update1));
        REQUIRE(queue.tryEnqueue(update2));
        REQUIRE(queue.tryEnqueue(update3));
        
        batchMap.clear();
        elementCache.clear();
        queue.processBatchWithCache(batchMap, elementCache);
        
        size_t initialCapacity = elementCache.capacity();
        
        // Process again with same elements
        REQUIRE(queue.tryEnqueue(update1));
        REQUIRE(queue.tryEnqueue(update2));
        
        batchMap.clear();
        elementCache.clear();
        queue.processBatchWithCache(batchMap, elementCache);
        
        // Cache should be efficiently reused
        REQUIRE(elementCache.size() == 2);
        REQUIRE(elementCache.capacity() >= initialCapacity); // No unnecessary reallocations
    }
    
    SECTION("Empty queue handling") {
        batchMap.clear();
        elementCache.clear();
        
        size_t processed = queue.processBatchWithCache(batchMap, elementCache);
        REQUIRE(processed == 0);
        REQUIRE(batchMap.empty());
        REQUIRE(elementCache.empty());
    }
}
