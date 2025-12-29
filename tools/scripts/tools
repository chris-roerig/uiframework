#!/bin/bash

# Usage function for incorrect input.
usage() {
    echo "Usage:"
    echo "  $0 uielement <Name> [-t]"
    echo "  $0 run"
    echo "  $0 print [-h] [-c] [-i <name>]... [-a <file>]..."
    exit 1
}

# Check for at least one argument.
if [ "$#" -lt 1 ]; then
    usage
fi

COMMAND="$1"
shift

if [ "$COMMAND" == "uielement" ]; then
    # Check for at least one argument (Name).
    if [ "$#" -lt 1 ]; then
        usage
    fi

    # Get element name and theme flag.
    ELEMENT_NAME="$1"
    shift
    THEME_FLAG=0
    if [ "$#" -ge 1 ] && [ "$1" == "-t" ]; then
        THEME_FLAG=1
    fi

    # Create directory if it doesn't exist.
    TARGET_DIR="UIElements"
    mkdir -p "$TARGET_DIR"

    # Create lower-case version of the name (for theme functions)
    ELEMENT_NAME_LOWER=$(echo "$ELEMENT_NAME" | tr '[:upper:]' '[:lower:]')

    # Determine target files.
    HEADER_FILE="$TARGET_DIR/${ELEMENT_NAME}.h"
    CPP_FILE="$TARGET_DIR/${ELEMENT_NAME}.cpp"
    UI_ELEMENTS_HEADER="UIElements.h"

    # Function to prompt for overwriting a file.
    prompt_overwrite() {
        local file="$1"
        if [ -f "$file" ]; then
            read -p "File '$file' already exists. Overwrite? (y/n): " answer
            if [ "$answer" != "y" ]; then
                echo "Aborted. File '$file' was not overwritten."
                exit 1
            fi
        fi
    }

    # Prompt for overwriting if files exist.
    prompt_overwrite "$HEADER_FILE"
    prompt_overwrite "$CPP_FILE"

    # Generate the header file.
    cat > "$HEADER_FILE" <<EOF
#pragma once

#include "UIElement.h"
#include <SDL2/SDL.h>

namespace ui {

class ${ELEMENT_NAME}  : public UIElement {
public:
    // Constructor forwarding position and size to the base class.
    ${ELEMENT_NAME} (int x, int y, int width, int height);
    
    // Virtual destructor.
    virtual ~${ELEMENT_NAME}();

    // Override the render method.
    void render(SDL_Renderer* renderer) override;
    
    // Override the event handler.
    void handleEvent(const SDL_Event &e) override;
    
    // Indicate that this element is interactive.
    bool isInteractive() const override;
    
    // Override the activation method.
    void activate() override;
};

} // namespace ui
EOF

    THEMEABLE="";
    # If theme flag is enabled, add theme line.
    if [ "$THEME_FLAG" -eq 1 ]; then
        THEMEABLE="    ThemeableElementColors tc = g_currentTheme->${ELEMENT_NAME_LOWER}Colors();"
    fi

    # Generate the cpp file.
    cat > "$CPP_FILE" <<EOF

#include "${ELEMENT_NAME}.h"
#include "../Theme/ThemeGlobals.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>

namespace ui {

${ELEMENT_NAME}::${ELEMENT_NAME}(int x, int y, int width, int height)
    : UIElement(x, y, width, height) {}

${ELEMENT_NAME}::~${ELEMENT_NAME}() {}

void ${ELEMENT_NAME}::render(SDL_Renderer* renderer) {
    ${THEMEABLE}
    // Example: Render a filled white rectangle.
    SDL_Rect rect = { x, y, width, height };
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &rect);
}

void ${ELEMENT_NAME}::handleEvent(const SDL_Event &e) {
    // Implement specific event handling here.
}

bool ${ELEMENT_NAME}::isInteractive() const {
    return true;
}

void ${ELEMENT_NAME}::activate() {
    // Implement activation logic, e.g., for hotkey triggers.
}

} // namespace ui

EOF

    # Append the new element's #include inside UIElements.h if not already included.
    if ! grep -q "#include \"UIElements/${ELEMENT_NAME}.h\"" "$UI_ELEMENTS_HEADER"; then
        echo "#include \"UIElements/${ELEMENT_NAME}.h\"" >> "$UI_ELEMENTS_HEADER"
    fi

    # Output update instructions.
    echo "Files created:"
    echo "  - ${HEADER_FILE}"
    echo "  - ${CPP_FILE}"
    echo ""
    echo "Updated ${UI_ELEMENTS_HEADER} with:"
    echo "  #include \"UIElements/${ELEMENT_NAME}.h\""
    echo ""
    echo "Be sure to update the following files:"
    echo "  // meson.build"
    echo "  'UIElements/${ELEMENT_NAME}.cpp',"

elif [ "$COMMAND" == "run" ]; then
    # Run meson build and demo.
    meson setup demos/build --reconfigure && meson compile -C demos/build && ./demos/build/sdl_ui_demo

elif [ "$COMMAND" == "print" ]; then
    INCLUDE_H=1
    INCLUDE_CPP=1
    IGNORE_LIST=()
    ADD_LIST=()

    while [[ $# -gt 0 ]]; do
        case "$1" in
            -h) INCLUDE_CPP=0 ;;
            -c) INCLUDE_H=0 ;;
            -i) shift; IGNORE_LIST+=("$1") ;;
            -a) shift; ADD_LIST+=("$1") ;;
            *) usage ;;
        esac
        shift
    done

    # If neither -h nor -c were specified, include both.
    if [ "$INCLUDE_H" -eq 0 ] && [ "$INCLUDE_CPP" -eq 0 ]; then
        INCLUDE_H=1
        INCLUDE_CPP=1
    fi

    # Find all .h and .cpp files in the project
    FILES=()
    while IFS= read -r file; do
        # Skip ignored directories and files unless they were added back
        for ignored in "${IGNORE_LIST[@]}"; do
            if [[ "$file" == *"$ignored"* ]] && [[ ! " ${ADD_LIST[*]} " =~ " $file " ]]; then
                continue 2
            fi
        done
        FILES+=("$file")
    done < <(find . -type f \( -name "*.h" -o -name "*.cpp" \))

    # Print the files with comments
    for file in "${FILES[@]}"; do
        echo "// file $(basename "$file") -->"
        cat "$file"
        echo "// <-- end file $(basename "$file")"
        echo ""
    done

else
    usage
fi
