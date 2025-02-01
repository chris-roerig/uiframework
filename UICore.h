#pragma once

#include "Color.h"
#include "ThemeBase.h"
#include "ThemeGlobals.h"  // Declares: extern std::shared_ptr<ui::Theme> g_currentTheme;
#include <SDL2/SDL.h>
#include <string>
#include <vector>
#include <functional>
#include <memory>

namespace ui {

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
};

// Label element.
class Label : public UIElement {
public:
    std::string text;
    Label(int x_, int y_, const std::string &text_)
      : UIElement(x_, y_, 0, 0), text(text_) {}
    void render(SDL_Renderer* renderer) override;
};

// Button element.
class Button : public UIElement {
public:
    std::string text;
    std::function<void()> onClick;
    Button(int x_, int y_, int w_, int h_, const std::string &text_, std::function<void()> callback)
      : UIElement(x_, y_, w_, h_), text(text_), onClick(callback) {}
    void render(SDL_Renderer* renderer) override;
    void handleEvent(const SDL_Event &e) override;
    bool isInteractive() const override { return true; }
};

// TextBox element.
// TextBox element.
class TextBox : public UIElement {
public:
    std::string content;
    bool textSelected;   // true if text is selected.
    bool autoHighlight;  // if true, auto-select text on focus (default true).
    TextBox(int x_, int y_, int w_, int h_, bool autoHighlight_ = true)
      : UIElement(x_, y_, w_, h_), content(""), textSelected(false), autoHighlight(autoHighlight_) {}
    void render(SDL_Renderer* renderer) override;
    void handleEvent(const SDL_Event &e) override;
    bool isInteractive() const override { return true; }
    SDL_Rect getFocusRect() const override;
    
    // Focus management:
    void onFocusGained();
    void onFocusLost();
};


// CheckBox element.
class CheckBox : public UIElement {
public:
    bool checked;
    std::function<void(bool)> onToggle;
    CheckBox(int x_, int y_, int size, bool initial = false, std::function<void(bool)> callback = nullptr)
      : UIElement(x_, y_, size, size), checked(initial), onToggle(callback) {}
    void render(SDL_Renderer* renderer) override;
    void handleEvent(const SDL_Event &e) override;
    bool isInteractive() const override { return true; }
};

// OptionSelect element.
class OptionSelect : public UIElement {
public:
    std::vector<std::string> options;
    int selectedIndex; // Used when expanded to indicate the highlighted option.
    int activeIndex;   // The currently chosen value.
    bool expanded;     // True when the drop-down is open.
    std::function<void(int)> onSelect;
    
    OptionSelect(int x_, int y_, int w_, int h_,
                 const std::vector<std::string>& opts,
                 int initial = 0,
                 int active = -1,
                 std::function<void(int)> callback = nullptr)
      : UIElement(x_, y_, w_, h_), options(opts), selectedIndex(initial),
        activeIndex(active == -1 ? initial : active), expanded(false), onSelect(callback) {}
    
    void render(SDL_Renderer* renderer) override;
    void handleEvent(const SDL_Event &e) override;
    
    // Override getFocusRect to use collapsed height when not expanded.
    SDL_Rect getFocusRect() const override;
    
    bool isInteractive() const override { return true; }
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
    std::shared_ptr<Theme> currentTheme;
    
    UICore(const char* title, int width, int height, std::shared_ptr<Theme> theme);
    ~UICore();
    void addElement(std::shared_ptr<UIElement> element);
    void setTheme(std::shared_ptr<Theme> theme);
    void run();
};

} // namespace ui
