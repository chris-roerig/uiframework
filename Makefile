.PHONY: help all build test clean setup demo demos static-analysis

# Default target - show help
help:
	@echo "UI Framework - Available targets:"
	@echo "  help      - Show this help message (default)"
	@echo "  build     - Compile the project (library only)"
	@echo "  demos     - Compile demos and examples"
	@echo "  test      - Run all tests"
	@echo "  demo      - Run the progressive demo"
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

# Run static analysis
static-analysis:
	meson compile -C build static-analysis

# Legacy alias
all: build
