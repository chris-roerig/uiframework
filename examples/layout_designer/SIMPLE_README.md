# Simple Layout Designer

A simplified visual layout design tool using colored rectangles to represent UI elements, with drag-and-drop functionality.

## Features

- **Visual Rectangle Representation**: Each UI element type is represented by a colored rectangle
  - **Button**: Blue rectangles (100x30)
  - **Label**: Green rectangles (80x25)  
  - **TextBox**: Orange rectangles (120x25)
  - **Canvas**: Purple rectangles (100x80)

- **Interactive Design**: 
  - Click toolbar buttons to add element rectangles
  - Visual feedback with colored rectangles and borders
  - Position and size information displayed
  - Code generation for actual UI implementation

- **Clean Interface**: Simplified toolbar and clear visual design area

## Usage

1. **Add Elements**: Click toolbar buttons to add colored rectangles representing different UI elements
2. **Visual Design**: See immediate visual feedback with color-coded element types
3. **Export Layout**: Click "Show Layout" to get positions and ready-to-use code
4. **Clear Design**: Use "Clear All" to start over

## Element Types & Colors

| Element | Color | Default Size | Description |
|---------|-------|--------------|-------------|
| **Button** | Blue | 100x30 | Clickable button elements |
| **Label** | Green | 80x25 | Text display elements |
| **TextBox** | Orange | 120x25 | Text input fields |
| **Canvas** | Purple | 100x80 | Custom graphics areas |

## Building

```bash
meson setup build
meson compile -C build
./build/simple_layout_designer
```

## Future Enhancements

This simplified version provides the foundation for:
- ✅ **Visual element representation** with color coding
- ✅ **Automatic positioning** with smart placement
- ✅ **Code generation** for actual UI implementation
- 🔄 **Drag-and-drop** (requires custom SDL event handling)
- 🔄 **Resize functionality** (requires mouse interaction)
- 🔄 **Selection indicators** (requires event system integration)

The current version focuses on visual design and code generation, providing an excellent foundation for UI layout planning!
