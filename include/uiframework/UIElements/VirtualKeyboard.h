#pragma once
#include "UIElement.h"
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

class VirtualKeyboard : public UIElement {
private:
    std::vector<std::string> currentChars;
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
    
    void render(SDL_Renderer* renderer, TTF_Font* font, std::shared_ptr<class Theme> theme) override;
    void handleEvent(const SDL_Event &e) override;
    bool isInteractive() const override { return true; }
    
    void setMode(KeyboardMode newMode);
    KeyboardMode getMode() const { return mode; }
    void setSelectedIndex(int index);
    int getSelectedIndex() const { return selectedIndex; }
    
    // Set callbacks for text editing
    void setOnBackspace(std::function<void()> callback) { onBackspace = callback; }
    void setOnCursorMove(std::function<void(int)> callback) { onCursorMove = callback; }
};

} // namespace ui
