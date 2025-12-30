.PHONY: help all build test clean setup demo demos static-analysis dsl-demo layout audio-demo

# Default target - show help
help:
	@echo "UI Framework - Available targets:"
	@echo "  help      - Show this help message (default)"
	@echo "  build     - Compile the project (library only)"
	@echo "  demos     - Compile demos and examples"
	@echo "  test      - Run all tests"
	@echo "  demo      - Run the progressive demo"
	@echo "  dsl-demo  - Build and run the DSL demo"
	@echo "  layout    - Build and run the layout designer tool (use FILE=path to load layout)"
	@echo "  audio-demo - Build and run the audio studio demo"
	@echo "  static-analysis - Run static analysis tools"
	@echo "  clean     - Remove build directory"
	@echo "  rebuild   - Clean and rebuild from scratch"
	@echo "  setup     - Setup build directory"

# Build the project (library only - excludes demos)
build:
	@echo "Building UI Framework library (excluding demos)..."
	@meson compile -C build | grep -v progressive_test || true

# Build demos and examples
demos:
	meson compile -C build progressive_test

# Run tests
test: build
	meson compile -C build ui_tests
	./build/ui_tests

# Clean build directory
clean:
	rm -rf build

# Setup build directory
setup:
	meson setup build --wipe

# Rebuild from scratch
rebuild: clean setup build

# Run demo app
demo: demos
	./build/progressive_test

# Build and run DSL demo
dsl-demo:
	./tools/ui-compiler dev/examples/demo.ui dev/examples/demo_ui.h
	meson compile -C build dsl_demo
	./build/dsl_demo

# Build and run keyboard layout designer tool
layout:
	@echo "Building and running keyboard layout designer..."
	@cd examples/layout_designer && meson compile -C build
	@cd examples/layout_designer && ./build/keyboard_layout_designer $(FILE)

# Build and run audio studio demo
audio-demo:
	@echo "Building and running audio studio demo..."
	@cd examples && meson compile -C build
	@cd examples && ./build/audio_studio_demo

# Run static analysis
static-analysis:
	meson compile -C build static-analysis

# Legacy alias
all: build
