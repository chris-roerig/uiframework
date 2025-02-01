#pragma once

#include "UI.h"
#include <memory>
#include <functional>
#include <vector>
#include <string>

class UIAbstraction {
private:
    std::unique_ptr<ui::UIManager> manager;
public:
    // Construct with window title and dimensions.
    UIAbstraction(const char* title, int width, int height);
    ~UIAbstraction();

    // Convenience methods with opinionated default sizes:
    void label(const std::string &text, int x, int y);
    void button(const std::string &text, int x, int y, std::function<void()> callback);
    void textBox(const std::string &defaultText, int x, int y);
    void checkBox(bool state, int x, int y, std::function<void(bool)> callback);
    void optionSelect(int current, const std::vector<std::string> &options, int x, int y, std::function<void(int)> callback);
    ui::Canvas* canvas(int x, int y, int width, int height);

    void setBackgroundColor(const ui::Color &color);
    void setFontColor(const ui::Color &color);
    void setHighlightColor(const ui::Color &color);

    // Run the UI loop.
    void run();
};

