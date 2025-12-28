# UI Framework Reorganization Plan

## Proposed Directory Structure

```
uiframework/
├── README.md
├── MIGRATION_GUIDE.md
├── meson.build
├── .gitignore
├── .git/
├── src/                    # Core source files
│   ├── UI.cpp
│   ├── UI.h
│   ├── UICore.cpp
│   ├── UICore.h
│   ├── Helpers.cpp
│   ├── Helpers.h
│   ├── UIElements.h
│   └── Themes.h
├── include/                # Public headers (if needed)
├── lib/                    # UIElements, Theme, Layout subdirs
│   ├── UIElements/
│   ├── Theme/
│   └── Layout/
├── assets/                 # Renamed from Assets
│   └── default_font.ttf
├── examples/               # Renamed from demos
│   ├── README.md
│   ├── basic/
│   ├── advanced/
│   └── assets/
├── tests/                  # Renamed from test
│   ├── unit/
│   └── integration/
├── scripts/                # All shell scripts
│   ├── run_tests.sh
│   ├── test_demo.sh
│   └── run_comprehensive_demo.sh
├── tools/                  # Development tools
└── build/                  # Keep as build output
```

## Files to Move

### To `src/`:
- UI.cpp, UI.h
- UICore.cpp, UICore.h  
- Helpers.cpp, Helpers.h
- UIElements.h
- Themes.h

### To `lib/`:
- UIElements/ → lib/UIElements/
- Theme/ → lib/Theme/
- Layout/ → lib/Layout/

### To `scripts/`:
- run_tests.sh
- test_demo.sh
- run_comprehensive_demo.sh
- tools (executable)

### To `examples/`:
- demos/ → examples/
- Organize by complexity level

### Rename:
- Assets/ → assets/
- test/ → tests/

## Benefits
- Clear separation of concerns
- Standard C++ project layout
- Easier navigation and maintenance
- Better for IDE integration
- Follows modern project conventions
