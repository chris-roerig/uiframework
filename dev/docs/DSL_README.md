# UI Framework DSL - Proof of Concept

A compile-time Domain Specific Language for creating UI layouts using XML-like syntax.

## Features

- **Compile-time code generation** - No runtime parsing overhead
- **Type-safe widget access** - Generated structs with named members
- **Hybrid approach** - Use DSL alongside programmatic API
- **Zero breaking changes** - Existing code continues to work

## Usage

### 1. Create a .ui file

```xml
<?xml version="1.0" encoding="UTF-8"?>
<UI>
    <Label id="title" text="My Application" x="10" y="10" />
    <Button id="clickBtn" text="Click Me!" x="10" y="50" />
    <TextBox id="input" placeholder="Enter text" x="10" y="90" />
    <CheckBox id="enable" checked="false" x="10" y="130" />
</UI>
```

### 2. Compile to C++ header

```bash
./tools/ui-compiler demo.ui demo_ui.h
```

### 3. Use in your application

```cpp
#include "uiframework/UI.h"
#include "demo_ui.h"

int main() {
    UI ui("My App", 800, 600);
    
    // Create widgets from .ui file
    auto widgets = ui_generated::createDemo(ui);
    
    // Add event handlers programmatically
    widgets.clickBtn = ui.createButton("Click Me!", 10, 50, []() {
        std::cout << "Button clicked!" << std::endl;
    });
    
    ui.run();
    return 0;
}
```

## Supported Elements

- `<Label>` - Text labels
- `<Button>` - Clickable buttons  
- `<TextBox>` - Text input fields
- `<CheckBox>` - Boolean toggles

## Attributes

- `id` - Widget identifier (required)
- `text` - Display text
- `placeholder` - TextBox placeholder
- `checked` - CheckBox initial state
- `x`, `y` - Position coordinates

## Build Integration

Add to your Makefile:
```makefile
%.ui.h: %.ui
	./tools/ui-compiler $< $@

my_app: my_app.cpp my_ui.ui.h
	g++ -o my_app my_app.cpp $(UI_LIBS)
```

## Benefits

- **Rapid prototyping** with declarative syntax
- **Designer-friendly** XML format
- **Type safety** with compile-time generation
- **Performance** - no runtime parsing
- **Flexibility** - mix with programmatic API

## Future Enhancements

- Layout support (VBox, HBox, Grid)
- CSS-like styling
- Event handler binding
- Property data binding
- IDE integration
