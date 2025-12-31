#include "uiframework/UIElements/VirtualKeyboard.h"
#include "uiframework/Theme/ThemeBase.h"
#include "uiframework/Helpers.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <algorithm>

namespace ui {

// Pre-build character sets for optimal performance
const std::vector<std::string> VirtualKeyboard::LOWERCASE_CHARS = [](){
    std::vector<std::string> chars;
    for (char c = 'a'; c <= 'z'; ++c) {
        chars.push_back(std::string(1, c));
    }
    return chars;
}();

const std::vector<std::string> VirtualKeyboard::UPPERCASE_CHARS = [](){
    std::vector<std::string> chars;
    for (char c = 'A'; c <= 'Z'; ++c) {
        chars.push_back(std::string(1, c));
    }
    return chars;
}();

const std::vector<std::string> VirtualKeyboard::NUMBER_CHARS = [](){
    std::vector<std::string> chars;
    for (char c = '0'; c <= '9'; ++c) {
        chars.push_back(std::string(1, c));
    }
    return chars;
}();

const std::vector<std::string> VirtualKeyboard::SPECIAL_CHARS = [](){
    std::vector<std::string> chars;
    for (char c : "!@#$%^&*()_+-=[]{}|;:,.<>?") {
        chars.push_back(std::string(1, c));
    }
    return chars;
}();

VirtualKeyboard::VirtualKeyboard(int x_, int y_, int w_, int h_, std::function<void(char)> callback)
    : InteractiveElement(x_, y_, w_, h_), onCharInput(callback) {
    updateCharacterSet();
}

void VirtualKeyboard::updateCharacterSet() {
    // Simply point to the appropriate pre-built character set
    switch (mode) {
        case KeyboardMode::LOWERCASE: currentChars = &LOWERCASE_CHARS; break;
        case KeyboardMode::UPPERCASE: currentChars = &UPPERCASE_CHARS; break;
        case KeyboardMode::NUMBERS: currentChars = &NUMBER_CHARS; break;
        case KeyboardMode::SPECIAL: currentChars = &SPECIAL_CHARS; break;
    }
    
    selectedIndex = std::min(selectedIndex, static_cast<int>(currentChars->size()) - 1);
    if (selectedIndex < 0) selectedIndex = 0;
}

void VirtualKeyboard::cycleMode() {
    switch (mode) {
        case KeyboardMode::LOWERCASE: mode = KeyboardMode::UPPERCASE; break;
        case KeyboardMode::UPPERCASE: mode = KeyboardMode::NUMBERS; break;
        case KeyboardMode::NUMBERS: mode = KeyboardMode::SPECIAL; break;
        case KeyboardMode::SPECIAL: mode = KeyboardMode::LOWERCASE; break;
    }
    updateCharacterSet();
}

void VirtualKeyboard::inputCurrentChar() {
    if (onCharInput && selectedIndex >= 0 && selectedIndex < currentChars->size()) {
        onCharInput((*currentChars)[selectedIndex][0]);
    }
}

void VirtualKeyboard::handleBackspace() {
    if (onBackspace) {
        onBackspace();
    }
}

void VirtualKeyboard::handleCursorMove(int direction) {
    if (onCursorMove) {
        onCursorMove(direction);
    }
}

void VirtualKeyboard::handleNavigation(SDL_Keycode key) {
    switch (key) {
        case SDLK_LEFT:
            selectedIndex = (selectedIndex - 1 + currentChars->size()) % currentChars->size();
            break;
        case SDLK_RIGHT:
            selectedIndex = (selectedIndex + 1) % currentChars->size();
            break;
        case SDLK_UP:
            selectedIndex = std::max(0, selectedIndex - ui::Constants::KEYBOARD_NAV_STEP);
            break;
        case SDLK_DOWN:
            selectedIndex = std::min(static_cast<int>(currentChars->size()) - 1, selectedIndex + ui::Constants::KEYBOARD_NAV_STEP);
            break;
    }
}

void VirtualKeyboard::renderImpl(const RenderContext& ctx) {
    auto colors = ctx.textInputColors();
    
    // Calculate proper height based on text size
    int textHeight = TTF_FontLineSkip(ctx.font);
    int actualHeight = textHeight * 3 + 40; // 3 rows of text + padding
    
    // Draw background
    SDL_Rect bgRect = {x, y, width, actualHeight};
    drawFilledRect(ctx.renderer, bgRect, colors.textInputBackground);
    
    // Draw border
    SDL_SetRenderDrawColor(ctx.renderer, colors.textInputBorderDark.r, colors.textInputBorderDark.g, 
                           colors.textInputBorderDark.b, colors.textInputBorderDark.a);
    SDL_RenderDrawRect(ctx.renderer, &bgRect);
    
    if (currentChars->empty()) return;
    
    // Calculate character display parameters
    const int padding = 5;
    const int charSpacing = 25;
    int startX = x + padding;
    int currentX = startX;
    int currentY = y + padding;
    
    // Render characters
    for (size_t i = 0; i < currentChars->size(); ++i) {
        bool isSelected = (i == selectedIndex);
        
        // Wrap to next line if needed
        if (currentX + charSpacing > x + width - padding) {
            currentX = startX;
            currentY += textHeight + 5;
            if (currentY > y + actualHeight - textHeight - 5) break; // Stop if we run out of space
        }
        
        // Draw character with simple color inversion for selection
        SDL_Color textColor, bgColor;
        if (isSelected) {
            // Selected: invert colors (background becomes text, text becomes background)
            textColor = {colors.textInputBackground.r, colors.textInputBackground.g, 
                        colors.textInputBackground.b, colors.textInputBackground.a};
            bgColor = {colors.textInputText.r, colors.textInputText.g, 
                      colors.textInputText.b, colors.textInputText.a};
        } else {
            // Normal: standard colors
            textColor = {colors.textInputText.r, colors.textInputText.g, 
                        colors.textInputText.b, colors.textInputText.a};
            bgColor = {colors.textInputBackground.r, colors.textInputBackground.g, 
                      colors.textInputBackground.b, colors.textInputBackground.a};
        }
        
        SDL_Surface* surface = TTF_RenderText_Solid(ctx.font, (*currentChars)[i].c_str(), textColor);
        if (surface) {
            SDL_Texture* texture = SDL_CreateTextureFromSurface(ctx.renderer, surface);
            if (texture) {
                // Draw background for selected character (simple inversion)
                if (isSelected) {
                    SDL_Rect charBgRect = {currentX, currentY, charSpacing, textHeight + 4};
                    Color bg(bgColor.r, bgColor.g, bgColor.b, bgColor.a);
                    drawFilledRect(ctx.renderer, charBgRect, bg);
                }
                
                SDL_Rect dst = {currentX + 5, currentY + 2, surface->w, surface->h};
                SDL_RenderCopy(ctx.renderer, texture, nullptr, &dst);
                SDL_DestroyTexture(texture);
            }
            SDL_FreeSurface(surface);
        }
        
        currentX += charSpacing;
    }
    
    // Draw mode indicator
    std::string modeText;
    switch (mode) {
        case KeyboardMode::LOWERCASE: modeText = "abc"; break;
        case KeyboardMode::UPPERCASE: modeText = "ABC"; break;
        case KeyboardMode::NUMBERS: modeText = "123"; break;
        case KeyboardMode::SPECIAL: modeText = "!@#"; break;
    }
    
    SDL_Color modeColor = {colors.textInputText.r, colors.textInputText.g, 
                          colors.textInputText.b, colors.textInputText.a};
    SDL_Surface* modeSurface = TTF_RenderText_Solid(ctx.font, modeText.c_str(), modeColor);
    if (modeSurface) {
        SDL_Texture* modeTexture = SDL_CreateTextureFromSurface(ctx.renderer, modeSurface);
        if (modeTexture) {
            SDL_Rect modeDst = {x + width - 40, y + 5, modeSurface->w, modeSurface->h};
            SDL_RenderCopy(ctx.renderer, modeTexture, nullptr, &modeDst);
            SDL_DestroyTexture(modeTexture);
        }
        SDL_FreeSurface(modeSurface);
    }
}

// VirtualKeyboard event handling moved to InteractiveElement base class

void VirtualKeyboard::onMouseDown(int x, int y) {
    if (e.type == SDL_KEYDOWN) {
        SDL_Keymod mods = SDL_GetModState();
        bool shiftPressed = (mods & KMOD_SHIFT) != 0;
        
        switch (e.key.keysym.sym) {
            case SDLK_LSHIFT:
            case SDLK_RSHIFT:
                cycleMode();
                break;
                
            case SDLK_RETURN:
            case SDLK_KP_ENTER: {
                if (shiftPressed) {
                    // Shift + Enter = backspace
                    handleBackspace();
                } else {
                    auto now = std::chrono::steady_clock::now();
                    auto timeSinceLastEnter = std::chrono::duration_cast<std::chrono::milliseconds>(
                        now - lastEnterTime).count();
                    
                    if (timeSinceLastEnter < DOUBLE_PRESS_MS) {
                        // Double press - send space
                        if (onCharInput) onCharInput(' ');
                    } else {
                        // Single press - input current character
                        inputCurrentChar();
                    }
                    lastEnterTime = now;
                }
                break;
            }
            
            case SDLK_LEFT:
            case SDLK_RIGHT:
                if (mods & KMOD_CTRL) {
                    // Ctrl + Left/Right = move cursor
                    int direction = (e.key.keysym.sym == SDLK_LEFT) ? -1 : 1;
                    handleCursorMove(direction);
                } else {
                    // Regular navigation
                    handleNavigation(e.key.keysym.sym);
                }
                break;
                
            case SDLK_UP:
            case SDLK_DOWN:
                handleNavigation(e.key.keysym.sym);
                break;
        }
    }
}

void VirtualKeyboard::onMouseDown(int x, int y) {
    // VirtualKeyboard primarily uses keyboard input
}

void VirtualKeyboard::setMode(KeyboardMode newMode) {
    mode = newMode;
    updateCharacterSet();
}

void VirtualKeyboard::setSelectedIndex(int index) {
    if (index >= 0 && index < currentChars->size()) {
        selectedIndex = index;
    }
}

} // namespace ui
