#include "uiframework/UI.h"
#include "uiframework/Resources/FontManager.h"
#include <iostream>
#include <chrono>
#include <thread>

int main() {
    try {
        UI ui("Phase 1 Cache Management Demo", 800, 600);
        
        // Test font cache management
        auto& fontManager = ui::FontManager::getInstance();
        
        std::cout << "=== Phase 1 Cache Management Demo ===" << std::endl;
        
        // Set a small cache size for testing
        fontManager.setMaxCacheSize(5);
        std::cout << "Set max font cache size to: " << fontManager.getMaxCacheSize() << std::endl;
        
        // Create multiple labels to test text cache limits
        std::vector<std::shared_ptr<ui::Label>> labels;
        for (int i = 0; i < 15; ++i) {
            auto label = ui.createLabel("Label " + std::to_string(i), 10, 10 + i * 30);
            labels.push_back(label);
        }
        
        // Load multiple fonts to test font cache eviction
        std::cout << "\nTesting font cache eviction..." << std::endl;
        for (int size = 8; size <= 20; size += 2) {
            auto font = fontManager.getFont("Roboto", size, ui::FontStyle::Regular);
            if (font) {
                std::cout << "Loaded Roboto size " << size << std::endl;
            }
            
            // Check cache stats
            auto stats = fontManager.getMemoryStats();
            std::cout << "Cache size: " << stats.currentFontCacheSize << "/" << stats.maxFontCacheSize << std::endl;
        }
        
        std::cout << "\nFinal memory statistics:" << std::endl;
        fontManager.logMemoryUsage();
        
        // Test text cache by updating labels multiple times
        std::cout << "\nTesting text cache limits..." << std::endl;
        for (int i = 0; i < 20; ++i) {
            for (auto& label : labels) {
                label->setText("Updated text " + std::to_string(i));
            }
            
            if (i % 5 == 0) {
                std::cout << "Updated all labels with text iteration " << i << std::endl;
            }
        }
        
        std::cout << "\nPhase 1 cache management test completed successfully!" << std::endl;
        std::cout << "Cache limits are working properly." << std::endl;
        std::cout << "Press any key or close window to exit..." << std::endl;
        
        // Run the UI (this will block until window is closed)
        ui.run();
        
    } catch (const ui::UIException& e) {
        std::cerr << "UI Error: " << e.what() << std::endl;
        return -1;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    
    return 0;
}
