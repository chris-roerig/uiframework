#include "uiframework/UI.h"
#include "AudioRecorder.h"
#include <iostream>

/**
 * @brief Simplified Audio Recorder Application
 * 
 * Focuses on basic recording/playback functionality with static UI updates.
 */
int main() {
    try {
        std::cout << "Starting Audio Recorder..." << std::endl;
        
        UI ui("Audio Recorder", 800, 600);
        AudioRecorder recorder;
        
        if (!recorder.initialize()) {
            std::cerr << "Failed to initialize audio recorder" << std::endl;
            return -1;
        }
        
        // Title
        auto title = ui.createLabel("Audio Recorder - Real-Time Demo", 20, 20);
        
        // Status label
        auto statusLabel = ui.createLabel("Ready to record", 20, 60);
        
        // Level display
        auto levelLabel = ui.createLabel("Level: 0%", 220, 105);
        
        // Declare buttons first
        std::shared_ptr<ui::Button> recordButton;
        std::shared_ptr<ui::Button> playButton;
        
        // Control buttons
        recordButton = ui.createButton("Record", 20, 100, [&]() {
            if (recorder.isRecording()) {
                recorder.stopRecording();
                recordButton->setText("Record");
                statusLabel->setText("Recording stopped - Ready to play");
                std::cout << "Recording stopped" << std::endl;
            } else {
                recorder.startRecording();
                recordButton->setText("Stop Recording");
                statusLabel->setText("Recording... (Click Stop to finish)");
                std::cout << "Recording started" << std::endl;
            }
        });
        
        playButton = ui.createButton("Play", 120, 100, [&]() {
            if (recorder.isPlaying()) {
                recorder.stopPlayback();
                playButton->setText("Play");
                statusLabel->setText("Playback stopped");
                std::cout << "Playback stopped" << std::endl;
            } else {
                auto waveform = recorder.getWaveformData();
                if (waveform.empty()) {
                    statusLabel->setText("No audio recorded yet");
                    std::cout << "No audio to play" << std::endl;
                } else {
                    recorder.startPlayback();
                    playButton->setText("Stop");
                    statusLabel->setText("Playing back recorded audio");
                    std::cout << "Playback started" << std::endl;
                }
            }
        });
        
        // Simple waveform canvas
        auto waveformCanvas = ui.createCanvas(20, 150, 760, 200);
        auto waveformLabel = ui.createLabel("Waveform Display", 20, 130);
        
        // VU Meter canvas
        auto vuMeterCanvas = ui.createCanvas(20, 400, 150, 100);
        auto vuLabel = ui.createLabel("VU Meter", 20, 380);
        
        // Update button to refresh displays
        auto updateButton = ui.createButton("Update Display", 220, 100, [&]() {
            // Update level
            float level = recorder.getCurrentLevel();
            int levelPercent = static_cast<int>(level * 100);
            levelLabel->setText("Level: " + std::to_string(levelPercent) + "%");
            
            // Update waveform
            auto waveformData = recorder.getWaveformData();
            waveformCanvas->clear();
            
            if (!waveformData.empty()) {
                int width = 760;
                int height = 200;
                int centerY = height / 2;
                
                // Draw center line
                waveformCanvas->lineRel(0, centerY, width, centerY, {100, 100, 100, 255});
                
                // Downsample for display
                int samplesPerPixel = std::max(1, static_cast<int>(waveformData.size()) / width);
                
                for (int x = 0; x < width && x * samplesPerPixel < waveformData.size(); ++x) {
                    float sample = 0.0f;
                    
                    // Average samples for this pixel
                    for (int i = 0; i < samplesPerPixel && (x * samplesPerPixel + i) < waveformData.size(); ++i) {
                        sample += std::abs(waveformData[x * samplesPerPixel + i]);
                    }
                    sample /= samplesPerPixel;
                    
                    int waveHeight = static_cast<int>(sample * centerY);
                    
                    // Draw waveform line
                    waveformCanvas->lineRel(x, centerY - waveHeight, x, centerY + waveHeight, {0, 255, 100, 255});
                }
                
                float duration = waveformData.size() / 44100.0f;
                statusLabel->setText("Recorded " + std::to_string(duration) + "s - Click Update to refresh");
            }
            
            // Update VU meter
            vuMeterCanvas->clear();
            vuMeterCanvas->filledRectRel(5, 5, 140, 90, {50, 50, 50, 255});
            
            int barHeight = static_cast<int>(level * 80);
            int barCount = 10;
            int barWidth = 12;
            
            for (int i = 0; i < barCount; ++i) {
                int barY = 85 - ((i + 1) * 8);
                
                ui::Color color;
                if (i < barCount * 0.7f) {
                    color = {0, 255, 0, 255}; // Green
                } else if (i < barCount * 0.9f) {
                    color = {255, 255, 0, 255}; // Yellow
                } else {
                    color = {255, 0, 0, 255}; // Red
                }
                
                if (barY >= 85 - barHeight) {
                    vuMeterCanvas->filledRectRel(10 + i * 13, barY, barWidth, 6, color);
                }
            }
            
            std::cout << "Display updated - Level: " << levelPercent << "%" << std::endl;
        });
        
        // Instructions
        auto instructions = ui.createLabel("Instructions: Record -> Update Display -> Play", 20, 520);
        
        std::cout << "Audio Recorder ready!" << std::endl;
        std::cout << "- Click Record to start recording" << std::endl;
        std::cout << "- Click Update Display to see waveform and levels" << std::endl;
        std::cout << "- Click Play to hear your recording" << std::endl;
        
        ui.run();
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    
    return 0;
}
