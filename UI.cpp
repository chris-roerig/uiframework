#include "UI.h"
#include "Themes.h"
#include "UIElements.h"
#include "UIContainers.h"
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

ui::Image* UI::image(const std::string &path, int x, int y, int w, int h, bool stretch) {
    // Assuming UICore exposes a method getRenderer() that returns SDL_Renderer*
    auto img = std::make_shared<ui::Image>(x, y, w, h, path, core->renderer, stretch);
    core->addElement(img);
    return img.get();
}

// for loading binary header images (#include "my_image.h")
ui::Image* UI::image(const unsigned char* data, size_t dataSize, int x, int y, int w, int h, bool stretch) {
    auto img = std::make_shared<ui::Image>(x, y, w, h, data, dataSize, core->renderer, stretch);
    core->addElement(img);
    return img.get();
}

ui::Sprite* UI::sprite(const std::string &path, int x, int y, int w, int h, bool stretch) {
    auto spr = std::make_shared<ui::Sprite>(x, y, w, h, path, core->renderer, stretch);
    core->addElement(spr);
    return spr.get();
}

ui::Sprite* UI::sprite(const unsigned char* data, size_t dataSize, int x, int y, int w, int h, bool stretch) {
    auto spr = std::make_shared<ui::Sprite>(x, y, w, h, data, dataSize, core->renderer, stretch);
    core->addElement(spr);
    return spr.get();
}

ui::AnimatedSprite* UI::animatedSprite(const std::string &path, int x, int y, int w, int h,
                                       int frameCount, Uint32 frameDelay, bool stretch) {
    auto animSpr = std::make_shared<ui::AnimatedSprite>(x, y, w, h, path, core->renderer, frameCount, frameDelay, stretch);
    core->addElement(animSpr);
    return animSpr.get();
}

ui::AnimatedSprite* UI::animatedSprite(const unsigned char* data, size_t dataSize, int x, int y, int w, int h,
                                       int frameCount, Uint32 frameDelay, bool stretch) {
    auto animSpr = std::make_shared<ui::AnimatedSprite>(x, y, w, h, data, dataSize, core->renderer, frameCount, frameDelay, stretch);
    core->addElement(animSpr);
    return animSpr.get();
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

//* CONTAINERS */

// Example usage for FlowContainer:
//   ui::FlowContainer* flow = ui->flowContainer(50, 50, 400, 300);
//   flow->addChild(std::make_shared<ui::Label>(10, 10, "Flow Item 1"));
//   flow->addChild(std::make_shared<ui::Button>(...));
ui::FlowContainer* UI::flowContainer(int x, int y, int w, int h) {
    auto container = std::make_shared<ui::FlowContainer>(x, y, w, h);
    core->addElement(container);
    return container.get();
}

// Example usage for StackContainer:
//   ui::StackContainer* stack = ui->stackContainer(50, 400, 300, 200, ui::StackOrientation::Vertical);
//   stack->addChild(std::make_shared<ui::Label>(0, 0, "Stack Item 1"));
//   stack->addChild(std::make_shared<ui::Button>(...));
ui::StackContainer* UI::stackContainer(int x, int y, int w, int h, ui::StackOrientation orientation) {
    auto container = std::make_shared<ui::StackContainer>(x, y, w, h, orientation);
    core->addElement(container);
    return container.get();
}

// Example usage for GridContainer:
//   ui::GridContainer* grid = ui->gridContainer(400, 50, 400, 300, 3);
//   grid->addChild(std::make_shared<ui::Label>(0, 0, "Grid Item 1"));
//   grid->addChild(std::make_shared<ui::Button>(...));
ui::GridContainer* UI::gridContainer(int x, int y, int w, int h, int columns) {
    auto container = std::make_shared<ui::GridContainer>(x, y, w, h, columns);
    core->addElement(container);
    return container.get();
}

// Example usage for ScrollViewContainer:
//   ui::ScrollViewContainer* scrollView = ui->scrollViewContainer(50, 100, 400, 300, 200, 150);
//   scrollView->addChild(std::make_shared<ui::Label>(10, 10, "Scrollable Item 1"));
ui::ScrollViewContainer* UI::scrollViewContainer(int x, int y, int w, int h, int minWidth, int minHeight) {
    auto container = std::make_shared<ui::ScrollViewContainer>(x, y, w, h, minWidth, minHeight);
    core->addElement(container);
    return container.get();
}

// Example usage for TabContainer:
//   ui::TabContainer* tabs = ui->tabContainer(50, 50, 400, 300);
//   tabs->addTab("Tab 1", std::make_shared<ui::Label>(0, 0, "Content for Tab 1"));
//   tabs->addTab("Tab 2", std::make_shared<ui::Button>(0, 0, 150, 40, "Click Me", [](){ /* callback */ }));
ui::TabContainer* UI::tabContainer(int x, int y, int w, int h) {
    auto container = std::make_shared<ui::TabContainer>(x, y, w, h);
    core->addElement(container);
    return container.get();
}

// Example usage for SplitContainer:
//   ui::SplitContainer* split = ui->splitContainer(cols[0], 80, 600, 400, 300);
//   split->setLeftPanel(ui->label("Left Panel", 0, 0));
//   split->setRightPanel(ui->label("Right Panel", 0, 0));
ui::SplitContainer* UI::splitContainer(int x, int y, int w, int h, int dividerPos) {
    auto container = std::make_shared<ui::SplitContainer>(x, y, w, h, dividerPos);
    core->addElement(container);
    return container.get();
}

// Example usage for OverlayContainer:
//   ui::OverlayContainer* overlay = ui->overlayContainer(0, 0, core->width, core->height);
//   overlay->addChild(std::make_shared<ui::Label>(100, 100, "This is an overlay"));
ui::OverlayContainer* UI::overlayContainer(int x, int y, int w, int h) {
    auto container = std::make_shared<ui::OverlayContainer>(x, y, w, h);
    core->addElement(container);
    return container.get();
}

// Example usage for LayeredContainer:
//   ui::LayeredContainer* layered = ui->layeredContainer(0, 0, core->width, core->height);
//   layered->addChild(std::make_shared<ui::Label>(50, 50, "Bottom Layer"), 0);
//   layered->addChild(std::make_shared<ui::Label>(60, 60, "Top Layer"), 10);
ui::LayeredContainer* UI::layeredContainer(int x, int y, int w, int h) {
    auto container = std::make_shared<ui::LayeredContainer>(x, y, w, h);
    core->addElement(container);
    return container.get();
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



