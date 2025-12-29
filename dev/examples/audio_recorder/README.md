# Audio Recorder Example

A real-time audio recording and playback application demonstrating the UI Framework's capabilities for audio applications like DAWs.

## Features

- **Real-time audio recording** from microphone input
- **Live waveform visualization** using Canvas widget
- **VU meter display** showing current audio levels
- **Playback with position indicator** 
- **Thread-safe audio processing** with UI updates at 60fps

## Controls

- **Record Button**: Start/stop audio recording
- **Play Button**: Playback recorded audio
- **Waveform Display**: Visual representation of recorded audio
- **VU Meter**: Real-time audio level indicator
- **Status Display**: Current application state

## Building

### Using Meson (Recommended)
```bash
cd examples/audio_recorder
meson setup build
meson compile -C build
./build/audio_recorder
```

### Using CMake
```bash
cd examples/audio_recorder
mkdir build && cd build
cmake ..
make
./audio_recorder
```

## Requirements

- SDL2 (audio, video, events)
- SDL2_ttf (text rendering)
- SDL2_image (image loading)
- Microphone access for recording

## Architecture

The application demonstrates several key concepts for real-time audio applications:

### Thread Safety
- Audio callbacks run on high-priority audio thread
- UI updates run on separate thread at 60fps
- Thread-safe data sharing using atomics and mutexes

### Real-Time Performance
- Minimal allocations in audio callbacks
- Efficient waveform downsampling for display
- Lock-free audio level updates

### UI Framework Integration
- Canvas widgets for custom graphics (waveform, VU meter)
- Button widgets for transport controls
- Label widgets for status display
- Thread-safe UI updates from audio thread

## Code Structure

- `AudioRecorder.h/cpp`: Core audio recording/playback engine
- `main.cpp`: UI application and real-time visualization
- Audio processing uses SDL2 audio callbacks
- UI rendering uses framework's Canvas widget for custom graphics

This example showcases how the UI Framework can be used for professional audio applications requiring real-time performance and thread safety.
