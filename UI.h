#pragma once
#include "UICore.h"
#include <memory>
#include <functional>
#include <vector>
#include <string>

class UI {
private:
    std::unique_ptr<ui::UICore> core;
public:
    UI(const char* title, int width, int height);
    ~UI();

    // Convenience widget methods:
    void label(const std::string &text, int x, int y);
    void button(const std::string &text, int x, int y, std::function<void()> callback);
    void textBox(const std::string &defaultText, int x, int y);
    void checkBox(bool state, int x, int y, std::function<void(bool)> callback);
    void optionSelect(int current, const std::vector<std::string> &options, int x, int y, std::function<void(int)> callback);
    ui::Canvas* canvas(int x, int y, int width, int height);

    // Allow dynamic addition of elements (for our TODO items).
    void addElement(std::shared_ptr<ui::UIElement> element);

    // Run the UI loop.
    void run();

    // Theme setter—sets the theme for the entire UI.
    void setTheme(const std::string &themeName);
};
