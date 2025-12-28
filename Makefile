.PHONY: all build test clean setup demo

# Default target
all: build

# Setup build directory
setup:
	meson setup build --wipe

# Build the project
build:
	meson compile -C build

# Run tests
test: build
	./build/ui_tests

# Clean build directory
clean:
	rm -rf build

# Rebuild from scratch
rebuild: clean setup build

# Run our demo app
demo: build
	./build/context_menu_demo

# Help target
help:
	@echo "Available targets:"
	@echo "  all       - Build the project (default)"
	@echo "  build     - Compile the project"
	@echo "  test      - Run all tests"
	@echo "  demo      - Run the context menu + grid demo"
	@echo "  clean     - Remove build directory"
	@echo "  rebuild   - Clean and rebuild from scratch"
	@echo "  setup     - Setup build directory"
	@echo "  help      - Show this help message"
