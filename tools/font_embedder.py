#!/usr/bin/env python3
"""
Font Embedder Tool - Converts TTF fonts to C++ header files
Part of Phase 0: Multi-Font Embedding System
"""

import sys
import os
from pathlib import Path

def convert_font_to_header(ttf_path, output_path, font_name):
    """Convert TTF file to C++ header with byte array"""
    try:
        with open(ttf_path, 'rb') as f:
            font_data = f.read()
    except FileNotFoundError:
        print(f"Error: Font file not found: {ttf_path}")
        return False
    except Exception as e:
        print(f"Error reading font file {ttf_path}: {e}")
        return False
    
    # Generate header content
    header_content = f"""#pragma once
// Auto-generated font embedding for {font_name}
// Source: {os.path.basename(ttf_path)}
// Size: {len(font_data)} bytes

namespace ui::embedded {{

const unsigned char {font_name}_data[] = {{
"""
    
    # Add byte data in rows of 16 for readability
    for i in range(0, len(font_data), 16):
        chunk = font_data[i:i+16]
        hex_values = ', '.join(f'0x{b:02x}' for b in chunk)
        header_content += f"    {hex_values}"
        if i + 16 < len(font_data):
            header_content += ","
        header_content += "\n"
    
    header_content += f"""
}};

const unsigned int {font_name}_size = {len(font_data)};

}} // namespace ui::embedded
"""
    
    # Write header file
    try:
        os.makedirs(os.path.dirname(output_path), exist_ok=True)
        with open(output_path, 'w') as f:
            f.write(header_content)
        print(f"Generated: {output_path} ({len(font_data)} bytes)")
        return True
    except Exception as e:
        print(f"Error writing header file {output_path}: {e}")
        return False

def main():
    if len(sys.argv) != 4:
        print("Usage: font_embedder.py <input.ttf> <output.h> <font_name>")
        print("Example: font_embedder.py fonts/roboto.ttf include/RobotoRegular.h RobotoRegular")
        sys.exit(1)
    
    ttf_path = sys.argv[1]
    output_path = sys.argv[2]
    font_name = sys.argv[3]
    
    # Validate font name (C++ identifier)
    if not font_name.replace('_', '').isalnum() or font_name[0].isdigit():
        print(f"Error: Font name '{font_name}' is not a valid C++ identifier")
        sys.exit(1)
    
    success = convert_font_to_header(ttf_path, output_path, font_name)
    sys.exit(0 if success else 1)

if __name__ == "__main__":
    main()
