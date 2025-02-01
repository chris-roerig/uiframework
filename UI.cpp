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

// Updated to fixed size: 150x40
void UI::button(const std::string &text, int x, int y, std::function<void()> callback) {
    auto btn = std::make_shared<ui::Button>(x, y, 150, 40, text, callback);
    core->addElement(btn);
}

// Updated to fixed size: 150x40
ui::TextBox* UI::textBox(const std::string &defaultText, int x, int y, bool autoHighlight) {
    auto tb = std::make_shared<ui::TextBox>(x, y, 150, 40, autoHighlight);
    tb->content = defaultText;
    core->addElement(tb);
    return tb.get();
}

// Updated checkbox size: 20x20
void UI::checkBox(bool state, int x, int y, std::function<void(bool)> callback) {
    auto cb = std::make_shared<ui::CheckBox>(x, y, 20, state, callback);
    core->addElement(cb);
}

// Updated optionSelect: fixed width of 150; collapsed height computed by TextBox logic (or you can force a fixed value),
// and expanded list cells fixed to 30 px tall.
void UI::optionSelect(int current, const std::vector<std::string> &options, int x, int y, std::function<void(int)> callback) {
    // For collapsed state, we want the height similar to a TextBox (assume 40 px here).
    int collapsedHeight = 40;
    auto os = std::make_shared<ui::OptionSelect>(x, y, 150, collapsedHeight, options, current, current, callback);
    core->addElement(os);
}

ui::Canvas* UI::canvas(int x, int y, int width, int height) {
    auto cnv = std::make_shared<ui::Canvas>(x, y, width, height);
    core->addElement(cnv);
    return cnv.get();
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
