#include "UI.h"
#include "Theme/ThemeFrameworkDefault.h"
#include "Theme/ThemeSolarizedDark.h"
#include "Theme/ThemeSolarizedLight.h"
#include "Theme/ThemeMolokai.h"
#include "UIElements/ContextMenu.h"
#include "UIElements/ListView.h"
#include "UIElements/Modal.h"
#include "UIElements/Label.h"
#include "UIElements/Button.h"
#include "UIElements/TextBox.h"
#include "UIElements/CheckBox.h"
#include "UIElements/OptionSelect.h"
#include "UIElements/Canvas.h"
#include <cctype>
#include <iostream>

UI::UI(const char* title, int width, int height) {
    core = std::make_unique<ui::UICore>(title, width, height, std::make_shared<ui::ThemeFrameworkDefault>());
}

UI::~UI() {}

void UI::setFocus(ui::UIElement* element) {
    core->setFocus(element);
}

void UI::assignHotKey(ui::UIElement* element, const std::string &hotKeyStr) {
    if (hotKeyStr.empty())
        return;
    // Use the UICore static helper:
    SDL_Keycode key = ui::UICore::keycodeFromString(hotKeyStr);
    if (key != SDLK_UNKNOWN) {
         core->registerHotKey(key, [element]() {
             element->hasFocus = true;
             element->activate();
         });
    }
}

// overload with a custom callback:
void UI::assignHotKey(ui::UIElement* element, const std::string &hotKeyStr, std::function<void()> customCallback) {
    if (hotKeyStr.empty())
        return;
    SDL_Keycode key = ui::UICore::keycodeFromString(hotKeyStr);
    if (key != SDLK_UNKNOWN) {
         core->registerHotKey(key, customCallback);
    }
}


ui::Label* UI::label(const std::string &text, int x, int y) {
    auto lbl = std::make_shared<ui::Label>(x, y, text);
    core->addElement(lbl);
    return lbl.get();
}

ui::Button* UI::button(const std::string &text, int x, int y, std::function<void()> callback) {
    auto btn = std::make_shared<ui::Button>(x, y, 150, 40, text, callback);
    core->addElement(btn);
    return btn.get();
}

ui::TextBox* UI::textBox(const std::string &defaultText, int x, int y, bool autoHighlight) {
    auto tb = std::make_shared<ui::TextBox>(x, y, 250, 40, autoHighlight);
    tb->content = defaultText;
    core->addElement(tb);
    return tb.get();
}

ui::CheckBox* UI::checkBox(bool state, int x, int y, std::function<void(bool)> callback) {
    auto cb = std::make_shared<ui::CheckBox>(x, y, 14, state, callback);
    core->addElement(cb);
    return cb.get();
}

ui::OptionSelect* UI::optionSelect(int current, const std::vector<std::string> &options, int x, int y, std::function<void(int)> callback) {
    int collapsedHeight = 40;
    auto os = std::make_shared<ui::OptionSelect>(x, y, 150, collapsedHeight, options, current, current, callback);
    core->addElement(os);
    return os.get();
}

ui::Canvas* UI::canvas(int x, int y, int width, int height) {
    auto cnv = std::make_shared<ui::Canvas>(x, y, width, height);
    core->addElement(cnv);
    return cnv.get();
}

ui::ContextMenu* UI::contextMenu(const std::vector<ui::TopMenuItem>& menus) {
    std::vector<ui::MenuItem> items;
    for (const auto &top : menus) {
        ui::MenuItem m;
        m.label = top.label;
        for (const auto &sub : top.subItems) {
            m.subItemLabels.push_back(sub.label);
            m.subCallbacks.push_back(sub.callback);
        }
        items.push_back(m);
    }
    auto ctxMenu = std::make_shared<ui::ContextMenu>(0, 0, core->width, 30);
    ctxMenu->setItems(items);
    core->addElement(ctxMenu);
    return ctxMenu.get();
}

ui::ListView* UI::listView(const std::vector<std::string>& items, int x, int y, int w, int h, int itemHeight) {
    auto lv = std::make_shared<ui::ListView>(x, y, w, h, itemHeight);
    lv->items = items;
    core->addElement(lv);
    return lv.get();
}

// General modal with a single button.
ui::Modal* UI::modal(const std::string &message, const std::string &buttonText, bool hasCancel, std::function<void()> onCloseCallback) {
    // If a modal is already active, do not create another.
    if(core->modalActive)
         return nullptr;

    int winW = core->width, winH = core->height; // Use UICore dimensions
    int modalW = 400, modalH = 150;
    int modalX = (winW - modalW) / 2;
    int modalY = (winH - modalH) / 2;
    auto m = std::make_shared<ui::Modal>(modalX, modalY, modalW, modalH, message);

    // Clear focus from underlying elements.
    for (auto &el : core->elements)
         el->hasFocus = false;
    core->focusedIndex = -1;
    
    // Mark that a modal is active.
    core->modalActive = true;
    
    // Set up the modal's button(s).
    // For a general modal, we create one button with the provided buttonText.
    m->buttonLabels.clear();
    m->buttonCallbacks.clear();
    m->buttonLabels.push_back(buttonText);
    m->buttonCallbacks.push_back([m]() {
         m->dismissed = true;
    });
    
    // Set the onDismiss callback to reset the modalActive flag and call any onClose callback.
    m->onDismiss = [this, onCloseCallback]() {
         core->modalActive = false;
         if (onCloseCallback)
             onCloseCallback();
    };
    
    core->addElement(m);
    return m.get();
}

ui::Modal* UI::infoModal(const std::string &message, std::function<void()> onCloseCallback) {
    // For an info modal, we simply use our general modal with "OK" as the button text.
    return modal(message, "OK", false, onCloseCallback);
}

ui::Modal* UI::confirmModal(const std::string &message, std::function<void()> onConfirmCallback, std::function<void()> onCancelCallback) {
    if(core->modalActive)
         return nullptr;

    int winW = core->width, winH = core->height; // Use UICore dimensions
    int modalW = 400, modalH = 150;
    int modalX = (winW - modalW) / 2;
    int modalY = (winH - modalH) / 2;
    auto m = std::make_shared<ui::Modal>(modalX, modalY, modalW, modalH, message);
    
    // Clear underlying focus.
    for (auto &el : core->elements)
         el->hasFocus = false;
    core->focusedIndex = -1;
    
    core->modalActive = true;
    
    // Set up two buttons: "Confirm" and "Cancel".
    m->buttonLabels.clear();
    m->buttonCallbacks.clear();
    
    // "Confirm" button.
    m->buttonLabels.push_back("Confirm");
    m->buttonCallbacks.push_back([m, onConfirmCallback]() {
         if (onConfirmCallback)
             onConfirmCallback();
         m->dismissed = true;
    });
    
    // "Cancel" button.
    m->buttonLabels.push_back("Cancel");
    m->buttonCallbacks.push_back([m, onCancelCallback]() {
         if (onCancelCallback)
             onCancelCallback();
         m->dismissed = true;
    });
    
    // When dismissed, reset modalActive.
    m->onDismiss = [this]() {
         core->modalActive = false;
    };
    
    core->addElement(m);
    return m.get();
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

void UI::addElement(std::shared_ptr<ui::UIElement> element) {
    core->addElement(element);
}
