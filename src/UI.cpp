#include "UI.h"
#include "../lib/Theme/ThemeFrameworkDefault.h"
#include "../lib/Theme/ThemeSolarizedDark.h"
#include "../lib/Theme/ThemeSolarizedLight.h"
#include "../lib/Theme/ThemeMolokai.h"
#include <iostream>

// Constructor
UI::UI(const char* title, int width, int height) {
    try {
        auto defaultTheme = std::make_shared<ui::ThemeFrameworkDefault>();
        core = std::make_unique<ui::UICore>(title, width, height, defaultTheme);
    } catch (const ui::UIException& e) {
        std::cerr << "UI initialization failed: " << e.what() << std::endl;
        throw;
    }
}

// --- New API Methods (returning shared_ptr) ---

std::shared_ptr<ui::Label> UI::createLabel(const std::string &text, int x, int y) {
    auto label = std::make_shared<ui::Label>(x, y, 100, 30, text); // Default size
    if (core->getFont()) {
        label->autoSize(core->getFont());
    }
    return registerElement(label);
}

std::shared_ptr<ui::Button> UI::createButton(const std::string &text, int x, int y, std::function<void()> callback) {
    auto button = std::make_shared<ui::Button>(x, y, 100, 30, text, callback); // Default size
    return registerElement(button);
}

std::shared_ptr<ui::TextBox> UI::createTextBox(const std::string &defaultText, int x, int y, bool autoHighlight) {
    auto textBox = std::make_shared<ui::TextBox>(x, y, 200, 30, defaultText, autoHighlight); // Default size
    return registerElement(textBox);
}

std::shared_ptr<ui::CheckBox> UI::createCheckBox(bool state, int x, int y, std::function<void(bool)> callback) {
    auto checkBox = std::make_shared<ui::CheckBox>(x, y, 20, state, callback); // Default size of 20x20
    return registerElement(checkBox);
}

std::shared_ptr<ui::OptionSelect> UI::createOptionSelect(int current, const std::vector<std::string> &options, int x, int y, std::function<void(int)> callback) {
    auto optionSelect = std::make_shared<ui::OptionSelect>(x, y, 150, 30, current, options, callback); // Default size
    return registerElement(optionSelect);
}

std::shared_ptr<ui::Canvas> UI::createCanvas(int x, int y, int width, int height) {
    auto canvas = std::make_shared<ui::Canvas>(x, y, width, height);
    return registerElement(canvas);
}

std::shared_ptr<ui::ContextMenu> UI::createContextMenu(const std::vector<ui::TopMenuItem>& menus) {
    auto contextMenu = std::make_shared<ui::ContextMenu>(0, 0, 200, 30, menus); // Position will be set dynamically
    return registerElement(contextMenu);
}

std::shared_ptr<ui::ListView> UI::createListView(const std::vector<std::string>& items, int x, int y, int w, int h, int itemHeight) {
    auto listView = std::make_shared<ui::ListView>(x, y, w, h, items, itemHeight);
    return registerElement(listView);
}

std::shared_ptr<ui::Image> UI::createImage(const std::string &path, int x, int y, int w, int h, bool stretch) {
    auto image = std::make_shared<ui::Image>(x, y, w, h, path, stretch);
    return registerElement(image);
}

std::shared_ptr<ui::Image> UI::createImage(const unsigned char* data, size_t dataSize, int x, int y, int w, int h, bool stretch) {
    auto image = std::make_shared<ui::Image>(x, y, w, h, data, dataSize, stretch);
    return registerElement(image);
}

std::shared_ptr<ui::Sprite> UI::createSprite(const std::string &path, int x, int y, int w, int h, bool stretch) {
    auto sprite = std::make_shared<ui::Sprite>(x, y, w, h, path, stretch);
    return registerElement(sprite);
}

std::shared_ptr<ui::Sprite> UI::createSprite(const unsigned char* data, size_t dataSize, int x, int y, int w, int h, bool stretch) {
    auto sprite = std::make_shared<ui::Sprite>(x, y, w, h, data, dataSize, stretch);
    return registerElement(sprite);
}

std::shared_ptr<ui::AnimatedSprite> UI::createAnimatedSprite(const std::string &path, int x, int y, int w, int h, int frameCount, Uint32 frameDelay, bool stretch) {
    auto animatedSprite = std::make_shared<ui::AnimatedSprite>(x, y, w, h, path, frameCount, frameDelay, stretch);
    return registerElement(animatedSprite);
}

std::shared_ptr<ui::AnimatedSprite> UI::createAnimatedSprite(const unsigned char* data, size_t dataSize, int x, int y, int w, int h, int frameCount, Uint32 frameDelay, bool stretch) {
    auto animatedSprite = std::make_shared<ui::AnimatedSprite>(x, y, w, h, data, dataSize, frameCount, frameDelay, stretch);
    return registerElement(animatedSprite);
}

std::shared_ptr<ui::ProgressBar> UI::createProgressBar(int x, int y, int width, int height, float initValue, bool showText) {
    auto progressBar = std::make_shared<ui::ProgressBar>(x, y, width, height, initValue, showText);
    return registerElement(progressBar);
}

// --- Modal Methods ---

std::shared_ptr<ui::Modal> UI::createModal(const std::string &message, const std::string &buttonText, bool hasCancel, std::function<void()> onCloseCallback) {
    auto modal = std::make_shared<ui::Modal>(0, 0, 400, 200, message, buttonText, hasCancel, onCloseCallback);
    // Center the modal
    modal->setPosition((getWidth() - 400) / 2, (getHeight() - 200) / 2);
    return registerElement(modal);
}

std::shared_ptr<ui::Modal> UI::createConfirmModal(const std::string &message, std::function<void()> onConfirmCallback, std::function<void()> onCancelCallback) {
    auto modal = std::make_shared<ui::Modal>(0, 0, 400, 200, message, "Confirm", true, onConfirmCallback);
    // Set cancel callback if provided
    if (onCancelCallback) {
        // This would need to be implemented in Modal class
    }
    modal->setPosition((getWidth() - 400) / 2, (getHeight() - 200) / 2);
    return registerElement(modal);
}

std::shared_ptr<ui::Modal> UI::createInfoModal(const std::string &message, std::function<void()> onCloseCallback) {
    return createModal(message, "OK", false, onCloseCallback);
}

// --- Element Management ---

void UI::removeElement(const std::string& elementId) {
    core->removeElement(elementId);
}

std::shared_ptr<ui::UIElement> UI::getElement(const std::string& elementId) {
    return core->getElement(elementId);
}

// --- Hotkey Management ---

void UI::assignHotKey(const std::string& elementId, const std::string &hotKey) {
    auto element = core->getElement(elementId);
    if (element) {
        SDL_Keycode key = ui::UICore::keycodeFromString(hotKey);
        core->registerHotKey(key, [element]() {
            element->activate();
        });
    }
}

void UI::assignHotKey(const std::string& elementId, const std::string &hotKey, std::function<void()> customCallback) {
    SDL_Keycode key = ui::UICore::keycodeFromString(hotKey);
    core->registerHotKey(key, customCallback);
}

void UI::assignHotKey(std::shared_ptr<ui::UIElement> element, const std::string &hotKey) {
    if (element) {
        assignHotKey(element->getId(), hotKey);
    }
}

void UI::assignHotKey(std::shared_ptr<ui::UIElement> element, const std::string &hotKey, std::function<void()> customCallback) {
    if (element) {
        assignHotKey(element->getId(), hotKey, customCallback);
    }
}

// --- Focus Management ---

void UI::setFocus(const std::string& elementId) {
    core->setFocus(elementId);
}

void UI::setFocus(std::shared_ptr<ui::UIElement> element) {
    if (element) {
        core->setFocus(element->getId());
    }
}

std::string UI::getFocusedElementId() const {
    return core->getFocusedElementId();
}

// --- Core Functionality ---

void UI::run() {
    core->run();
}

void UI::setTheme(const std::string &themeName) {
    std::shared_ptr<ui::Theme> theme;
    
    if (themeName == "SolarizedDark") {
        theme = std::make_shared<ui::ThemeSolarizedDark>();
    } else if (themeName == "SolarizedLight") {
        theme = std::make_shared<ui::ThemeSolarizedLight>();
    } else if (themeName == "Molokai") {
        theme = std::make_shared<ui::ThemeMolokai>();
    } else {
        theme = std::make_shared<ui::ThemeFrameworkDefault>();
    }
    
    core->setTheme(theme);
}

void UI::addElement(std::shared_ptr<ui::UIElement> element) {
    if (element) {
        core->addElement(element);
    }
}

// Layout creation methods
std::shared_ptr<ui::LayoutContainer> UI::createVBoxLayout(int x, int y, int width, int height, int spacing) {
    auto layout = std::make_unique<ui::VBoxLayout>(spacing);
    auto container = std::make_shared<ui::LayoutContainer>(x, y, width, height, std::move(layout));
    return registerElement(container);
}

std::shared_ptr<ui::LayoutContainer> UI::createHBoxLayout(int x, int y, int width, int height, int spacing) {
    auto layout = std::make_unique<ui::HBoxLayout>(spacing);
    auto container = std::make_shared<ui::LayoutContainer>(x, y, width, height, std::move(layout));
    return registerElement(container);
}

std::shared_ptr<ui::LayoutContainer> UI::createGridLayout(int x, int y, int width, int height, int rows, int columns, int spacing) {
    auto layout = std::make_unique<ui::GridLayout>(rows, columns);
    layout->setSpacing(spacing);
    auto container = std::make_shared<ui::LayoutContainer>(x, y, width, height, std::move(layout));
    return registerElement(container);
}

int UI::getWidth() const {
    return core->getWidth();
}

int UI::getHeight() const {
    return core->getHeight();
}

// --- Deprecated Methods (for backward compatibility) ---

ui::Label* UI::label(const std::string &text, int x, int y) {
    auto labelPtr = createLabel(text, x, y);
    return labelPtr.get(); // Return raw pointer for backward compatibility
}

ui::Button* UI::button(const std::string &text, int x, int y, std::function<void()> callback) {
    auto buttonPtr = createButton(text, x, y, callback);
    return buttonPtr.get();
}

ui::TextBox* UI::textBox(const std::string &defaultText, int x, int y, bool autoHighlight) {
    auto textBoxPtr = createTextBox(defaultText, x, y, autoHighlight);
    return textBoxPtr.get();
}

ui::CheckBox* UI::checkBox(bool state, int x, int y, std::function<void(bool)> callback) {
    auto checkBoxPtr = createCheckBox(state, x, y, callback);
    return checkBoxPtr.get();
}

ui::OptionSelect* UI::optionSelect(int current, const std::vector<std::string> &options, int x, int y, std::function<void(int)> callback) {
    auto optionSelectPtr = createOptionSelect(current, options, x, y, callback);
    return optionSelectPtr.get();
}

ui::Canvas* UI::canvas(int x, int y, int width, int height) {
    auto canvasPtr = createCanvas(x, y, width, height);
    return canvasPtr.get();
}

ui::ContextMenu* UI::contextMenu(const std::vector<ui::TopMenuItem>& menus) {
    auto contextMenuPtr = createContextMenu(menus);
    return contextMenuPtr.get();
}

ui::ListView* UI::listView(const std::vector<std::string>& items, int x, int y, int w, int h, int itemHeight) {
    auto listViewPtr = createListView(items, x, y, w, h, itemHeight);
    return listViewPtr.get();
}

ui::Image* UI::image(const std::string &path, int x, int y, int w, int h, bool stretch) {
    auto imagePtr = createImage(path, x, y, w, h, stretch);
    return imagePtr.get();
}

ui::Image* UI::image(const unsigned char* data, size_t dataSize, int x, int y, int w, int h, bool stretch) {
    auto imagePtr = createImage(data, dataSize, x, y, w, h, stretch);
    return imagePtr.get();
}

ui::Sprite* UI::sprite(const std::string &path, int x, int y, int w, int h, bool stretch) {
    auto spritePtr = createSprite(path, x, y, w, h, stretch);
    return spritePtr.get();
}

ui::Sprite* UI::sprite(const unsigned char* data, size_t dataSize, int x, int y, int w, int h, bool stretch) {
    auto spritePtr = createSprite(data, dataSize, x, y, w, h, stretch);
    return spritePtr.get();
}

ui::AnimatedSprite* UI::animatedSprite(const std::string &path, int x, int y, int w, int h, int frameCount, Uint32 frameDelay, bool stretch) {
    auto animatedSpritePtr = createAnimatedSprite(path, x, y, w, h, frameCount, frameDelay, stretch);
    return animatedSpritePtr.get();
}

ui::AnimatedSprite* UI::animatedSprite(const unsigned char* data, size_t dataSize, int x, int y, int w, int h, int frameCount, Uint32 frameDelay, bool stretch) {
    auto animatedSpritePtr = createAnimatedSprite(data, dataSize, x, y, w, h, frameCount, frameDelay, stretch);
    return animatedSpritePtr.get();
}

ui::ProgressBar* UI::progressBar(int x, int y, int width, int height, float initValue, bool showText) {
    auto progressBarPtr = createProgressBar(x, y, width, height, initValue, showText);
    return progressBarPtr.get();
}

ui::Modal* UI::modal(const std::string &message, const std::string &buttonText, bool hasCancel, std::function<void()> onCloseCallback) {
    auto modalPtr = createModal(message, buttonText, hasCancel, onCloseCallback);
    return modalPtr.get();
}

ui::Modal* UI::confirmModal(const std::string &message, std::function<void()> onConfirmCallback, std::function<void()> onCancelCallback) {
    auto modalPtr = createConfirmModal(message, onConfirmCallback, onCancelCallback);
    return modalPtr.get();
}

ui::Modal* UI::infoModal(const std::string &message, std::function<void()> onCloseCallback) {
    auto modalPtr = createInfoModal(message, onCloseCallback);
    return modalPtr.get();
}
