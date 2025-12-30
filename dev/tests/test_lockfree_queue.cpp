#include <catch2/catch_test_macros.hpp>
#include "uiframework/UI.h"
#include "uiframework/RealTime/UIUpdateQueue.h"
#include <thread>
#include <chrono>

TEST_CASE("Lock-free UI update queue", "[audio][lockfree]") {
    
    SECTION("Basic queue operations") {
        ui::UIUpdateQueue queue;
        
        // Test empty queue
        REQUIRE(queue.empty());
        REQUIRE(queue.size() == 0);
        
        // Test enqueue/dequeue
        ui::UIUpdate update(ui::UIUpdate::SET_TEXT, "test_element");
        update.textValue = "Hello World";
        
        REQUIRE(queue.tryEnqueue(update));
        REQUIRE(!queue.empty());
        REQUIRE(queue.size() == 1);
        
        ui::UIUpdate dequeued;
        REQUIRE(queue.tryDequeue(dequeued));
        REQUIRE(dequeued.type == ui::UIUpdate::SET_TEXT);
        REQUIRE(dequeued.elementId == "test_element");
        REQUIRE(dequeued.textValue == "Hello World");
        
        REQUIRE(queue.empty());
    }
    
    SECTION("Real-time safe UI updates") {
        UI ui("Real-Time Test", 800, 600);
        
        auto label = ui.createLabel("Initial", 10, 10);
        auto button = ui.createButton("Click", 10, 50, [](){});
        
        // Test real-time safe text updates
        REQUIRE(ui.realtimeSetText(label->getId(), "Updated"));
        REQUIRE(ui.realtimeSetText(button->getId(), "New Text"));
        
        // Test real-time safe position updates
        REQUIRE(ui.realtimeSetPosition(label->getId(), 100, 200));
        
        // Test real-time safe size updates
        REQUIRE(ui.realtimeSetSize(button->getId(), 150, 40));
        
        // Test real-time safe visibility
        REQUIRE(ui.realtimeSetVisibility(label->getId(), false));
        
        // Process updates (normally called in main loop)
        ui.processRealtimeUpdates();
        
        // Verify updates were applied
        REQUIRE(label->getText() == "Updated");
        REQUIRE(button->getText() == "New Text");
        REQUIRE(label->x == 100);
        REQUIRE(label->y == 200);
        REQUIRE(button->width == 150);
        REQUIRE(button->height == 40);
        REQUIRE(!label->isVisible());
    }
    
    SECTION("High-frequency updates simulation") {
        ui::UIUpdateQueue queue;
        std::atomic<int> enqueueCount{0};
        std::atomic<int> dequeueCount{0};
        
        // Simulate real-time thread producing updates
        std::thread producer([&]() {
            for (int i = 0; i < 1000; ++i) {
                ui::UIUpdate update(ui::UIUpdate::SET_VALUE, "meter_" + std::to_string(i % 10));
                update.data.floatValue.value = static_cast<float>(i % 100) / 100.0f;
                
                while (!queue.tryEnqueue(update)) {
                    std::this_thread::yield(); // Wait if queue full
                }
                enqueueCount++;
                
                // Simulate high-frequency update rate
                std::this_thread::sleep_for(std::chrono::microseconds(20));
            }
        });
        
        // Simulate UI thread consuming updates
        std::thread consumer([&]() {
            ui::UIUpdate update;
            while (dequeueCount < 1000) {
                if (queue.tryDequeue(update)) {
                    dequeueCount++;
                    // Simulate processing time
                    std::this_thread::sleep_for(std::chrono::microseconds(5));
                } else {
                    std::this_thread::yield();
                }
            }
        });
        
        producer.join();
        consumer.join();
        
        REQUIRE(enqueueCount == 1000);
        REQUIRE(dequeueCount == 1000);
        REQUIRE(queue.empty());
    }
}
