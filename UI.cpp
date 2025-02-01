#include "UI.h"
#include "ThemeFrameworkDefault.h"
#include "ThemeSolarizedDark.h"
#include "ThemeSolarizedLight.h"
#include "ThemeMolokai.h"
#include <iostream>

UI::UI(const char* title, int width, int height) {
    // Use our custom default theme.
    core = std::make_unique<ui::UICore>(title, width, height, std::make_shared<ui::ThemeFrameworkDefault>());
}

UI::~UI() {}

void UI::label(const std::string &text, int x, int y) {
    auto lbl = std::make_shared<ui::Label>(x, y, text);
    core->addElement(lbl);
}

void UI::button(const std::string &text, int x, int y, std::function<void()> callback) {
    // Default size: 200x50.
    auto btn = std::make_shared<ui::Button>(x, y, 200, 50, text, callback);
    core->addElement(btn);
}

void UI::textBox(const std::string &defaultText, int x, int y) {
    auto tb = std::make_shared<ui::TextBox>(x, y, 200, 50);
    tb->content = defaultText;
    core->addElement(tb);
}

void UI::checkBox(bool state, int x, int y, std::function<void(bool)> callback) {
    auto cb = std::make_shared<ui::CheckBox>(x, y, 30, state, callback);
    core->addElement(cb);
}

void UI::optionSelect(int current, const std::vector<std::string> &options, int x, int y, std::function<void(int)> callback) {
    int height = static_cast<int>(options.size()) * 50;
    auto os = std::make_shared<ui::OptionSelect>(x, y, 200, height, options, current, current, callback);
    core->addElement(os);
}

ui::Canvas* UI::canvas(int x, int y, int width, int height) {
    auto cnv = std::make_shared<ui::Canvas>(x, y, width, height);
    core->addElement(cnv);
    return cnv.get();
}

// New method to add UI elements dynamically.
void UI::addElement(std::shared_ptr<ui::UIElement> element) {
    core->addElement(element);
}

void UI::run() {
    core->run();
}

void UI::setTheme(const std::string &themeName) {
    std::shared_ptr<ui::Theme> theme;
    if (themeName == "SolarizedDark")
        theme = std::make_shared<ui::ThemeSolarizedDark>();
    else if (themeName == "SolarizedLight")
        theme = std::make_shared<ui::ThemeSolarizedLight>();
    else if (themeName == "Molokai")
        theme = std::make_shared<ui::ThemeMolokai>();
    else if (themeName == "FrameworkDefault")
        theme = std::make_shared<ui::ThemeFrameworkDefault>();
    else {
        std::cerr << "Unknown theme, defaulting to FrameworkDefault" << std::endl;
        theme = std::make_shared<ui::ThemeFrameworkDefault>();
    }
    core->setTheme(theme);
}
