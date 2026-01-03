.PHONY: help all build test clean setup static-analysis demo layout-editor audio-recorder

# Default target - show help
help:
	@echo "UI Framework - Available targets:"
	@echo "  help         - Show this help message (default)"
	@echo "  build        - Compile the project (library only)"
	@echo "  test         - Run all tests (with monitoring)"
	@echo "  demo         - Build and run grid system demo"
	@echo "  layout-editor - Build and run UI layout editor tool"
	@echo "  audio-recorder - Build and run audio recorder example app"
	@echo "  static-analysis - Run static analysis tools"
	@echo "  clean        - Remove build directory"
	@echo "  rebuild      - Clean and rebuild from scratch"
	@echo "  setup        - Setup build directory"

# Build the project (library only)
build:
	@echo "Building UI Framework library..."
	@meson compile -C build

# Run tests with monitoring (memory leak detection and timeout protection)
test: build
	meson compile -C build ui_tests
	@echo "Starting monitored test run..."
	@./build/ui_tests & \
	PID=$$!; \
	echo "Test PID: $$PID"; \
	(sleep 300; kill $$PID 2>/dev/null; echo "Test killed after 5min timeout") & \
	TIMEOUT_PID=$$!; \
	while kill -0 $$PID 2>/dev/null; do \
		MEM=$$(ps -p $$PID -o rss= 2>/dev/null | tr -d ' '); \
		if [ "$$MEM" -gt 1048576 ]; then \
			echo "Memory limit exceeded (>1GB), killing process"; \
			kill $$PID; \
			break; \
		fi; \
		sleep 2; \
	done; \
	wait $$PID; \
	EXIT_CODE=$$?; \
	kill $$TIMEOUT_PID 2>/dev/null; \
	echo "Test completed with exit code: $$EXIT_CODE"; \
	exit $$EXIT_CODE

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

# Build and run UI layout editor tool
layout-editor: build
	meson compile -C build layout_editor
	./build/layout_editor

# Build and run audio recorder example app
audio-recorder: build
	meson compile -C build audio_recorder_app
	./build/audio_recorder_app

# Legacy alias
all: build
