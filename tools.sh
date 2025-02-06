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
#include "../UICore.h"
#include <vector>
#include <string>
#include <functional>

namespace ui {

class ${ELEMENT_NAME} : public UIElement {
public:
    ${ELEMENT_NAME}(int x_, int y_) : UIElement(x_, y_, 0, 0) {}
    void render(SDL_Renderer* renderer) override;
    void handleEvent(const SDL_Event &e) override;
    bool isInteractive() const override { return true; }
    virtual void activate() override;
};

} // namespace ui
EOF

    # Generate the cpp file.
    cat > "$CPP_FILE" <<EOF
#include "${ELEMENT_NAME}.h"
#include "../Theme/ThemeGlobals.h"
#include "../Theme/ThemeBase.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>

namespace ui {

void ${ELEMENT_NAME}::render(SDL_Renderer* renderer) {
EOF

    # If theme flag is enabled, add theme line.
    if [ "$THEME_FLAG" -eq 1 ]; then
        echo "    ThemeableElementColors tc = g_currentTheme->${ELEMENT_NAME_LOWER}Colors();" >> "$CPP_FILE"
    fi

    # Close out the render function and add additional functions.
    cat >> "$CPP_FILE" <<EOF
}

void ${ELEMENT_NAME}::handleEvent(const SDL_Event &e) {
  // implement event handling here
}

void ${ELEMENT_NAME}::activate() {
  // implement activate action here. typically used when triggered by hotkey
}

} // namespace ui
EOF

    # Output update instructions.
    echo "Files created:"
    echo "  - ${HEADER_FILE}"
    echo "  - ${CPP_FILE}"
    echo ""
    echo "Be sure to update the following files:"
    echo "  // meson.build"
    echo "  'UIElements/${ELEMENT_NAME}.cpp',"
    echo ""
    echo "  // UI.cpp, UI.h, UICore.cpp"
    echo "  #include \"UIElements/${ELEMENT_NAME}.h\""

    
    if [ "$THEME_FLAG" -eq 1 ]; then
        echo ""
        echo "Also update Theme/ThemeBase.h with the following additions:"
        echo "  Color ${ELEMENT_NAME_LOWER}Background;"
        echo "  Color ${ELEMENT_NAME_LOWER}Foreground;"
        echo ""
        echo "  virtual ThemeableElementColors ${ELEMENT_NAME_LOWER}Colors() const = 0"
        echo ""
        echo "  Color default${ELEMENT_NAME}Background;"
        echo "  Color default${ELEMENT_NAME}Foreground;"
        echo ""
        echo "      ThemeableElementColors ${ELEMENT_NAME_LOWER}Colors() const override {"
        echo "          ThemeableElementColors c;"
        echo "          c.${ELEMENT_NAME_LOWER}Background = default${ELEMENT_NAME}Background;"
        echo "          c.${ELEMENT_NAME_LOWER}Foreground = default${ELEMENT_NAME}Foreground;"
        echo "          return c;"
        echo "      }"
    fi

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
