#include "uiframework/UI_new.h"
#include "uiframework/Theme/ThemeFrameworkDefault.h"

// Constructor
UI::UI(const char* title, int width, int height) {
    try {
        auto defaultTheme = std::make_shared<ui::ThemeFrameworkDefault>();
        core = std::make_unique<ui::UICore>(title, width, height, defaultTheme);
    } catch (const ui::UIException& e) {
        throw;
    }
}

// Core framework methods
void UI::run() {
    core->run();
}

void UI::setTheme(std::shared_ptr<ui::Theme> theme) {
    core->setTheme(theme);
}

std::shared_ptr<ui::Theme> UI::getTheme() const {
    return core->getTheme();
}

bool UI::registerHotKey(SDL_Keycode key, std::function<void()> callback) {
    return core->registerHotKey(key, callback);
}

void UI::unregisterHotKey(SDL_Keycode key) {
    core->unregisterHotKey(key);
}

// Utility methods
SDL_Renderer* UI::getRenderer() const {
    return core->getRenderer();
}

TTF_Font* UI::getFont() const {
    return core->getFont();
}

int UI::getWidth() const {
    return core->getWidth();
}

int UI::getHeight() const {
    return core->getHeight();
}
