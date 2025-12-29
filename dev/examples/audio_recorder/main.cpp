#include "uiframework/UI.h"
#include "AudioRecorder.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <SDL2/SDL.h>

/**
 * @brief Simple Audio Recorder Application
 * 
 * Demonstrates real-time audio recording/playback with UI visualization:
 * - Record audio from microphone
 * - Display waveform in real-time
 * - VU meter showing current audio level
 * - Playback with position indicator
 */
class AudioRecorderApp {
public:
    AudioRecorderApp() : ui("Audio Recorder", 1000, 700) {}
    
    bool initialize() {
        if (!recorder.initialize()) {
            return false;
        }
        
        setupUI();
        return true;
    }
    
    void run() {
        // Run main UI loop - the UI framework handles events and rendering internally
        ui.run();
    }
    
private:
    void setupUI() {
        // Title
        ui.createLabel("Audio Recorder - Real-Time Demo", 20, 20);
        
        // Control buttons
        recordButton = ui.createButton("Record", 20, 60, [this]() {
            if (recorder.isRecording()) {
                recorder.stopRecording();
                recordButton->setText("Record");
            } else {
                recorder.startRecording();
                recordButton->setText("Stop Recording");
            }
        });
        
        playButton = ui.createButton("Play", 120, 60, [this]() {
            if (recorder.isPlaying()) {
                recorder.stopPlayback();
                playButton->setText("Play");
            } else {
                recorder.startPlayback();
                playButton->setText("Stop");
            }
        });
        
        // Status label
        statusLabel = ui.createLabel("Ready to record", 220, 65);
        
        // Waveform canvas
        waveformCanvas = ui.createCanvas(20, 120, 960, 300);
        ui.createLabel("Waveform Display", 20, 100);
        
        // VU Meter canvas
        vuMeterCanvas = ui.createCanvas(20, 480, 200, 150);
        ui.createLabel("VU Meter", 20, 460);
        
        // Level display
        levelLabel = ui.createLabel("Level: 0%", 240, 500);
    }
    
    void updateUI() {
        // This method is no longer used - updates are done in main loop
    }
    
    void updateWaveform() {
        auto waveformData = recorder.getWaveformData();
        if (waveformData.empty()) return;
        
        // Clear canvas
        waveformCanvas->clear();
        
        int width = 960;
        int height = 300;
        int centerY = height / 2;
        
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
        
        // Draw playback position
        if (recorder.isPlaying()) {
            float pos = recorder.getPlaybackPosition();
            int posX = static_cast<int>(pos * width);
            waveformCanvas->lineRel(posX, 0, posX, height, {255, 255, 0, 255});
        }
        
        // Draw center line
        waveformCanvas->lineRel(0, centerY, width, centerY, {100, 100, 100, 255});
    }
    
    void updateVUMeter() {
        float level = recorder.getCurrentLevel();
        
        // Clear VU meter
        vuMeterCanvas->clear();
        
        int width = 200;
        int height = 150;
        
        // Draw VU meter background
        vuMeterCanvas->filledRectRel(10, 10, width - 20, height - 20, {50, 50, 50, 255});
        
        // Draw level bars
        int barHeight = static_cast<int>(level * (height - 40));
        int barCount = 20;
        int barWidth = (width - 40) / barCount;
        
        for (int i = 0; i < barCount; ++i) {
            int barY = height - 20 - ((i + 1) * (height - 40) / barCount);
            
            ui::Color color;
            if (i < barCount * 0.7f) {
                color = {0, 255, 0, 255}; // Green
            } else if (i < barCount * 0.9f) {
                color = {255, 255, 0, 255}; // Yellow
            } else {
                color = {255, 0, 0, 255}; // Red
            }
            
            if (barY >= height - 20 - barHeight) {
                vuMeterCanvas->filledRectRel(15 + i * barWidth, barY, barWidth - 2, (height - 40) / barCount - 2, color);
            }
        }
        
        // Update level label
        int levelPercent = static_cast<int>(level * 100);
        levelLabel->setText("Level: " + std::to_string(levelPercent) + "%");
    }
    
    void updateStatus() {
        if (recorder.isRecording()) {
            statusLabel->setText("Recording... (Click Stop to finish)");
        } else if (recorder.isPlaying()) {
            statusLabel->setText("Playing back recorded audio");
        } else {
            auto waveformData = recorder.getWaveformData();
            if (!waveformData.empty()) {
                float duration = waveformData.size() / 44100.0f;
                statusLabel->setText("Ready - Recorded " + std::to_string(duration) + "s");
            } else {
                statusLabel->setText("Ready to record");
            }
        }
    }
    
    UI ui;
    AudioRecorder recorder;
    
    std::shared_ptr<ui::Button> recordButton;
    std::shared_ptr<ui::Button> playButton;
    std::shared_ptr<ui::Label> statusLabel;
    std::shared_ptr<ui::Label> levelLabel;
    std::shared_ptr<ui::Canvas> waveformCanvas;
    std::shared_ptr<ui::Canvas> vuMeterCanvas;
    
    std::atomic<bool> running{true};
};

int main() {
    try {
        AudioRecorderApp app;
        
        if (!app.initialize()) {
            std::cerr << "Failed to initialize audio recorder" << std::endl;
            return -1;
        }
        
        std::cout << "Audio Recorder started!" << std::endl;
        std::cout << "- Click Record to start recording from microphone" << std::endl;
        std::cout << "- Click Stop Recording when done" << std::endl;
        std::cout << "- Click Play to hear your recording" << std::endl;
        
        app.run();
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    
    return 0;
}
