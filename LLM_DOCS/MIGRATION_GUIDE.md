# UI Framework Migration Guide

This guide helps you migrate from the old API to the new refactored API that addresses critical memory management, thread safety, and error handling issues.

## Key Changes

### 1. Memory Management
**Old API (Deprecated):**
```cpp
ui::Button* btn = ui.button("Click me", 10, 10, callback);
// Raw pointer - unclear ownership, potential memory leaks
```

**New API:**
```cpp
auto btn = ui.createButton("Click me", 10, 10, callback);
// Returns shared_ptr<ui::Button> - clear ownership, automatic cleanup
```

### 2. Element Management
**Old API:**
```cpp
ui.setFocus(btn); // Using raw pointer
ui.assignHotKey(btn, "b");
```

**New API:**
```cpp
ui.setFocus(btn); // Works with shared_ptr
ui.setFocus(btn->getId()); // Or use element ID
ui.assignHotKey(btn, "b");
ui.assignHotKey(btn->getId(), "b"); // Or use element ID
```

### 3. Error Handling
**Old API:**
```cpp
UI ui("My App", 800, 600); // Silent failures
```

**New API:**
```cpp
try {
    UI ui("My App", 800, 600);
} catch (const ui::InitializationException& e) {
    std::cerr << "Failed to initialize UI: " << e.what() << std::endl;
    return -1;
}
```

### 4. Thread Safety
**Old API:**
```cpp
// Global theme state - not thread safe
extern std::shared_ptr<ui::Theme> g_currentTheme;
```

**New API:**
```cpp
// Thread-safe theme management
ui.setTheme("SolarizedDark");
auto theme = core->getTheme(); // Thread-safe access
```

## Migration Steps

### Step 1: Update Element Creation
Replace all element creation calls:

```cpp
// OLD
auto label = ui.label("Hello", 10, 10);
auto button = ui.button("Click", 10, 50, [](){});

// NEW
auto label = ui.createLabel("Hello", 10, 10);
auto button = ui.createButton("Click", 10, 50, [](){});
```

### Step 2: Update Element Management
```cpp
// OLD
ui.setFocus(button);
ui.assignHotKey(button, "b");

// NEW (both approaches work)
ui.setFocus(button);
ui.setFocus(button->getId());
ui.assignHotKey(button, "b");
ui.assignHotKey(button->getId(), "b");
```

### Step 3: Add Error Handling
```cpp
// NEW - Wrap UI initialization in try-catch
try {
    UI ui("My App", 800, 600);
    
    // Element creation can also throw
    auto button = ui.createButton("Test", -10, -10, [](){}); // Will throw
    
    ui.run();
} catch (const ui::UIException& e) {
    std::cerr << "UI Error: " << e.what() << std::endl;
    return -1;
}
```

### Step 4: Update Custom Elements
If you have custom UI elements, update the render method signature:

```cpp
// OLD
void render(SDL_Renderer* renderer) override;

// NEW
void render(SDL_Renderer* renderer, TTF_Font* font, std::shared_ptr<Theme> theme) override;
```

## Backward Compatibility

The old API methods are still available but marked as deprecated:
- They will generate compiler warnings
- They return raw pointers for compatibility
- They will be removed in a future version

## Benefits of Migration

1. **Memory Safety**: Automatic resource cleanup, no memory leaks
2. **Thread Safety**: Safe to use UI from multiple threads
3. **Error Handling**: Clear error reporting and recovery
4. **Better API**: Consistent naming and parameter patterns
5. **Future-Proof**: New features will only be available in the new API

## Example: Complete Migration

**Before:**
```cpp
int main() {
    UI ui("Old App", 800, 600);
    
    auto button = ui.button("Click me", 10, 10, [](){
        std::cout << "Clicked!" << std::endl;
    });
    
    ui.assignHotKey(button, "b");
    ui.setFocus(button);
    
    ui.run();
    return 0;
}
```

**After:**
```cpp
int main() {
    try {
        UI ui("New App", 800, 600);
        
        auto button = ui.createButton("Click me", 10, 10, [](){
            std::cout << "Clicked!" << std::endl;
        });
        
        ui.assignHotKey(button, "b");
        ui.setFocus(button);
        
        ui.run();
    } catch (const ui::UIException& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    
    return 0;
}
```

## Timeline

- **Current**: Both APIs available, old API deprecated
- **Next Release**: Old API will generate warnings
- **Future Release**: Old API will be removed

Start migrating now to avoid issues in future releases!
