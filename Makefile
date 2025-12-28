.PHONY: all build test clean setup

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

# Run specific test tags
test-button: build
	./build/ui_tests "[button]"

test-uicore: build
	./build/ui_tests "[uicore]"

test-label: build
	./build/ui_tests "[label]"

test-checkbox: build
	./build/ui_tests "[checkbox]"

test-progressbar: build
	./build/ui_tests "[progressbar]"

test-canvas: build
	./build/ui_tests "[canvas]"

test-theme: build
	./build/ui_tests "[theme]"

test-image: build
	./build/ui_tests "[image]"

test-sprite: build
	./build/ui_tests "[sprite]"

test-animatedsprite: build
	./build/ui_tests "[animatedsprite]"

test-listview: build
	./build/ui_tests "[listview]"

test-modal: build
	./build/ui_tests "[modal]"

test-contextmenu: build
	./build/ui_tests "[contextmenu]"

test-layout: build
	./build/ui_tests "[layout]"

test-textbox: build
	./build/ui_tests "[textbox]"

test-optionselect: build
	./build/ui_tests "[optionselect]"

demo-optionselect: build
	./build/optionselect_demo

test-optionselect-functionality: build
	./build/optionselect_test

# Help target
help:
	@echo "Available targets:"
	@echo "  all       - Build the project (default)"
	@echo "  build     - Compile the project"
	@echo "  test      - Run all tests"
	@echo "  test-button - Run only button tests"
	@echo "  test-uicore - Run only UICore tests"
	@echo "  test-label - Run only label tests"
	@echo "  test-checkbox - Run only checkbox tests"
	@echo "  test-progressbar - Run only progressbar tests"
	@echo "  test-canvas - Run only canvas tests"
	@echo "  test-theme - Run only theme tests"
	@echo "  test-image - Run only image tests"
	@echo "  test-sprite - Run only sprite tests"
	@echo "  test-animatedsprite - Run only animatedsprite tests"
	@echo "  test-listview - Run only listview tests"
	@echo "  test-modal - Run only modal tests"
	@echo "  test-contextmenu - Run only contextmenu tests"
	@echo "  test-layout - Run only layout tests"
	@echo "  test-textbox - Run only textbox tests"
	@echo "  test-optionselect - Run only optionselect tests"
	@echo "  demo-optionselect - Run OptionSelect demo"
	@echo "  clean     - Remove build directory"
	@echo "  rebuild   - Clean and rebuild from scratch"
	@echo "  setup     - Setup build directory"
	@echo "  help      - Show this help message"
