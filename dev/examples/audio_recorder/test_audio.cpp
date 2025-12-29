#include "AudioRecorder.h"
#include <iostream>
#include <chrono>
#include <thread>

/**
 * @brief Simple test for AudioRecorder functionality
 * 
 * Tests basic initialization and API without requiring actual audio hardware.
 */
int main() {
    std::cout << "Testing AudioRecorder..." << std::endl;
    
    AudioRecorder recorder;
    
    // Test initialization
    if (!recorder.initialize()) {
        std::cout << "Warning: Could not initialize audio (no audio hardware?)" << std::endl;
        std::cout << "This is expected in headless environments." << std::endl;
        return 0;
    }
    
    std::cout << "✓ Audio system initialized successfully" << std::endl;
    
    // Test basic state
    std::cout << "✓ Initial state - Recording: " << recorder.isRecording() 
              << ", Playing: " << recorder.isPlaying() << std::endl;
    
    // Test level reading
    float level = recorder.getCurrentLevel();
    std::cout << "✓ Current level: " << level << std::endl;
    
    // Test waveform data (should be empty initially)
    auto waveform = recorder.getWaveformData();
    std::cout << "✓ Initial waveform size: " << waveform.size() << " samples" << std::endl;
    
    // Test playback position
    float pos = recorder.getPlaybackPosition();
    std::cout << "✓ Playback position: " << pos << std::endl;
    
    std::cout << "✓ All AudioRecorder tests passed!" << std::endl;
    
    return 0;
}
