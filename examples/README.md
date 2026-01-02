# Audio Recorder Example App

This example application demonstrates how to use a wireframe layout created with the UI Layout Editor Tool to build a functional application.

## Overview

The app recreates the exact layout from `build/wireframe.json` in a **1024x600 window** (matching the layout editor canvas size), which contains:
- Large waveform display canvas (1010x320 pixels)
- Left and Right VU meters with sample audio levels
- Vertical threshold slider for recording control
- Threshold mode selector (Manual/Auto/Voice)
- Record button for start/stop functionality
- All elements positioned precisely as designed in the wireframe

## Features Demonstrated

### Layout Recreation
- **Exact positioning**: All elements placed at coordinates from wireframe.json
- **Proper sizing**: Canvas areas, buttons, and controls sized correctly  
- **Correct window size**: 1024x600 matching the layout editor canvas
- **Visual elements**: Sample waveform, VU meter levels, and UI styling

### UI Framework Usage
- **Canvas drawing**: Waveform visualization and VU meter graphics
- **Interactive controls**: Functional record button and threshold slider
- **CycleList widget**: Mode selector with multiple options
- **Label positioning**: Text labels for interface elements

### Wireframe-to-Code Workflow
1. **Design**: Create layout visually using Layout Editor Tool
2. **Export**: Generate wireframe.json with precise positioning data
3. **Implement**: Use wireframe coordinates to create functional app
4. **Enhance**: Add interactivity and visual feedback

## Building and Running

```bash
# Using make (recommended)
make audio-recorder

# Or using meson directly
meson compile -C build audio_recorder_app
./build/audio_recorder_app
```

## Code Structure

The application demonstrates:
- **Static layout recreation**: Positioning elements from wireframe data
- **Canvas graphics**: Drawing waveforms and VU meter visualizations
- **Interactive callbacks**: Button click handlers and control responses
- **UI framework integration**: Proper use of createCanvas, createButton, etc.

## Extending the Example

This basic implementation can be extended with:
- **Real audio processing**: Connect to actual audio input/output
- **Dynamic updates**: Real-time waveform and level meter updates
- **File I/O**: Save/load audio recordings
- **Advanced controls**: EQ, effects, and processing options

## Wireframe Source

The layout is based on `build/wireframe.json` created with the UI Layout Editor Tool:
- Run `make layout-editor` to open the visual design tool
- Modify the layout and export updated wireframe data
- Update this example app to reflect layout changes

This demonstrates the complete design-to-implementation workflow enabled by the UI Framework's layout editor tool.
