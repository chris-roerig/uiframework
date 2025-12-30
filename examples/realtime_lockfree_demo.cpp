#include "uiframework/UI.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <atomic>
#include <cmath>

/**
 * @brief Real-Time UI Demo - Lock-Free Updates
 * 
 * Demonstrates Phase 1: Lock-Free UI Update Queue for high-frequency applications.
 * Simulates a real-time thread updating UI elements without blocking.
 * Use cases: Audio instruments, games, data visualization, live monitoring.
 */

int main() {
    try {
        UI ui("Real-Time UI Demo - Lock-Free Updates", 800, 600);
        
        // Create UI elements for real-time application interface
        auto title = ui.createLabel("⚡ Real-Time UI Framework - Phase 1 Demo", 10, 10);
        auto status = ui.createLabel("Status: Initializing...", 10, 50);
        
        // Data meters (simulated with progress bars)
        auto leftLabel = ui.createLabel("Data A", 50, 100);
        auto leftMeter = ui.createProgressBar(100, 100, 200, 20, 0.0f, false);
        
        auto rightLabel = ui.createLabel("Data B", 50, 130);
        auto rightMeter = ui.createProgressBar(100, 130, 200, 20, 0.0f, false);
        
        // Real-time controls
        auto pauseButton = ui.createButton("Pause", 320, 100, [](){
            std::cout << "Pause toggled (real-time safe callback)" << std::endl;
        });
        
        auto scaleLabel = ui.createLabel("Scale: 0.75", 320, 140);
        
        // Performance counter
        auto perfLabel = ui.createLabel("Updates: 0", 10, 200);
        
        std::atomic<bool> realtimeRunning{true};
        std::atomic<int> updateCount{0};
        
        // Simulate real-time thread with high-frequency updates
        std::thread realtimeThread([&]() {
            auto startTime = std::chrono::steady_clock::now();
            
            while (realtimeRunning) {
                auto now = std::chrono::steady_clock::now();
                auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime).count();
                
                // Simulate data streams (sine waves)
                float dataA = (std::sin(elapsed * 0.003f) + 1.0f) * 0.5f;
                float dataB = (std::cos(elapsed * 0.002f) + 1.0f) * 0.5f;
                
                // Lock-free updates to UI (real-time thread safe!)
                bool success = true;
                success &= ui.realtimeSetValue(leftMeter->getId(), dataA);
                success &= ui.realtimeSetValue(rightMeter->getId(), dataB);
                
                // Update status every 100ms
                if (elapsed % 100 < 20) {
                    std::string statusText = "Status: Running (" + 
                                           std::to_string(static_cast<int>(dataA * 100)) + "%, " +
                                           std::to_string(static_cast<int>(dataB * 100)) + "%)";
                    success &= ui.realtimeSetText(status->getId(), statusText);
                }
                
                // Update performance counter
                if (success) {
                    int count = updateCount.fetch_add(1);
                    if (count % 50 == 0) {
                        ui.realtimeSetText(perfLabel->getId(), "Updates: " + std::to_string(count));
                    }
                }
                
                // Simulate high-frequency update rate (48kHz equivalent)
                std::this_thread::sleep_for(std::chrono::microseconds(21));
            }
        });
        
        std::cout << "⚡ Real-Time UI Demo Started!" << std::endl;
        std::cout << "📊 Lock-free updates running at high frequency" << std::endl;
        std::cout << "🔧 Phase 1: Lock-Free UI Update Queue - ACTIVE" << std::endl;
        std::cout << "⚡ Zero-blocking real-time thread updates" << std::endl;
        std::cout << "📈 Watch the data meters update in real-time!" << std::endl;
        std::cout << "❌ Close window to exit" << std::endl;
        
        // Run UI (processes real-time updates automatically)
        ui.run();
        
        // Cleanup
        realtimeRunning = false;
        realtimeThread.join();
        
        std::cout << "✅ Demo completed. Total updates: " << updateCount.load() << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "❌ Error: " << e.what() << std::endl;
        return -1;
    }
    
    return 0;
}
