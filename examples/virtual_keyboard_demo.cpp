#include "../src/UI.h" #include <iostream>
#include <string>

int main() {
    try {
        UI ui("Virtual Keyboard Demo", 800, 600);
        
        // Title
        auto title = ui.createLabel("Virtual Keyboard Demo", 300, 20);
        
        // Instructions (split into multiple lines)
        auto instructions1 = ui.createLabel("Arrow keys: navigate, Shift: cycle modes, Enter: input", 50, 60);
        auto instructions2 = ui.createLabel("Shift+Enter: backspace, Ctrl+Left/Right: move cursor", 50, 80);
        
        // Text display area with cursor support
        std::string inputText = "";
        int cursorPos = 0;
        auto textDisplay = ui.createLabel("Input: " + inputText, 50, 110);
        
        auto updateDisplay = [&]() {
            std::string displayText = "Input: " + inputText;
            if (cursorPos < inputText.length()) {
                displayText.insert(cursorPos + 7, "|"); // +7 for "Input: " prefix
            } else {
                displayText += "|";
            }
            textDisplay->setText(displayText);
        };
        
        updateDisplay();
        
        // Virtual keyboard with text editing support
        auto keyboard = ui.createVirtualKeyboard(50, 140, 200, 100, [&](char c) {
            inputText.insert(cursorPos, 1, c);
            cursorPos++;
            updateDisplay();
            std::cout << "Character input: '" << c << "' at position " << (cursorPos-1) << std::endl;
        });
        
        // Set up backspace callback
        keyboard->setOnBackspace([&]() {
            if (cursorPos > 0) {
                inputText.erase(cursorPos - 1, 1);
                cursorPos--;
                updateDisplay();
                std::cout << "Backspace at position " << cursorPos << std::endl;
            }
        });
        
        // Set up cursor movement callback (using Ctrl+Left/Right)
        keyboard->setOnCursorMove([&](int direction) {
            cursorPos += direction;
            cursorPos = std::max(0, std::min(cursorPos, static_cast<int>(inputText.length())));
            updateDisplay();
            std::cout << "Cursor moved to position " << cursorPos << std::endl;
        });
        
        // Clear button
        auto clearBtn = ui.createButton("Clear", 50, 380, [&]() {
            inputText.clear();
            cursorPos = 0;
            updateDisplay();
            std::cout << "Text cleared" << std::endl;
        });
        
        // Mode display
        auto modeDisplay = ui.createLabel("Mode: Lowercase", 400, 380);
        
        // Set focus to keyboard
        ui.setFocus(keyboard);
        
        std::cout << "Virtual Keyboard Demo Controls:" << std::endl;
        std::cout << "- Arrow keys: Navigate characters" << std::endl;
        std::cout << "- Shift: Cycle between character sets (abc -> ABC -> 123 -> !@#)" << std::endl;
        std::cout << "- Enter: Input selected character" << std::endl;
        std::cout << "- Double Enter (quick): Input space character" << std::endl;
        std::cout << "- Shift + Enter: Delete previous character (backspace)" << std::endl;
        std::cout << "- Ctrl + Left/Right: Move cursor left/right" << std::endl;
        std::cout << "- Click Clear button to clear input" << std::endl;
        
        ui.run();
        
    } catch (const ui::UIException& e) {
        std::cerr << "UI Error: " << e.what() << std::endl;
        return -1;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    
    return 0;
}
