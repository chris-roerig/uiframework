#!/bin/bash
# Install UI Framework as system library

PREFIX=${1:-/usr/local}
LIB_DIR="$PREFIX/lib"
INCLUDE_DIR="$PREFIX/include/uiframework"

echo "Installing UI Framework to $PREFIX"

# Create directories
mkdir -p "$LIB_DIR"
mkdir -p "$INCLUDE_DIR"

# Build library
meson setup build_lib --buildtype=release
meson compile -C build_lib

# Copy library
cp build_lib/libuiframework.so "$LIB_DIR/" || cp build_lib/libuiframework.a "$LIB_DIR/"

# Copy headers
cp -r src/*.h "$INCLUDE_DIR/"
cp -r lib/ "$INCLUDE_DIR/"

# Create pkg-config file
cat > "$LIB_DIR/pkgconfig/uiframework.pc" << EOF
prefix=$PREFIX
libdir=\${prefix}/lib
includedir=\${prefix}/include

Name: UIFramework
Description: High-performance C++17 UI framework
Version: 1.0.0
Libs: -L\${libdir} -luiframework -lSDL2 -lSDL2_ttf -lSDL2_image
Cflags: -I\${includedir}
EOF

echo "Installation complete!"
