# UI Framework Examples

This directory contains example applications demonstrating various features of the UI Framework.

## Available Examples

### 🎵 Audio Recorder (`audio_recorder/`)
A comprehensive real-time audio recording application showcasing:
- **Real-time audio processing** with SDL2 audio callbacks
- **Waveform visualization** using Canvas widgets
- **VU meters** for audio level monitoring
- **Thread-safe audio operations** suitable for DAW applications
- **Professional audio quality** at 44.1kHz sample rate

**Features:**
- Record and playback audio in real-time
- Visual waveform display with 60fps updates
- Audio level meters with peak detection
- Thread-safe data sharing between audio and UI threads
- Demonstrates Canvas API for custom graphics

### 🎨 Layout Designer (`layout_designer/`)
A visual layout design tool for rapid UI prototyping:
- **Visual design interface** with toolbar controls
- **Multiple widget types** (Buttons, Labels, TextBoxes, Canvas)
- **Automatic element positioning** with smart spacing
- **Code generation** - exports ready-to-use UI code snippets
- **Layout analysis** with position and size data

**Features:**
- Click-to-add UI elements
- Automatic positioning prevents overlap
- Export layout as code snippets
- Window resizing for responsive testing
- Perfect for UI mockups and prototyping

### 📝 Simple Demo (`simple_demo.cpp`)
Basic demonstration of core UI Framework features:
- Button creation and event handling
- Label text display
- Basic window management
- Simple event loop

## Building Examples

Each example includes its own build system:

```bash
# For individual examples
cd audio_recorder/
meson setup build
meson compile -C build
./build/audio_recorder

cd layout_designer/
meson setup build  
meson compile -C build
./build/layout_designer
```

## Example Complexity Levels

- **Beginner**: `simple_demo.cpp` - Basic UI elements and events
- **Intermediate**: `layout_designer/` - Multiple widgets and code generation
- **Advanced**: `audio_recorder/` - Real-time processing and custom graphics

## Dependencies

All examples require:
- SDL2, SDL2_ttf, SDL2_image
- C++17 compiler
- Meson build system

The audio recorder additionally requires:
- SDL2 audio support
- System audio permissions (microphone access)

## Usage Tips

1. **Start with simple_demo** to understand basic concepts
2. **Use layout_designer** for rapid UI prototyping and code generation
3. **Study audio_recorder** for advanced real-time applications and custom graphics
4. **Check README files** in each example directory for specific instructions

These examples demonstrate the UI Framework's capabilities from basic widget usage to professional-grade real-time applications!
