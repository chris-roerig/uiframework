# Layout Designer - Enhanced

A comprehensive visual layout design tool for the UI Framework supporting all available UI elements with advanced positioning features.

## Features

- **Complete UI Element Support**: All 10+ UI element types available
  - **Basic Elements**: Button, Label, TextBox, CheckBox, Canvas
  - **Advanced Elements**: HSlider, ProgressBar, ListView, OptionSelect, Image
- **Smart Positioning**: Automatic placement with collision avoidance
- **Randomize Tool**: Instantly reposition all elements for layout experimentation
- **Layout Export**: Generate ready-to-use code snippets and position data
- **Clear All**: Quick reset for starting fresh designs
- **Large Design Area**: 1200x800 window with spacious design canvas

## Usage

### Adding Elements
Click any toolbar button to add that element type:
- **Row 1**: Button, Label, TextBox, CheckBox, Canvas
- **Row 2**: Slider, ProgressBar, ListView, OptionSelect, Image  
- **Row 3**: Show Layout, Clear All, Randomize

### Layout Management
- **Show Layout**: Exports all element positions and code to console
- **Clear All**: Removes all elements from the design
- **Randomize**: Randomly repositions all existing elements
- **Window Resize**: Test responsive behavior by resizing the window

## Controls

- **Element Buttons**: Add new UI elements to the design area
- **Show Layout**: Export current layout with code snippets
- **Clear All**: Remove all elements and start over
- **Randomize**: Reposition all elements randomly for experimentation

## Building

```bash
meson setup build
meson compile -C build
./build/layout_designer
```

## Supported Elements

| Element | Description | Code Example |
|---------|-------------|--------------|
| **Button** | Clickable button with callback | `ui.createButton("Text", x, y, callback);` |
| **Label** | Static text display | `ui.createLabel("Text", x, y);` |
| **TextBox** | Text input field | `ui.createTextBox("Default", x, y);` |
| **CheckBox** | Toggle checkbox | `ui.createCheckBox(false, x, y, callback);` |
| **Canvas** | Custom graphics area | `ui.createCanvas(x, y, width, height);` |
| **HSlider** | Horizontal slider | `ui.createHSlider(x, y, w, h, min, max, val);` |
| **ProgressBar** | Progress indicator | `ui.createProgressBar(x, y, w, h, value, showText);` |
| **ListView** | Scrollable item list | `ui.createListView(items, x, y, w, h);` |
| **OptionSelect** | Dropdown selection | `ui.createOptionSelect(current, options, x, y, callback);` |
| **Image** | Image display | `ui.createImage("path.png", x, y, w, h);` |

## Output Format

Example layout export:
```
=== COMPREHENSIVE LAYOUT DESIGN ===
Window Size: 1200x800
Design Area: y=130 (below toolbar)

Element 1:
  Type: Button
  Position: (65, 160)
  Size: 100x30
  Code: ui.createButton("Button Text", 65, 160, callback);

Element 2:
  Type: HSlider
  Position: (340, 410)
  Size: 120x20
  Code: ui.createHSlider(340, 410, 120, 20, 0.0f, 100.0f, 50.0f);

Total Elements: 2
===================================
```

## Future Enhancements

While drag-and-drop functionality requires deeper UI framework integration, the current version provides:
- ✅ **Complete element coverage** - All UI framework elements supported
- ✅ **Smart positioning** - Automatic placement prevents overlap
- ✅ **Layout experimentation** - Randomize tool for quick iterations
- ✅ **Code generation** - Ready-to-use snippets for each element
- ✅ **Professional workflow** - Clear, export, and iterate efficiently

Perfect for comprehensive UI prototyping and learning all available UI framework elements!
