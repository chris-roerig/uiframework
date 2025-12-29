#!/bin/bash

# UI Framework Test and Demo Runner
echo "=== UI Framework Build and Test Script ==="

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Function to print colored output
print_status() {
    echo -e "${GREEN}[INFO]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[WARN]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

# Check if meson is available
if ! command -v meson &> /dev/null; then
    print_error "Meson build system not found. Please install meson."
    exit 1
fi

# Check if ninja is available
if ! command -v ninja &> /dev/null; then
    print_warning "Ninja not found, using default backend"
fi

# Clean previous build
if [ -d "build" ]; then
    print_status "Cleaning previous build..."
    rm -rf build
fi

# Setup build directory
print_status "Setting up build directory..."
if ! meson setup build; then
    print_error "Failed to setup build directory"
    exit 1
fi

# Compile
print_status "Compiling project..."
if ! meson compile -C build; then
    print_error "Compilation failed"
    exit 1
fi

print_status "Build completed successfully!"

# Run tests if requested
if [ "$1" = "test" ] || [ "$1" = "all" ]; then
    print_status "Running tests..."
    echo ""
    
    if [ -f "build/ui_tests" ]; then
        ./build/ui_tests
        test_result=$?
        
        if [ $test_result -eq 0 ]; then
            print_status "All tests passed!"
        else
            print_error "Some tests failed (exit code: $test_result)"
        fi
    else
        print_error "Test executable not found"
        exit 1
    fi
fi

# Run demo if requested
if [ "$1" = "demo" ] || [ "$1" = "all" ] || [ -z "$1" ]; then
    print_status "Starting demo application..."
    echo ""
    
    if [ -f "build/sdl_ui_demo" ]; then
        ./build/sdl_ui_demo
    else
        print_error "Demo executable not found"
        exit 1
    fi
fi

print_status "Script completed!"
