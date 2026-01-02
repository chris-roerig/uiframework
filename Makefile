.PHONY: help all build test clean setup static-analysis demo

# Default target - show help
help:
	@echo "UI Framework - Available targets:"
	@echo "  help      - Show this help message (default)"
	@echo "  build     - Compile the project (library only)"
	@echo "  test      - Run all tests"
	@echo "  demo      - Build and run grid system demo"
	@echo "  static-analysis - Run static analysis tools"
	@echo "  clean     - Remove build directory"
	@echo "  rebuild   - Clean and rebuild from scratch"
	@echo "  setup     - Setup build directory"

# Build the project (library only)
build:
	@echo "Building UI Framework library..."
	@meson compile -C build

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

# Run static analysis
static-analysis:
	meson compile -C build static-analysis

# Build and run grid system demo
demo: build
	meson compile -C build demo
	./build/demo

# Legacy alias
all: build
