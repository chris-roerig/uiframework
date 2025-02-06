#pragma once

#include "Theme/Color.h"
#include "Theme/ThemeBase.h"
#include "Theme/ThemeGlobals.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <vector>
#include <functional>
#include <memory>

namespace ui {

extern TTF_Font* globalFont;  // Remove the "static" qualifier.
void initFont();
void drawFilledRect(SDL_Renderer* renderer, const SDL_Rect &rect, const Color &color);
void drawLine(SDL_Renderer* renderer, int x1, int y1, int x2, int y2, const Color &color);

// Helper function for text rendering.
void renderText(SDL_Renderer* renderer, const std::string &text, int x, int y, const Color &color);

// Base class for all UI elements.
class UIElement {
public:
    int x, y, width, height;
    bool hasFocus = false;
    UIElement(int x_, int y_, int w_, int h_)
      : x(x_), y(y_), width(w_), height(h_) {}
    virtual ~UIElement() {}
    virtual void render(SDL_Renderer* renderer) = 0;
    virtual void handleEvent(const SDL_Event &e) {}
    virtual bool isInteractive() const { return false; }
    virtual SDL_Rect getFocusRect() const { return SDL_Rect{ x - 2, y - 2, width + 4, height + 4 }; }
    // virtual method for hot key activation.
    virtual void activate() { /* default does nothing */ }
};

// Canvas element for custom drawing.
class Canvas : public UIElement {
public:
    std::vector<std::function<void(SDL_Renderer*)>> drawCommands;
    Canvas(int x_, int y_, int w_, int h_) : UIElement(x_, y_, w_, h_) {}
    void render(SDL_Renderer* renderer) override;
    void rect(const SDL_Rect &rect, const Color &color);
    void line(int x1, int y1, int x2, int y2, const Color &color);
};

// UICore: Core UI engine.
class UICore {
public:
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    std::vector<std::shared_ptr<UIElement>> elements;
    int focusedIndex = -1;
    int width;
    int height;
    std::shared_ptr<Theme> currentTheme;
    
    // Flag indicating that a modal is active.
    bool modalActive = false;

    // Hot key registry: maps an SDL_Keycode to a callback function.
    std::unordered_map<SDL_Keycode, std::function<void()>> hotKeys;

    // Registers a hot key. Returns false if the key is already registered.
    bool registerHotKey(SDL_Keycode key, std::function<void()> callback) {
         if (hotKeys.find(key) != hotKeys.end())
              return false;
         hotKeys[key] = callback;
         return true;
    }
    // Static helper to convert a single-character string to SDL_Keycode (always lowercase)
    static SDL_Keycode keycodeFromString(const std::string &s);
    
    
    UICore(const char* title, int width, int height, std::shared_ptr<Theme> theme);
    ~UICore();
    void addElement(std::shared_ptr<UIElement> element);
    void setTheme(std::shared_ptr<Theme> theme);
    void setFocus(UIElement* element);
    void showQuitConfirmation(bool &quit);
    void run();
};
} // namespace ui
