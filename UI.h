#pragma once

#include <SDL2/SDL.h>
#include <string>
#include <vector>
#include <functional>
#include <memory>

namespace ui {

// Color class for RGBA color and predefined themes.
class Color {
public:
    Uint8 r, g, b, a;
    Color(Uint8 red = 0, Uint8 green = 0, Uint8 blue = 0, Uint8 alpha = 255)
        : r(red), g(green), b(blue), a(alpha) {}

    // Predefined theme colors.
    static Color SolarizedDarkBackground() { return Color(0, 43, 54); }
    static Color SolarizedDarkForeground() { return Color(131, 148, 150); }
    static Color SolarizedDarkHighlight() { return Color(181, 137, 0); }
    static Color SolarizedLightBackground() { return Color(253, 246, 227); }
    static Color SolarizedLightForeground() { return Color(101, 123, 131); }
    static Color SolarizedLightHighlight() { return Color(203, 75, 22); }
    static Color MolokaiBackground() { return Color(39, 40, 34); }
    static Color MolokaiForeground() { return Color(248, 248, 242); }
    static Color MolokaiHighlight() { return Color(189, 147, 249); }
};

// Base class for all UI elements.
class UIElement {
public:
    int x, y, width, height;
    bool hasFocus = false;
    UIElement(int x_, int y_, int w_, int h_)
        : x(x_), y(y_), width(w_), height(h_) {}
    virtual ~UIElement() {}

    // Render the UI element.
    virtual void render(SDL_Renderer* renderer) = 0;
    // Handle keyboard events.
    virtual void handleEvent(const SDL_Event& e) {}
    // Determines if the element is interactive.
    virtual bool isInteractive() const { return false; }

    virtual SDL_Rect getFocusRect() const { 
      return SDL_Rect{ x - 2, y - 2, width + 4, height + 4 }; 
    }
};

// Label element (non-interactive).
class Label : public UIElement {
public:
    std::string text;
    Color fontColor;
    Label(int x_, int y_, const std::string& text_, const Color& color = Color::SolarizedDarkForeground())
        : UIElement(x_, y_, 0, 0), text(text_), fontColor(color) {}
    void render(SDL_Renderer* renderer) override;
};

// Button element (interactive).
class Button : public UIElement {
public:
    std::string text;
    Color fontColor;
    std::function<void()> onClick;
    Button(int x_, int y_, int w_, int h_, const std::string& text_,
           std::function<void()> callback,
           const Color& fontColor_ = Color::SolarizedDarkForeground())
        : UIElement(x_, y_, w_, h_), text(text_), fontColor(fontColor_), onClick(callback) {}
    void render(SDL_Renderer* renderer) override;
    void handleEvent(const SDL_Event& e) override;
    bool isInteractive() const override { return true; }
};

// TextBox element (interactive).
class TextBox : public UIElement {
public:
    std::string content;
    Color fontColor;
    TextBox(int x_, int y_, int w_, int h_,
            const Color& fontColor_ = Color::SolarizedDarkForeground())
        : UIElement(x_, y_, w_, h_), fontColor(fontColor_) {}
    void render(SDL_Renderer* renderer) override;
    void handleEvent(const SDL_Event& e) override;
    bool isInteractive() const override { return true; }
    SDL_Rect getFocusRect() const override;
};

// CheckBox element (interactive).
class CheckBox : public UIElement {
public:
    bool checked;
    std::function<void(bool)> onToggle;
    CheckBox(int x_, int y_, int size, bool initial = false,
             std::function<void(bool)> callback = nullptr)
        : UIElement(x_, y_, size, size), checked(initial), onToggle(callback) {}
    void render(SDL_Renderer* renderer) override;
    void handleEvent(const SDL_Event& e) override;
    bool isInteractive() const override { return true; }
};

// OptionSelect element (interactive).
class OptionSelect : public UIElement {
public:
    std::vector<std::string> options;
    int selectedIndex;  // For interactive focus
    int activeIndex;    // The currently active (selected) setting
    std::function<void(int)> onSelect;
    // If 'active' is not provided (or -1), the activeIndex defaults to initial.
    OptionSelect(int x_, int y_, int w_, int h_,
                 const std::vector<std::string>& opts,
                 int initial = 0,
                 int active = -1,
                 std::function<void(int)> callback = nullptr)
        : UIElement(x_, y_, w_, h_), options(opts), selectedIndex(initial),
          activeIndex(active == -1 ? initial : active), onSelect(callback) {}
    void render(SDL_Renderer* renderer) override;
    void handleEvent(const SDL_Event& e) override;
    bool isInteractive() const override { return true; }
};


// Canvas element for custom drawing (non-interactive).
class Canvas : public UIElement {
public:
    // Store drawing commands as lambdas.
    std::vector<std::function<void(SDL_Renderer*)>> drawCommands;
    Canvas(int x_, int y_, int w_, int h_)
        : UIElement(x_, y_, w_, h_) {}
    void render(SDL_Renderer* renderer) override;
    // Drawing functions.
    void rect(const SDL_Rect& rect, const Color& color);
    void line(int x1, int y1, int x2, int y2, const Color& color);
};

// UI Manager class to handle global settings and UI elements.
class UIManager {
public:
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    Color backgroundColor;
    Color fontColor;
    Color highlightColor;
    std::vector<std::shared_ptr<UIElement>> elements;
    // For simplicity, we manage focus by index.
    int focusedIndex = -1;

    UIManager(const char* title, int width, int height,
              const Color& bg = Color::SolarizedDarkBackground(),
              const Color& fg = Color::SolarizedDarkForeground());
    ~UIManager();

    // Add UI elements.
    void addElement(std::shared_ptr<UIElement> element);

    // Set colors.
    void setBackgroundColor(const Color& color);
    void setFontColor(const Color& color);
    void setHighlightColor(const Color& color);

    // Main loop.
    void run();
};

} // namespace ui

