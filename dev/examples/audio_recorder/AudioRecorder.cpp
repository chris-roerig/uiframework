#include "AudioRecorder.h"
#include <algorithm>
#include <cmath>
#include <iostream>

AudioRecorder::AudioRecorder() 
    : recordingDevice(0), playbackDevice(0) {
}

AudioRecorder::~AudioRecorder() {
    cleanup();
}

bool AudioRecorder::initialize() {
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        std::cerr << "Failed to initialize SDL Audio: " << SDL_GetError() << std::endl;
        return false;
    }
    
    // Setup recording
    SDL_AudioSpec desiredRecordingSpec;
    SDL_zero(desiredRecordingSpec);
    desiredRecordingSpec.freq = SAMPLE_RATE;
    desiredRecordingSpec.format = AUDIO_F32SYS;
    desiredRecordingSpec.channels = 1;
    desiredRecordingSpec.samples = BUFFER_SIZE;
    desiredRecordingSpec.callback = recordingCallback;
    desiredRecordingSpec.userdata = this;
    
    recordingDevice = SDL_OpenAudioDevice(nullptr, 1, &desiredRecordingSpec, &recordingSpec, 0);
    if (recordingDevice == 0) {
        std::cerr << "Failed to open recording device: " << SDL_GetError() << std::endl;
        return false;
    }
    
    // Setup playback
    SDL_AudioSpec desiredPlaybackSpec;
    SDL_zero(desiredPlaybackSpec);
    desiredPlaybackSpec.freq = SAMPLE_RATE;
    desiredPlaybackSpec.format = AUDIO_F32SYS;
    desiredPlaybackSpec.channels = 1;
    desiredPlaybackSpec.samples = BUFFER_SIZE;
    desiredPlaybackSpec.callback = playbackCallback;
    desiredPlaybackSpec.userdata = this;
    
    playbackDevice = SDL_OpenAudioDevice(nullptr, 0, &desiredPlaybackSpec, &playbackSpec, 0);
    if (playbackDevice == 0) {
        std::cerr << "Failed to open playback device: " << SDL_GetError() << std::endl;
        return false;
    }
    
    return true;
}

void AudioRecorder::cleanup() {
    stopRecording();
    stopPlayback();
    
    if (recordingDevice != 0) {
        SDL_CloseAudioDevice(recordingDevice);
        recordingDevice = 0;
    }
    
    if (playbackDevice != 0) {
        SDL_CloseAudioDevice(playbackDevice);
        playbackDevice = 0;
    }
}

void AudioRecorder::startRecording() {
    if (recordingDevice != 0) {
        std::lock_guard<std::mutex> lock(bufferMutex);
        audioBuffer.clear();
        recording = true;
        SDL_PauseAudioDevice(recordingDevice, 0);
    }
}

void AudioRecorder::stopRecording() {
    if (recordingDevice != 0) {
        SDL_PauseAudioDevice(recordingDevice, 1);
        recording = false;
    }
}

void AudioRecorder::startPlayback() {
    if (playbackDevice != 0 && !audioBuffer.empty()) {
        playbackPosition = 0;
        playing = true;
        SDL_PauseAudioDevice(playbackDevice, 0);
    }
}

void AudioRecorder::stopPlayback() {
    if (playbackDevice != 0) {
        SDL_PauseAudioDevice(playbackDevice, 1);
        playing = false;
        playbackPosition = 0;
    }
}

std::vector<float> AudioRecorder::getWaveformData() const {
    std::lock_guard<std::mutex> lock(bufferMutex);
    return audioBuffer;
}

float AudioRecorder::getPlaybackPosition() const {
    if (audioBuffer.empty()) return 0.0f;
    return static_cast<float>(playbackPosition.load()) / audioBuffer.size();
}

void AudioRecorder::recordingCallback(void* userdata, Uint8* stream, int len) {
    AudioRecorder* recorder = static_cast<AudioRecorder*>(userdata);
    if (!recorder->recording.load()) return;
    
    float* samples = reinterpret_cast<float*>(stream);
    int sampleCount = len / sizeof(float);
    
    // Calculate level for VU meter
    float maxLevel = 0.0f;
    for (int i = 0; i < sampleCount; ++i) {
        maxLevel = std::max(maxLevel, std::abs(samples[i]));
    }
    recorder->currentLevel = maxLevel;
    
    // Store samples in buffer
    std::lock_guard<std::mutex> lock(recorder->bufferMutex);
    recorder->audioBuffer.insert(recorder->audioBuffer.end(), samples, samples + sampleCount);
}

void AudioRecorder::playbackCallback(void* userdata, Uint8* stream, int len) {
    AudioRecorder* recorder = static_cast<AudioRecorder*>(userdata);
    if (!recorder->playing.load()) {
        SDL_memset(stream, 0, len);
        return;
    }
    
    float* samples = reinterpret_cast<float*>(stream);
    int sampleCount = len / sizeof(float);
    
    std::lock_guard<std::mutex> lock(recorder->bufferMutex);
    
    size_t pos = recorder->playbackPosition.load();
    float maxLevel = 0.0f;
    
    for (int i = 0; i < sampleCount; ++i) {
        if (pos < recorder->audioBuffer.size()) {
            samples[i] = recorder->audioBuffer[pos];
            maxLevel = std::max(maxLevel, std::abs(samples[i]));
            pos++;
        } else {
            samples[i] = 0.0f;
            recorder->playing = false;
        }
    }
    
    recorder->playbackPosition = pos;
    recorder->currentLevel = maxLevel;
}
