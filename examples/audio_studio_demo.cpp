#include "uiframework/UI.h"
#include <iostream>
#include <vector>
#include <string>

class AudioStudioDemo {
private:
    UI ui;
    std::vector<std::string> trackList = {
        "Track 1 - Kick.wav",
        "Track 2 - Snare.wav", 
        "Track 3 - HiHat.wav",
        "Track 4 - Bass.wav",
        "Track 5 - Lead.wav",
        "Track 6 - Pad.wav",
        "Track 7 - Vocal.wav",
        "Track 8 - FX.wav"
    };
    
    std::vector<std::string> presets = {
        "Hip Hop Kit",
        "Rock Kit", 
        "Electronic Kit",
        "Jazz Kit",
        "Custom Kit"
    };
    
    std::vector<std::string> effects = {
        "Reverb",
        "Delay", 
        "Chorus",
        "Distortion",
        "Filter"
    };

public:
    AudioStudioDemo() : ui("Audio Studio Demo", 1019, 600) {}
    
    void createInterface() {
        // Left Panel Layout Container
        auto leftPanel = ui.createVBoxLayout(0, 0, 260, 540);
        
        // Top controls in left panel
        auto loadButton = ui.createButton("Load Project", 10, 10, [](){
            std::cout << "Loading project...\n";
        });
        
        auto saveButton = ui.createButton("Save Project", 140, 10, [](){
            std::cout << "Saving project...\n";
        });
        
        // Preset selector
        auto presetSelect = ui.createOptionSelect(0, {"Hip Hop Kit", "Rock Kit", "Electronic Kit", "Jazz Kit", "Custom Kit"}, 
                                                 10, 50, [](int index){
            std::cout << "Selected preset: " << index << "\n";
        });
        
        // Track list
        auto trackListView = ui.createListView(trackList, 10, 90, 240, 440);
        
        // Main canvas area for waveform/visualization
        auto mainCanvas = ui.createCanvas(270, 10, 740, 350);
        
        // Transport controls (play, stop, etc.)
        auto playButton = ui.createButton("▶", 270, 370, [](){
            std::cout << "Play\n";
        });
        
        auto stopButton = ui.createButton("⏹", 310, 370, [](){
            std::cout << "Stop\n";
        });
        
        auto recordButton = ui.createButton("⏺", 350, 370, [](){
            std::cout << "Record\n";
        });
        
        auto loopButton = ui.createButton("🔁", 390, 370, [](){
            std::cout << "Loop toggle\n";
        });
        
        // Drum pad grid (4x4)
        for (int row = 0; row < 4; row++) {
            for (int col = 0; col < 4; col++) {
                int x = 270 + (col * 40);
                int y = 410 + (row * 40);
                int padNum = row * 4 + col + 1;
                
                auto padButton = ui.createButton(std::to_string(padNum), x, y, [padNum](){
                    std::cout << "Drum pad " << padNum << " hit!\n";
                });
                padButton->setSize(30, 30);  // Make pads square
            }
        }
        
        // Mixer sliders (8 channels)
        for (int i = 0; i < 8; i++) {
            int x = 570 + (i % 4) * 30;
            int y = 380 + (i / 4) * 70;
            
            auto slider = ui.createVSlider(x, y, 20, 60, 0.0f, 100.0f, 75.0f);
        }
        
        // Effects selector
        auto effectsSelect = ui.createOptionSelect(0, effects, 460, 370, [](int index){
            std::cout << "Selected effect: " << index << "\n";
        });
        
        // Bottom control bar
        auto bottomPanel = ui.createHBoxLayout(0, 540, 1019, 60);
        
        auto tempoButton = ui.createButton("Tempo: 120", 10, 550, [](){
            std::cout << "Tempo adjustment\n";
        });
        
        auto metronomeButton = ui.createButton("Metronome", 150, 550, [](){
            std::cout << "Metronome toggle\n";
        });
    }
    
    void run() {
        createInterface();
        std::cout << "Audio Studio Demo Started!\n";
        std::cout << "Features:\n";
        std::cout << "- Load/Save projects\n";
        std::cout << "- Track list management\n";
        std::cout << "- Drum pad grid (16 pads)\n";
        std::cout << "- 8-channel mixer\n";
        std::cout << "- Transport controls\n";
        std::cout << "- Effects selection\n";
        std::cout << "- Tempo and metronome controls\n\n";
        
        ui.run();
    }
};

int main() {
    try {
        AudioStudioDemo demo;
        demo.run();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    
    return 0;
}
