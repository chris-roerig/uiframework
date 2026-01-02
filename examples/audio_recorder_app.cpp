#include "uiframework/UI.h"
#include <iostream>
#include <cmath>

int main() {
    try {
        UI ui("Audio Recorder - Based on Wireframe Layout", 1024, 600);
        
        std::cout << "Audio Recorder App Started!" << std::endl;
        std::cout << "This app demonstrates the wireframe layout from build/wireframe.json" << std::endl;
        
        // Recreate the exact layout from wireframe.json (using relative coordinates)
        
        // Record button (ToggleRecordBtn) - x:880, y:530
        auto recordButton = ui.createButton("Start Recording", 880, 530, [](){
            std::cout << "Recording toggled!" << std::endl;
        });
        recordButton->setSize(120, 40);
        
        // Left VU Label - x:0, y:340
        ui.createLabel("L", 0, 340);
        
        // Main waveform canvas (Canvas1) - x:10, y:10
        auto waveformCanvas = ui.createCanvas(10, 10, 1010, 320);
        waveformCanvas->filledRect({0, 0, 1010, 320}, ui::Color(10, 10, 10, 255));
        // Draw center line
        waveformCanvas->filledRect({0, 160, 1010, 1}, ui::Color(80, 80, 80, 255));
        // Draw sample waveform
        for (int i = 0; i < 500; i += 20) {
            int y = 160 + (int)(50 * sin(i * 0.02));
            waveformCanvas->filledRect({i * 2, y, 4, 4}, ui::Color(0, 255, 100, 255));
        }
        
        // Left VU Meter (Canvas2) - x:10, y:360
        auto leftVUMeter = ui.createCanvas(10, 360, 40, 200);
        leftVUMeter->filledRect({0, 0, 40, 200}, ui::Color(20, 20, 20, 255));
        // Draw sample level (60%)
        leftVUMeter->filledRect({5, 80, 30, 120}, ui::Color(0, 255, 0, 255));
        
        // Right VU Meter (RightVULabl) - x:70, y:360
        auto rightVUMeter = ui.createCanvas(70, 360, 40, 200);
        rightVUMeter->filledRect({0, 0, 40, 200}, ui::Color(20, 20, 20, 255));
        // Draw sample level (45%)
        rightVUMeter->filledRect({5, 110, 30, 90}, ui::Color(0, 255, 0, 255));
        
        // Threshold slider (ThreshSlider) - x:50, y:350
        auto thresholdSlider = ui.createVSlider(50, 350, 20, 220, 0.0f, 1.0f, 0.3f);
        
        // Threshold label (threshholdLabl) - x:160, y:360
        ui.createLabel("Threshold Record", 160, 360);
        
        // Threshold mode selector (ThresholdCycleList) - x:270, y:350
        auto thresholdMode = ui.createCycleList(270, 350, 100, 30, {"Manual", "Auto", "Voice"});
        
        // Add some additional labels for clarity
        ui.createLabel("Audio Recorder Interface", 10, 0);
        ui.createLabel("VU Meters", 10, 340);
        
        // Instructions
        std::cout << "Interface Elements:" << std::endl;
        std::cout << "- Large waveform display canvas (1010x320)" << std::endl;
        std::cout << "- Left and Right VU meters with sample levels" << std::endl;
        std::cout << "- Vertical threshold slider" << std::endl;
        std::cout << "- Threshold mode selector (Manual/Auto/Voice)" << std::endl;
        std::cout << "- Record button (Start/Stop Recording)" << std::endl;
        std::cout << "- Window size: 1024x600 (matches layout editor canvas)" << std::endl;
        
        ui.run();
        
    } catch (const ui::UIException& e) {
        std::cerr << "UI Error: " << e.what() << std::endl;
        return -1;
    }
    
    return 0;
}
