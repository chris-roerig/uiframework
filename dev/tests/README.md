# UI Framework Test Suite

This directory contains comprehensive tests for the refactored UI framework, ensuring memory safety, thread safety, and proper error handling.

## Test Structure

### Test Framework (`TestFramework.h`)
- Simple, lightweight testing framework
- Automatic test discovery and execution
- Exception-safe test execution
- Performance timing for each test
- Clear pass/fail reporting with error messages

### Test Suites

#### 1. Core Tests (`CoreTests.cpp`)
Tests fundamental framework functionality:
- **UI Initialization** - Proper SDL setup and resource management
- **Theme Management** - Theme switching and validation
- **Element Creation** - Creating UI elements with proper memory management
- **Element Retrieval** - Getting elements by ID
- **Element Removal** - Safe element cleanup
- **Focus Management** - Focus system with ID-based management
- **Hotkey Assignment** - Keyboard shortcut registration
- **Coordinate Validation** - Input validation and bounds checking

#### 2. Element Tests (`ElementTests.cpp`)
Tests individual UI element functionality:
- **Button** - Click handling, text changes, callbacks
- **Label** - Text display and updates
- **TextBox** - Text input, selection, keyboard navigation
- **CheckBox** - State changes, toggle functionality
- **ProgressBar** - Progress updates, bounds checking
- **OptionSelect** - Dropdown functionality, selection changes
- **Canvas** - Drawing operations, coordinate systems
- **ListView** - Item management, selection, scrolling
- **Modal** - Dialog creation, button handling, dismissal

#### 3. Resource Tests (`ResourceTests.cpp`)
Tests memory management and resource handling:
- **Shared Pointer Management** - Memory lifecycle validation
- **Element Lifecycle** - Creation, usage, and cleanup
- **Theme Switching** - Resource management during theme changes
- **Exception Safety** - Framework stability under error conditions
- **Callback Exception Safety** - Handling exceptions in user callbacks
- **Focus Management with Removal** - Focus handling when elements are removed
- **Large Number of Elements** - Performance and stability with many elements
- **Element Property Changes** - State management during property updates

## Running Tests

### Using the Test Runner Script
```bash
# Run only tests
./run_tests.sh test

# Run tests and demo
./run_tests.sh all

# Run only demo (default)
./run_tests.sh demo
# or just
./run_tests.sh
```

### Manual Execution
```bash
# Build the project
meson setup build
meson compile -C build

# Run tests
./build/ui_tests

# Run demo
./build/sdl_ui_demo
```

## Test Output

The test framework provides detailed output:

```
=== Running Test Suite: Core Functionality ===
[PASS] UI Initialization (15ms)
[PASS] Theme Management (8ms)
[FAIL] Element Creation (3ms) - Assertion failed: expected non-null pointer
[PASS] Element Retrieval (2ms)

--- Summary ---
Total: 4, Passed: 3, Failed: 1
Total time: 28ms

Failed tests:
  - Element Creation: Assertion failed: expected non-null pointer
```

## Adding New Tests

### Creating a New Test Suite
1. Create a new `.cpp` file in the `test/` directory
2. Include `TestFramework.h`
3. Create a function that sets up and runs a `TestSuite`
4. Add the function declaration to `test/main.cpp`
5. Call the function in `main()`

### Example Test
```cpp
#include "TestFramework.h"
#include "../UI.h"

void runMyTests() {
    TestSuite suite("My Test Suite");
    
    suite.addTest("My Test", []() {
        UI ui("Test", 800, 600);
        auto button = ui.createButton("Test", 10, 10, [](){});
        
        ASSERT_NOT_NULL(button.get());
        ASSERT_EQ("Test", button->getText());
    });
    
    suite.run();
}
```

## Test Assertions

Available assertion macros:
- `ASSERT_TRUE(condition)` - Verify condition is true
- `ASSERT_FALSE(condition)` - Verify condition is false
- `ASSERT_EQ(expected, actual)` - Verify equality
- `ASSERT_NE(expected, actual)` - Verify inequality
- `ASSERT_NOT_NULL(ptr)` - Verify pointer is not null
- `ASSERT_NULL(ptr)` - Verify pointer is null

## Dependencies

Tests require the same dependencies as the main framework:
- SDL2
- SDL2_ttf
- SDL2_image
- C++17 compiler

## Continuous Integration

The test suite is designed to be easily integrated into CI/CD pipelines:
- Returns appropriate exit codes (0 for success, non-zero for failure)
- Provides machine-readable output
- Runs without requiring user interaction
- Minimal external dependencies beyond the framework itself
