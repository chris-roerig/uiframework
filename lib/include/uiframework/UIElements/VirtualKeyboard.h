#pragma once
#include "InteractiveElement.h"
#include <vector>
#include <string>
#include <functional>
#include <chrono>

namespace ui {

enum class KeyboardMode {
    LOWERCASE,
    UPPERCASE, 
    NUMBERS,
    SPECIAL
};

class VirtualKeyboard : public InteractiveElement {
private:
    // Pre-built static character sets for performance
    static const std::vector<std::string> LOWERCASE_CHARS;
    static const std::vector<std::string> UPPERCASE_CHARS;
    static const std::vector<std::string> NUMBER_CHARS;
    static const std::vector<std::string> SPECIAL_CHARS;
    
    const std::vector<std::string>* currentChars = &LOWERCASE_CHARS;
    int selectedIndex = 0;
    KeyboardMode mode = KeyboardMode::LOWERCASE;
    std::function<void(char)> onCharInput;
    std::function<void()> onBackspace;
    std::function<void(int)> onCursorMove;
    std::chrono::steady_clock::time_point lastEnterTime;
    static constexpr int DOUBLE_PRESS_MS = 300;
    
    void updateCharacterSet();
    void cycleMode();
    void inputCurrentChar();
    void handleNavigation(SDL_Keycode key);
    void handleBackspace();
    void handleCursorMove(int direction);
    
public:
    VirtualKeyboard(int x_, int y_, int w_, int h_, std::function<void(char)> callback);
    
    void setMode(KeyboardMode newMode);
    KeyboardMode getMode() const { return mode; }
    void setSelectedIndex(int index);
    int getSelectedIndex() const { return selectedIndex; }
    
protected:
    void onMouseDown(int x, int y) override;
    void onKeyDown(const SDL_Keycode& key) override;
    void renderImpl(const RenderContext& ctx) override;
    
    // Set callbacks for text editing
    void setOnBackspace(std::function<void()> callback) { onBackspace = callback; }
    void setOnCursorMove(std::function<void(int)> callback) { onCursorMove = callback; }
};

} // namespace ui
