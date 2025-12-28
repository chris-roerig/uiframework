# UI Framework

A modern, thread-safe C++ UI framework built on SDL2 with proper memory management and error handling.

## Features

- **Memory Safe**: Uses `shared_ptr` for automatic resource management
- **Thread Safe**: All operations are thread-safe with proper synchronization
- **Error Handling**: Comprehensive exception handling with clear error messages
- **Modern C++**: Uses C++17 features and best practices
- **Themeable**: Multiple built-in themes with easy customization
- **Cross-Platform**: Works on macOS, Linux, and Windows

## Quick Start

```cpp
#include "UI.h"

int main() {
    try {
        UI ui("My App", 800, 600);
        
        auto button = ui.createButton("Click me!", 10, 10, [](){
            std::cout << "Button clicked!" << std::endl;
        });
        
        auto label = ui.createLabel("Hello World", 10, 50);
        
        ui.run();
    } catch (const ui::UIException& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    
    return 0;
}
```

## Building

```bash
meson setup build
meson compile -C build
./build/sdl_ui_demo
```

## Architecture

### Core Components

- **UICore**: Thread-safe core engine managing SDL resources and elements
- **UIElement**: Base class for all UI components with ID-based management
- **Theme**: Pluggable theming system with multiple built-in themes
- **SDLResources**: RAII wrapper for SDL resource management

### Memory Management

All UI elements are managed using `shared_ptr`, ensuring:
- Automatic cleanup when elements go out of scope
- Safe sharing between different parts of your application
- No memory leaks or dangling pointers

### Thread Safety

The framework is fully thread-safe:
- Element management uses mutex protection
- Theme changes are synchronized
- Event handling is thread-safe

### Error Handling

Comprehensive error handling with specific exception types:
- `InitializationException`: SDL/TTF initialization failures
- `RenderException`: Rendering errors
- `UIException`: General UI errors

## API Reference

### Element Creation

```cpp
// Create elements (returns shared_ptr)
auto button = ui.createButton("Text", x, y, callback);
auto label = ui.createLabel("Text", x, y);
auto textBox = ui.createTextBox("Default", x, y);
auto checkBox = ui.createCheckBox(false, x, y, callback);
auto canvas = ui.createCanvas(x, y, width, height);
```

### Element Management

```cpp
// Focus management
ui.setFocus(element);
ui.setFocus(elementId);
std::string focusedId = ui.getFocusedElementId();

// Remove elements
ui.removeElement(elementId);

// Get elements
auto element = ui.getElement(elementId);
```

### Hotkeys

```cpp
// Assign hotkeys
ui.assignHotKey(element, "b");
ui.assignHotKey(elementId, "b");
ui.assignHotKey(element, "b", customCallback);
```

### Themes

```cpp
ui.setTheme("SolarizedDark");
ui.setTheme("SolarizedLight");
ui.setTheme("Molokai");
ui.setTheme("Default");
```

## Migration from Old API

If you're upgrading from the previous version, see [MIGRATION_GUIDE.md](MIGRATION_GUIDE.md) for detailed migration instructions.

The old API is still available but deprecated:
```cpp
// OLD (deprecated)
ui::Button* btn = ui.button("Click", 10, 10, callback);

// NEW (recommended)
auto btn = ui.createButton("Click", 10, 10, callback);
```

## Dependencies

- SDL2
- SDL2_ttf
- SDL2_image
- C++17 compiler

## License

[Your License Here]
