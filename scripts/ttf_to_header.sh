#!/bin/bash
# Convert TTF font to C header file

if [ $# -ne 2 ]; then
    echo "Usage: $0 <input.ttf> <output.h>"
    exit 1
fi

INPUT="$1"
OUTPUT="$2"
BASENAME=$(basename "$INPUT" .ttf)
VARNAME="${BASENAME}_font_data"

echo "Converting $INPUT to $OUTPUT..."

# Generate header
cat > "$OUTPUT" << EOF
#pragma once

namespace ui {
namespace embedded {

// Embedded font data from $INPUT
const unsigned char ${VARNAME}[] = {
EOF

# Convert binary to hex array
xxd -i < "$INPUT" | grep -v "unsigned" | sed 's/^//' >> "$OUTPUT"

# Get file size
SIZE=$(wc -c < "$INPUT" | tr -d ' ')

# Add size and closing
cat >> "$OUTPUT" << EOF
};

const unsigned int ${BASENAME}_font_size = ${SIZE};

} // namespace embedded
} // namespace ui
EOF

echo "Generated $OUTPUT with variable ${VARNAME} (${SIZE} bytes)"
