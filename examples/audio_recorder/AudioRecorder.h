#pragma once

#include <SDL2/SDL.h>
#include <vector>
#include <atomic>
#include <mutex>
#include <memory>

/**
 * @brief Simple audio recorder using SDL2 audio
 * 
 * Provides basic recording and playback functionality with thread-safe
 * access to audio data for real-time UI updates.
 */
class AudioRecorder {
public:
    AudioRecorder();
    ~AudioRecorder();
    
    bool initialize();
    void cleanup();
    
    void startRecording();
    void stopRecording();
    void startPlayback();
    void stopPlayback();
    
    bool isRecording() const { return recording.load(); }
    bool isPlaying() const { return playing.load(); }
    
    // Thread-safe access to audio data
    std::vector<float> getWaveformData() const;
    float getCurrentLevel() const { return currentLevel.load(); }
    float getPlaybackPosition() const;
    
private:
    static void recordingCallback(void* userdata, Uint8* stream, int len);
    static void playbackCallback(void* userdata, Uint8* stream, int len);
    
    SDL_AudioDeviceID recordingDevice;
    SDL_AudioDeviceID playbackDevice;
    SDL_AudioSpec recordingSpec;
    SDL_AudioSpec playbackSpec;
    
    std::vector<float> audioBuffer;
    mutable std::mutex bufferMutex;
    
    std::atomic<bool> recording{false};
    std::atomic<bool> playing{false};
    std::atomic<float> currentLevel{0.0f};
    std::atomic<size_t> playbackPosition{0};
    
    static constexpr int SAMPLE_RATE = 44100;
    static constexpr int BUFFER_SIZE = 1024;
};
