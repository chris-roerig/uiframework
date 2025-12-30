#include "uiframework/UI.h"
#include <iostream>
#include <chrono>
#include <thread>

/**
 * @brief Example demonstrating element pooling for real-time audio instrument UI
 * 
 * This example simulates high-frequency UI updates typical in audio instruments:
 * - VU meter segments updating at 60 FPS
 * - Parameter value displays changing in real-time
 * - Trigger indicators for pad hits
 */

int main() {
    try {
        UI ui("Audio Instrument - Element Pooling Demo", 1024, 768);
        
        // Enable element pooling for high-frequency updates
        ui.enableElementPooling(100, 50);  // 100 labels, 50 buttons
        
        std::cout << "Element pooling enabled for audio instrument UI\n";
        
        // Create some persistent UI elements
        auto titleLabel = ui.createLabel("MPC-Style Audio Instrument", 10, 10);
        auto statusLabel = ui.createLabel("Status: Ready", 10, 50);
        
        // Simulate real-time audio instrument behavior
        std::vector<std::shared_ptr<ui::Label>> vuMeters;
        std::vector<std::shared_ptr<ui::Button>> padTriggers;
        
        // Create VU meter segments using pooled elements
        for (int channel = 0; channel < 8; ++channel) {
            for (int segment = 0; segment < 16; ++segment) {
                auto vuSegment = ui.createLabelPooled("█", 
                    100 + channel * 60, 100 + segment * 20);
                vuMeters.push_back(vuSegment);
            }
        }
        
        // Create pad trigger indicators using pooled elements
        for (int pad = 0; pad < 16; ++pad) {
            auto trigger = ui.createButtonPooled("PAD " + std::to_string(pad + 1),
                200 + (pad % 4) * 120, 400 + (pad / 4) * 60,
                [pad]() {
                    std::cout << "Pad " << (pad + 1) << " triggered!\n";
                });
            padTriggers.push_back(trigger);
        }
        
        std::cout << "Created " << vuMeters.size() << " VU meter segments\n";
        std::cout << "Created " << padTriggers.size() << " pad triggers\n";
        
        // Show pool statistics
        auto stats = ui.getPoolStats();
        std::cout << "Pool stats - Labels: " << stats.labelsInUse << " in use, " 
                  << stats.labelsAvailable << " available\n";
        std::cout << "Pool stats - Buttons: " << stats.buttonsInUse << " in use, " 
                  << stats.buttonsAvailable << " available\n";
        
        // Simulate high-frequency updates (like real-time audio processing)
        std::thread updateThread([&]() {
            int updateCount = 0;
            while (updateCount < 100) {  // Simulate 100 updates
                
                // Simulate VU meter updates (would be driven by audio levels)
                for (size_t i = 0; i < vuMeters.size(); ++i) {
                    int level = rand() % 16;  // Random audio level
                    if (i % 16 < level) {
                        vuMeters[i]->setText("█");  // Active segment
                    } else {
                        vuMeters[i]->setText("░");  // Inactive segment
                    }
                }
                
                // Update status
                statusLabel->setText("Status: Processing - Update " + std::to_string(updateCount));
                
                updateCount++;
                std::this_thread::sleep_for(std::chrono::milliseconds(16));  // ~60 FPS
            }
            
            std::cout << "Completed " << updateCount << " high-frequency updates\n";
            std::cout << "In a real audio instrument, this would be driven by audio callbacks\n";
        });
        
        // Run for a short demo
        std::thread demoTimer([&]() {
            std::this_thread::sleep_for(std::chrono::seconds(3));
            std::cout << "\nDemo complete! In a real application:\n";
            std::cout << "- VU meters would update based on actual audio levels\n";
            std::cout << "- Pad triggers would respond to MIDI or audio input\n";
            std::cout << "- Element pooling eliminates allocation overhead\n";
            std::cout << "- No audio dropouts from UI memory allocation\n";
            exit(0);
        });
        
        updateThread.join();
        demoTimer.join();
        
        ui.run();
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    
    return 0;
}
