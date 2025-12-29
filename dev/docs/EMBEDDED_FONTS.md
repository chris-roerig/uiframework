# Embedded Font System

The UI Framework now includes an embedded font system that ensures a font is always available without external dependencies.

## Features

- ✅ **Console.ttf embedded** as default font
- ✅ **No external file dependencies** - font is compiled into the library
- ✅ **Automatic fallback** - uses embedded font when no external font specified
- ✅ **External fonts still supported** - can load additional TTF files at runtime

## Usage

```cpp
// Uses embedded font automatically
UI ui("My App", 800, 600);
auto label = ui.createLabel("Hello World", 10, 10);

// Or explicitly load external font
FontManager::getInstance().getFont("custom.ttf", 16);
```

## Converting Fonts

Use the provided script to embed your own fonts:

```bash
./scripts/ttf_to_header.sh MyFont.ttf lib/Resources/MyFont.h
```

Then modify FontManager.cpp to use your embedded font as default.

## Technical Details

- Font data stored as `const unsigned char[]` array
- Loaded via `SDL_RWFromConstMem()` and `TTF_OpenFontRW()`
- Zero runtime file I/O for default font
- Thread-safe font caching system
- Automatic cleanup on shutdown

The embedded font ensures your UI framework works immediately without requiring users to bundle font files or worry about missing system fonts.
