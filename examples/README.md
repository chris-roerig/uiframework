# UI Framework Demo Applications

This directory contains comprehensive demo applications showcasing all available UI widgets and features in the framework.

## Available Demos

### 1. Simple Demo (`simple_demo`)
A clean, straightforward demonstration of core UI widgets without layout containers.

**Features demonstrated:**
- Labels and text display
- Interactive buttons with callbacks
- Text input boxes
- Checkboxes with state management
- Option selectors (dropdown menus)
- Progress bars with different values
- List views with scrollable content
- Canvas for custom drawing
- Modal dialogs (info and confirmation)
- Context menus with nested items
- Theme switching system
- Keyboard navigation and hotkeys

**Run with:**
```bash
./build/simple_demo
```

### 2. Comprehensive Demo (`comprehensive_demo`)
Advanced demonstration including layout containers and complex widget arrangements.

**Additional features:**
- VBox layout containers
- HBox layout containers  
- Grid layout containers
- Automatic widget positioning
- Responsive layout management

**Run with:**
```bash
./build/comprehensive_demo
```

### 3. Original Demo (`sdl_ui_demo`)
The original framework demo with basic functionality.

**Run with:**
```bash
./build/sdl_ui_demo
```

### 4. Showcase Demo (`ui_showcase`)
Feature showcase with advanced examples.

**Run with:**
```bash
./build/ui_showcase
```

## Controls and Navigation

### Keyboard Navigation
- **Tab / Shift+Tab**: Navigate between interactive elements
- **Enter / Space**: Activate focused buttons and controls
- **Arrow Keys**: Navigate within lists and option selectors
- **Escape**: Close modals and context menus

### Mouse Interaction
- **Left Click**: Activate buttons, select options, focus elements
- **Right Click**: Show context menu (where available)
- **Scroll Wheel**: Scroll through list views

### Hotkeys (Simple Demo)
- **1**: Activate Button 1
- **2**: Activate Button 2  
- **i**: Show Info Modal
- **c**: Show Confirm Modal

## Widget Showcase

### Basic Widgets
- **Label**: Static text display with theme-aware styling
- **Button**: Interactive buttons with click callbacks
- **TextBox**: Text input with cursor and selection support
- **CheckBox**: Boolean state toggles with callbacks
- **OptionSelect**: Dropdown menus with multiple choices

### Advanced Widgets
- **ProgressBar**: Visual progress indicators with optional text
- **ListView**: Scrollable lists with item selection
- **Canvas**: Custom drawing surface for graphics
- **Modal**: Popup dialogs for user interaction
- **ContextMenu**: Right-click menus with hierarchical structure

### Layout Containers
- **VBoxLayout**: Vertical arrangement of child widgets
- **HBoxLayout**: Horizontal arrangement of child widgets
- **GridLayout**: Grid-based positioning system

### Theme System
The demos showcase the complete theme system with four built-in themes:
- **Default**: Standard gray theme
- **SolarizedDark**: Dark theme with warm colors
- **SolarizedLight**: Light theme with soft colors
- **Molokai**: Dark theme with vibrant accents

## Building and Running

### Prerequisites
- SDL2 development libraries
- SDL2_ttf development libraries
- SDL2_image development libraries
- C++17 compatible compiler
- Meson build system

### Build Commands
```bash
# Setup build directory
meson setup build

# Compile all demos
meson compile -C build

# Run specific demo
./build/simple_demo
./build/comprehensive_demo
./build/sdl_ui_demo
./build/ui_showcase
```

### Quick Start Script
Use the provided script for an interactive demo experience:
```bash
./run_comprehensive_demo.sh
```

## Code Examples

### Basic Widget Creation
```cpp
// Create a button with callback
auto button = ui.createButton("Click Me", 10, 10, []() {
    std::cout << "Button clicked!" << std::endl;
});

// Create a text input
auto textBox = ui.createTextBox("Enter text...", 10, 50);

// Create a checkbox with state callback
auto checkbox = ui.createCheckBox(false, 10, 90, [](bool state) {
    std::cout << "Checkbox: " << (state ? "checked" : "unchecked") << std::endl;
});
```

### Theme Management
```cpp
// Switch themes dynamically
ui.setTheme("SolarizedDark");
ui.setTheme("SolarizedLight");
ui.setTheme("Molokai");
ui.setTheme("Default");
```

### Modal Dialogs
```cpp
// Info modal
ui.createInfoModal("Information message");

// Confirmation modal with callbacks
ui.createConfirmModal("Are you sure?",
    []() { std::cout << "Confirmed!" << std::endl; },
    []() { std::cout << "Cancelled!" << std::endl; }
);
```

### Layout Containers
```cpp
// Create vertical layout
auto vbox = ui.createVBoxLayout(10, 10, 200, 300, 5);

// Add widgets to layout
auto btn1 = ui.createButton("Button 1", 0, 0, callback1);
auto btn2 = ui.createButton("Button 2", 0, 0, callback2);
vbox->addElement(btn1);
vbox->addElement(btn2);
```

## Error Handling

All demos include comprehensive error handling:
```cpp
try {
    UI ui("Demo App", 800, 600);
    // ... create widgets ...
    ui.run();
} catch (const ui::UIException& e) {
    std::cerr << "UI Error: " << e.what() << std::endl;
    return -1;
}
```

## Performance Notes

- The framework uses hardware-accelerated SDL2 rendering
- Smart pointer management ensures no memory leaks
- Thread-safe design allows safe multi-threaded usage
- Efficient event handling with minimal CPU usage when idle

## Troubleshooting

### Font Issues
If text doesn't display properly:
- Ensure system fonts are available
- Check console output for font loading messages
- The framework tries multiple font paths automatically

### Build Issues
If compilation fails:
- Verify SDL2 development packages are installed
- Check C++17 compiler support
- Ensure all dependencies are available via pkg-config

### Runtime Issues
If the demo crashes or behaves unexpectedly:
- Check console output for error messages
- Verify window manager compatibility
- Try different themes to isolate theme-specific issues

## Contributing

To add new demo features:
1. Create new demo source file in `demos/` directory
2. Add executable target to `meson.build`
3. Follow existing code patterns for consistency
4. Include comprehensive error handling
5. Document new features in this README
