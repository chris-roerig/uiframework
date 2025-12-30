#include "uiframework/UI.h"
#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <thread>
#include <iomanip>

/**
 * @brief Bulk Operations Demo - Phase 6 Implementation
 * 
 * Demonstrates SIMD-optimized bulk update operations for high-throughput scenarios:
 * - Data visualization with 100+ elements
 * - Real-time parameter sweeps
 * - Bulk animation systems
 * - Performance monitoring dashboards
 */

class BulkOperationsDemo {
private:
    UI ui;
    std::vector<std::shared_ptr<ui::Label>> dataLabels;
    std::vector<std::shared_ptr<ui::ProgressBar>> progressBars;
    std::vector<std::shared_ptr<ui::HSlider>> parameterSliders;
    std::shared_ptr<ui::Label> performanceLabel;
    std::shared_ptr<ui::Label> instructionsLabel;
    
    std::mt19937 rng;
    std::uniform_real_distribution<float> valueDist{0.0f, 1.0f};
    std::uniform_int_distribution<int> positionDist{5, 50};
    
    // Performance tracking
    size_t totalBulkUpdates = 0;
    std::chrono::high_resolution_clock::time_point lastUpdate;
    
public:
    BulkOperationsDemo() : ui("Phase 6: Bulk Operations Demo", 1200, 800), rng(std::random_device{}()) {
        setupUI();
        lastUpdate = std::chrono::high_resolution_clock::now();
    }
    
    void setupUI() {
        // Instructions
        instructionsLabel = ui.createLabel(
            "Phase 6 Bulk Operations Demo - SIMD Optimized Updates\n"
            "SPACE: Bulk update all elements (100+ simultaneous)\n"
            "1: Bulk text updates (data labels)\n"
            "2: Bulk value updates (progress bars)\n"
            "3: Bulk position updates (parameter sliders)\n"
            "4: Mixed bulk operations\n"
            "ESC: Exit", 10, 10
        );
        
        // Performance display
        performanceLabel = ui.createLabel("Performance: Ready", 10, 120);
        
        // Create data visualization elements (100+ elements for bulk testing)
        createDataVisualization();
        createProgressBars();
        createParameterSliders();
        
        std::cout << "Created " << (dataLabels.size() + progressBars.size() + parameterSliders.size()) 
                  << " UI elements for bulk operations testing\n";
    }
    
    void createDataVisualization() {
        // Data labels grid (10x8 = 80 elements)
        for (int row = 0; row < 8; ++row) {
            for (int col = 0; col < 10; ++col) {
                int x = 50 + col * 100;
                int y = 160 + row * 25;
                
                auto label = ui.createLabel("Data " + std::to_string(row * 10 + col), x, y);
                dataLabels.push_back(label);
            }
        }
    }
    
    void createProgressBars() {
        // Progress bars (20 elements)
        for (int i = 0; i < 20; ++i) {
            int x = 50 + (i % 10) * 100;
            int y = 380 + (i / 10) * 30;
            
            auto progress = ui.createProgressBar(x, y, 80, 20);
            progress->setProgress(0.5f);
            progressBars.push_back(progress);
        }
    }
    
    void createParameterSliders() {
        // Parameter sliders (25 elements)
        for (int i = 0; i < 25; ++i) {
            int x = 50 + (i % 5) * 200;
            int y = 450 + (i / 5) * 40;
            
            auto slider = ui.createHSlider(x, y, 150, 0.0f, 1.0f, 0.5f);
            parameterSliders.push_back(slider);
        }
    }
    
    void performBulkTextUpdates() {
        std::vector<std::string> ids;
        std::vector<std::string> texts;
        
        // Prepare bulk data
        for (size_t i = 0; i < dataLabels.size(); ++i) {
            ids.push_back(dataLabels[i]->getId());
            
            float value = valueDist(rng);
            texts.push_back("Val: " + std::to_string(value).substr(0, 5));
        }
        
        // Perform bulk update
        auto start = std::chrono::high_resolution_clock::now();
        size_t result = ui.realtimeBulkSetText(ids, texts);
        auto end = std::chrono::high_resolution_clock::now();
        
        updatePerformanceStats("Bulk Text", result, start, end);
    }
    
    void performBulkValueUpdates() {
        std::vector<std::string> ids;
        std::vector<float> values;
        
        // Prepare bulk data for progress bars
        for (const auto& bar : progressBars) {
            ids.push_back(bar->getId());
            values.push_back(valueDist(rng));
        }
        
        // Perform bulk update
        auto start = std::chrono::high_resolution_clock::now();
        size_t result = ui.realtimeBulkSetValue(ids, values);
        auto end = std::chrono::high_resolution_clock::now();
        
        updatePerformanceStats("Bulk Values", result, start, end);
    }
    
    void performBulkPositionUpdates() {
        std::vector<std::string> ids;
        std::vector<std::pair<int, int>> positions;
        
        // Prepare bulk data for sliders (animate positions)
        for (size_t i = 0; i < parameterSliders.size(); ++i) {
            ids.push_back(parameterSliders[i]->getId());
            
            int baseX = 50 + (i % 5) * 200;
            int baseY = 450 + (i / 5) * 40;
            int offsetX = positionDist(rng) - 25; // Random offset
            int offsetY = positionDist(rng) - 25;
            
            positions.push_back({baseX + offsetX, baseY + offsetY});
        }
        
        // Perform bulk update
        auto start = std::chrono::high_resolution_clock::now();
        size_t result = ui.realtimeBulkSetPosition(ids, positions);
        auto end = std::chrono::high_resolution_clock::now();
        
        updatePerformanceStats("Bulk Positions", result, start, end);
    }
    
    void performMixedBulkOperations() {
        auto start = std::chrono::high_resolution_clock::now();
        
        // Perform multiple bulk operations simultaneously
        size_t textResult = 0, valueResult = 0, positionResult = 0;
        
        // Bulk text updates
        {
            std::vector<std::string> ids;
            std::vector<std::string> texts;
            for (size_t i = 0; i < std::min(dataLabels.size(), size_t(50)); ++i) {
                ids.push_back(dataLabels[i]->getId());
                texts.push_back("Mix " + std::to_string(i));
            }
            textResult = ui.realtimeBulkSetText(ids, texts);
        }
        
        // Bulk value updates
        {
            std::vector<std::string> ids;
            std::vector<float> values;
            for (const auto& bar : progressBars) {
                ids.push_back(bar->getId());
                values.push_back(valueDist(rng));
            }
            valueResult = ui.realtimeBulkSetValue(ids, values);
        }
        
        // Bulk position updates
        {
            std::vector<std::string> ids;
            std::vector<std::pair<int, int>> positions;
            for (size_t i = 0; i < std::min(parameterSliders.size(), size_t(15)); ++i) {
                ids.push_back(parameterSliders[i]->getId());
                int baseX = 50 + (i % 5) * 200;
                int baseY = 450 + (i / 5) * 40;
                positions.push_back({baseX, baseY});
            }
            positionResult = ui.realtimeBulkSetPosition(ids, positions);
        }
        
        auto end = std::chrono::high_resolution_clock::now();
        
        size_t totalResult = textResult + valueResult + positionResult;
        updatePerformanceStats("Mixed Bulk", totalResult, start, end);
    }
    
    void performMassiveBulkUpdate() {
        // Update ALL elements simultaneously (125+ elements)
        auto start = std::chrono::high_resolution_clock::now();
        
        size_t totalUpdates = 0;
        
        // All data labels
        {
            std::vector<std::string> ids;
            std::vector<std::string> texts;
            for (size_t i = 0; i < dataLabels.size(); ++i) {
                ids.push_back(dataLabels[i]->getId());
                texts.push_back("Bulk " + std::to_string(totalBulkUpdates) + ":" + std::to_string(i));
            }
            totalUpdates += ui.realtimeBulkSetText(ids, texts);
        }
        
        // All progress bars
        {
            std::vector<std::string> ids;
            std::vector<float> values;
            for (const auto& bar : progressBars) {
                ids.push_back(bar->getId());
                values.push_back(valueDist(rng));
            }
            totalUpdates += ui.realtimeBulkSetValue(ids, values);
        }
        
        // All sliders
        {
            std::vector<std::string> ids;
            std::vector<float> values;
            for (const auto& slider : parameterSliders) {
                ids.push_back(slider->getId());
                values.push_back(valueDist(rng));
            }
            totalUpdates += ui.realtimeBulkSetValue(ids, values);
        }
        
        auto end = std::chrono::high_resolution_clock::now();
        
        updatePerformanceStats("MASSIVE Bulk", totalUpdates, start, end);
        totalBulkUpdates++;
    }
    
    void updatePerformanceStats(const std::string& operation, size_t elementCount, 
                               std::chrono::high_resolution_clock::time_point start,
                               std::chrono::high_resolution_clock::time_point end) {
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        auto now = std::chrono::high_resolution_clock::now();
        auto timeSinceLastUpdate = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastUpdate);
        
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(2);
        oss << "Performance: " << operation << " - " << elementCount << " elements in " 
            << duration.count() << "μs";
        
        if (elementCount > 0) {
            oss << " (" << (duration.count() / static_cast<double>(elementCount)) << "μs/element)";
        }
        
        oss << " | Rate: " << (1000.0 / timeSinceLastUpdate.count()) << " ops/sec";
        
        performanceLabel->setText(oss.str());
        lastUpdate = now;
        
        std::cout << oss.str() << std::endl;
    }
    
    void handleKeyPress(SDL_Keycode key) {
        switch (key) {
            case SDLK_SPACE:
                performMassiveBulkUpdate();
                break;
            case SDLK_1:
                performBulkTextUpdates();
                break;
            case SDLK_2:
                performBulkValueUpdates();
                break;
            case SDLK_3:
                performBulkPositionUpdates();
                break;
            case SDLK_4:
                performMixedBulkOperations();
                break;
        }
    }
    
    void run() {
        std::cout << "\n=== Phase 6: Bulk Operations Demo ===\n";
        std::cout << "Demonstrating SIMD-optimized bulk updates for high-throughput scenarios\n";
        std::cout << "Total elements: " << (dataLabels.size() + progressBars.size() + parameterSliders.size()) << "\n";
        std::cout << "Press keys 1-4 or SPACE for different bulk operations\n\n";
        
        // Automatic demo mode - perform bulk updates periodically
        auto lastAutoUpdate = std::chrono::high_resolution_clock::now();
        
        // Simple event loop with periodic updates
        bool running = true;
        while (running) {
            SDL_Event event;
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    running = false;
                } else if (event.type == SDL_KEYDOWN) {
                    handleKeyPress(event.key.keysym.sym);
                    if (event.key.keysym.sym == SDLK_ESCAPE) {
                        running = false;
                    }
                }
            }
            
            auto now = std::chrono::high_resolution_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastAutoUpdate);
            
            // Auto-update every 2 seconds
            if (elapsed.count() > 2000) {
                performMassiveBulkUpdate();
                lastAutoUpdate = now;
            }
            
            // Process real-time updates
            ui.processRealtimeUpdates();
            
            // Small delay to prevent busy waiting
            std::this_thread::sleep_for(std::chrono::milliseconds(16)); // ~60 FPS
        }
    }
};

int main() {
    try {
        BulkOperationsDemo demo;
        demo.run();
    } catch (const ui::UIException& e) {
        std::cerr << "Demo error: " << e.what() << std::endl;
        return -1;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    
    return 0;
}
