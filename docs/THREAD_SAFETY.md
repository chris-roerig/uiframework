# UI Framework Thread Safety Guide

## Overview

The UI Framework is designed with comprehensive thread safety in mind. Most operations can be safely performed from any thread, with automatic synchronization handled internally through mutex protection.

## Thread Safety Guarantees

### ✅ FULLY THREAD-SAFE OPERATIONS

These operations can be called safely from any thread:

#### Widget Creation
- `createButton()`, `createLabel()`, `createTextBox()`, etc.
- `createModal()`, `createConfirmModal()`, `createInfoModal()`
- `createVBoxLayout()`, `createHBoxLayout()`, `createGridLayout()`

#### Element Management
- `addElement()` - Add elements to the UI
- `removeElement()` - Remove by string or numeric ID
- `getElement()` - Retrieve by string or numeric ID

#### Focus Management
- `setFocus()` - Set focus to specific element
- `focusNext()`, `focusPrevious()` - Navigate focus
- `getFocusedElementId()` - Get currently focused element
- `setFocusOrder()` - Define tab order
- `createFocusGroup()`, `setActiveFocusGroup()` - Focus groups
- `trapFocus()`, `releaseFocusTrap()` - Focus trapping

#### Hotkey Management
- `assignHotKey()` - Assign keyboard shortcuts

#### Theme Management
- `setTheme()` - Change UI theme

#### Utility Methods
- `getWidth()`, `getHeight()` - Get window dimensions

### ⚠️ MAIN THREAD ONLY

These operations must be called from the main thread:

#### Core Lifecycle
- `UI()` constructor - Window creation
- `~UI()` destructor - Resource cleanup
- `run()` - Main event loop (blocks until window closes)

## Implementation Details

### Mutex Protection

The framework uses multiple mutexes for fine-grained locking:

```cpp
// UICore mutexes
std::mutex elementsMutex;    // Protects element collections
std::mutex themeMutex;       // Protects theme operations  
std::mutex hotKeysMutex;     // Protects hotkey assignments

// FocusManager mutex
std::mutex focusMutex;       // Protects all focus state
```

### Performance Optimizations

- **Lock-free fast paths**: Common operations use `try_lock()` when possible
- **Pending operations**: Focus changes are queued for batch processing
- **Automatic cleanup**: Expired weak_ptr references cleaned during normal operations
- **Numeric IDs**: O(1) element lookups using uint64_t IDs alongside string IDs

### Memory Safety

- **RAII**: All resources automatically cleaned up
- **Smart pointers**: `shared_ptr` for elements, `weak_ptr` for focus registry
- **Automatic cleanup**: Expired references removed automatically
- **Exception safety**: All operations provide strong exception guarantees

## Usage Examples

### Multi-threaded Widget Creation

```cpp
UI ui("Multi-threaded App", 800, 600);

// Safe to create widgets from any thread
std::thread worker([&ui]() {
    auto button = ui.createButton("Worker Button", 10, 10, [](){
        std::cout << "Clicked from worker thread!" << std::endl;
    });
    
    // Safe to manage focus from any thread
    ui.setFocus(button->getId());
});

// Safe to create widgets from main thread too
auto label = ui.createLabel("Main Thread Label", 10, 50);

worker.join();
ui.run(); // Must be called from main thread
```

### Thread-safe Element Management

```cpp
// Thread 1: Create elements
std::thread creator([&ui]() {
    for (int i = 0; i < 100; ++i) {
        auto btn = ui.createButton("Button " + std::to_string(i), 
                                   10, 10 + i * 30, [](){});
        // Store ID for later use
        buttonIds.push_back(btn->getId());
    }
});

// Thread 2: Remove elements
std::thread remover([&ui]() {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    for (const auto& id : buttonIds) {
        ui.removeElement(id); // Thread-safe removal
    }
});

creator.join();
remover.join();
```

### Safe Focus Management

```cpp
// Background thread managing focus
std::thread focusManager([&ui]() {
    while (running) {
        ui.focusNext(); // Thread-safe navigation
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
});

// Main thread runs event loop
ui.run();
focusManager.join();
```

## Best Practices

### ✅ DO

- Create widgets from any thread as needed
- Use numeric IDs for high-performance element lookups
- Let the framework handle synchronization automatically
- Use RAII and smart pointers for resource management

### ❌ DON'T

- Call `run()` from background threads
- Manually manage SDL resources
- Assume widget creation is instantaneous (it's thread-safe but not atomic)
- Mix raw pointers with the framework's smart pointer system

## Error Handling

All thread-safe operations provide strong exception safety guarantees:

```cpp
try {
    auto button = ui.createButton("Test", 10, 10, [](){});
    ui.setFocus(button->getId());
} catch (const ui::UIException& e) {
    // Framework-specific errors
    std::cerr << "UI Error: " << e.what() << std::endl;
} catch (const std::exception& e) {
    // Standard library errors
    std::cerr << "Error: " << e.what() << std::endl;
}
```

## Performance Considerations

- **Widget creation**: Thread-safe but involves mutex locking
- **Element lookup**: Use numeric IDs for O(1) performance
- **Focus operations**: Queued and batched for efficiency
- **Memory cleanup**: Automatic and non-blocking

The framework is optimized for high-performance multi-threaded applications while maintaining complete thread safety.
