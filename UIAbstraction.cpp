#include "UIAbstraction.h"

UIAbstraction::UIAbstraction(const char* title, int width, int height) {
    // Create the underlying UIManager.
    manager = std::make_unique<ui::UIManager>(title, width, height);
}

UIAbstraction::~UIAbstraction() {
    // manager is automatically cleaned up.
}

void UIAbstraction::setBackgroundColor(const ui::Color &color) {
    manager->setBackgroundColor(color);
}

void UIAbstraction::setFontColor(const ui::Color &color) {
    manager->setFontColor(color);
}

void UIAbstraction::setHighlightColor(const ui::Color &color) {
    manager->setHighlightColor(color);
}

void UIAbstraction::label(const std::string &text, int x, int y) {
    auto lbl = std::make_shared<ui::Label>(x, y, text);
    manager->addElement(lbl);
}

void UIAbstraction::button(const std::string &text, int x, int y, std::function<void()> callback) {
    // Use default button size 200x50.
    auto btn = std::make_shared<ui::Button>(x, y, 200, 50, text, callback);
    manager->addElement(btn);
}

void UIAbstraction::textBox(const std::string &defaultText, int x, int y) {
    // Use default textBox size 200x50.
    auto tb = std::make_shared<ui::TextBox>(x, y, 200, 50);
    tb->content = defaultText;
    manager->addElement(tb);
}

void UIAbstraction::checkBox(bool state, int x, int y, std::function<void(bool)> callback) {
    // Use default checkbox size 30.
    auto cb = std::make_shared<ui::CheckBox>(x, y, 30, state, callback);
    manager->addElement(cb);
}

void UIAbstraction::optionSelect(int current, const std::vector<std::string> &options, int x, int y, std::function<void(int)> callback) {
    // Use default size: 200 width and height equal to (number of options * 50).
    int height = static_cast<int>(options.size()) * 50;
    // Both interactive and active index set to current.
    auto os = std::make_shared<ui::OptionSelect>(x, y, 200, height, options, current, current, callback);
    manager->addElement(os);
}

ui::Canvas* UIAbstraction::canvas(int x, int y, int width, int height) {
    auto cnv = std::make_shared<ui::Canvas>(x, y, width, height);
    manager->addElement(cnv);
    // Return a raw pointer for convenience.
    return cnv.get();
}

void UIAbstraction::run() {
    manager->run();
}

