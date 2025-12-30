#include "uiframework/UI.h"
#include <thread>
#include <chrono>
#include <atomic>
#include <iostream>
#include <random>

/**
 * @brief Enhanced real-time demo showcasing high-frequency update batching
 * 
 * This demo simulates multiple high-frequency data sources updating UI elements
 * simultaneously, demonstrating the batching system's efficiency in handling
 * thousands of updates per second while maintaining smooth UI performance.
 */

int main() {
    try {
        UI ui("Real-Time Batching Demo", 1000, 700);
        
        // Create UI elements for different data streams
        auto titleLabel = ui.createLabel("High-Frequency Update Batching Demo", 10, 10);
        auto statsLabel = ui.createLabel("Initializing...", 10, 40);
        
        // Data visualization elements
        auto dataSlider1 = ui.createHSlider(50, 100, 300, 20, 0.0f, 100.0f, 0.0f);
        auto dataLabel1 = ui.createLabel("Data Stream 1: 0.0", 360, 105);
        
        auto dataSlider2 = ui.createHSlider(50, 150, 300, 20, 0.0f, 100.0f, 0.0f);
        auto dataLabel2 = ui.createLabel("Data Stream 2: 0.0", 360, 155);
        
        auto dataSlider3 = ui.createHSlider(50, 200, 300, 20, 0.0f, 100.0f, 0.0f);
        auto dataLabel3 = ui.createLabel("Data Stream 3: 0.0", 360, 205);
        
        // Progress bars for system metrics
        auto cpuProgress = ui.createProgressBar(50, 270, 200, 20, 0.0f);
        auto memProgress = ui.createProgressBar(50, 310, 200, 20, 0.0f);
        auto netProgress = ui.createProgressBar(50, 350, 200, 20, 0.0f);
        
        auto cpuLabel = ui.createLabel("CPU: 0%", 260, 275);
        auto memLabel = ui.createLabel("Memory: 0%", 260, 315);
        auto netLabel = ui.createLabel("Network: 0%", 260, 355);
        
        // Performance monitoring
        auto perfLabel = ui.createLabel("Performance: Measuring...", 10, 400);
        auto batchLabel = ui.createLabel("Batching: Enabled", 10, 430);
        
        // Control elements
        auto startButton = ui.createButton("Start High-Frequency Updates", 50, 500, [](){});
        auto stopButton = ui.createButton("Stop Updates", 250, 500, [](){});
        
        // Simulation state
        std::atomic<bool> running{false};
        std::atomic<int> updatesPerSecond{0};
        std::atomic<int> batchedUpdates{0};
        
        // High-frequency data simulation threads
        std::vector<std::thread> dataThreads;
        
        // Button callbacks
        startButton->setCallback([&]() {
            if (running.load()) return;
            
            running.store(true);
            updatesPerSecond.store(0);
            batchedUpdates.store(0);
            
            // Data stream 1: Sine wave (audio-like)
            dataThreads.emplace_back([&]() {
                std::random_device rd;
                std::mt19937 gen(rd());
                std::uniform_real_distribution<> noise(-2.0, 2.0);
                
                auto startTime = std::chrono::high_resolution_clock::now();
                int updateCount = 0;
                
                while (running.load()) {
                    auto now = std::chrono::high_resolution_clock::now();
                    auto elapsed = std::chrono::duration<double>(now - startTime).count();
                    
                    // 440Hz sine wave with noise (audio frequency)
                    float value = 50.0f + 30.0f * std::sin(2.0 * M_PI * 440.0 * elapsed) + noise(gen);
                    value = std::max(0.0f, std::min(100.0f, value));
                    
                    ui.realtimeSetValue(dataSlider1->getId(), value);
                    ui.realtimeSetText(dataLabel1->getId(), "Data Stream 1: " + std::to_string(value));
                    
                    updateCount += 2;
                    updatesPerSecond.store(updateCount);
                    
                    std::this_thread::sleep_for(std::chrono::microseconds(100)); // 10kHz
                }
            });
            
            // Data stream 2: Sawtooth wave (control data)
            dataThreads.emplace_back([&]() {
                auto startTime = std::chrono::high_resolution_clock::now();
                
                while (running.load()) {
                    auto now = std::chrono::high_resolution_clock::now();
                    auto elapsed = std::chrono::duration<double>(now - startTime).count();
                    
                    // 2Hz sawtooth
                    float value = 100.0f * (std::fmod(elapsed * 2.0, 1.0));
                    
                    ui.realtimeSetValue(dataSlider2->getId(), value);
                    ui.realtimeSetText(dataLabel2->getId(), "Data Stream 2: " + std::to_string(value));
                    
                    std::this_thread::sleep_for(std::chrono::microseconds(500)); // 2kHz
                }
            });
            
            // Data stream 3: Random walk (sensor data)
            dataThreads.emplace_back([&]() {
                std::random_device rd;
                std::mt19937 gen(rd());
                std::uniform_real_distribution<> walk(-1.0, 1.0);
                
                float value = 50.0f;
                
                while (running.load()) {
                    value += walk(gen);
                    value = std::max(0.0f, std::min(100.0f, value));
                    
                    ui.realtimeSetValue(dataSlider3->getId(), value);
                    ui.realtimeSetText(dataLabel3->getId(), "Data Stream 3: " + std::to_string(value));
                    
                    std::this_thread::sleep_for(std::chrono::milliseconds(1)); // 1kHz
                }
            });
            
            // System metrics simulation
            dataThreads.emplace_back([&]() {
                std::random_device rd;
                std::mt19937 gen(rd());
                std::uniform_real_distribution<> cpu(20.0, 80.0);
                std::uniform_real_distribution<> mem(30.0, 70.0);
                std::uniform_real_distribution<> net(10.0, 90.0);
                
                while (running.load()) {
                    ui.realtimeSetValue(cpuProgress->getId(), cpu(gen));
                    ui.realtimeSetValue(memProgress->getId(), mem(gen));
                    ui.realtimeSetValue(netProgress->getId(), net(gen));
                    
                    std::this_thread::sleep_for(std::chrono::milliseconds(50)); // 20Hz
                }
            });
        });
        
        stopButton->setCallback([&]() {
            running.store(false);
            for (auto& thread : dataThreads) {
                if (thread.joinable()) {
                    thread.join();
                }
            }
            dataThreads.clear();
        });
        
        // Performance monitoring thread
        std::thread perfThread([&]() {
            auto lastTime = std::chrono::high_resolution_clock::now();
            int lastUpdates = 0;
            
            while (true) {
                std::this_thread::sleep_for(std::chrono::seconds(1));
                
                auto now = std::chrono::high_resolution_clock::now();
                int currentUpdates = updatesPerSecond.load();
                
                int updatesThisSecond = currentUpdates - lastUpdates;
                lastUpdates = currentUpdates;
                
                // Update performance stats on UI thread
                ui.realtimeSetText(statsLabel->getId(), 
                    "Updates/sec: " + std::to_string(updatesThisSecond) + 
                    " | Total: " + std::to_string(currentUpdates));
                
                ui.realtimeSetText(perfLabel->getId(), 
                    "Performance: " + std::to_string(updatesThisSecond) + " ops/sec");
                
                if (!running.load() && dataThreads.empty()) {
                    break;
                }
            }
        });
        
        std::cout << "\\n=== Real-Time Batching Demo ===" << std::endl;
        std::cout << "This demo showcases high-frequency update batching:" << std::endl;
        std::cout << "• Multiple data streams updating at different frequencies" << std::endl;
        std::cout << "• Batching system coalesces redundant updates" << std::endl;
        std::cout << "• Maintains smooth UI performance under high load" << std::endl;
        std::cout << "• Click 'Start' to begin simulation" << std::endl;
        std::cout << "\\nPress Ctrl+C or close window to exit\\n" << std::endl;
        
        ui.run();
        
        // Cleanup
        running.store(false);
        for (auto& thread : dataThreads) {
            if (thread.joinable()) {
                thread.join();
            }
        }
        if (perfThread.joinable()) {
            perfThread.join();
        }
        
    } catch (const ui::UIException& e) {
        std::cerr << "UI Error: " << e.what() << std::endl;
        return -1;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    
    return 0;
}
