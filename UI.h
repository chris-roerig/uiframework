#pragma once
#include "UICore.h"
#include "UIElements.h"
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

    // Existing methods.
    ui::Label* label(const std::string &text, int x, int y);
    ui::Button* button(const std::string &text, int x, int y, std::function<void()> callback);
    ui::TextBox* textBox(const std::string &defaultText, int x, int y, bool autoHighlight = true);
    ui::CheckBox* checkBox(bool state, int x, int y, std::function<void(bool)> callback);
    ui::OptionSelect* optionSelect(int current, const std::vector<std::string> &options, int x, int y, std::function<void(int)> callback);
    ui::Canvas* canvas(int x, int y, int width, int height);
    ui::ContextMenu* contextMenu(const std::vector<ui::TopMenuItem>& menus);
    ui::ListView* listView(const std::vector<std::string>& items, int x, int y, int w, int h, int itemHeight = 30);
    ui::Image* image(const std::string &path, int x, int y, int w, int h, bool stretch = false);
    ui::Image* image(const unsigned char* data, size_t dataSize, int x, int y, int w, int h, bool stretch = false);
    ui::Sprite* sprite(const std::string &path, int x, int y, int w, int h, bool stretch);
    ui::Sprite* sprite(const unsigned char* data, size_t dataSize, int x, int y, int w, int h, bool stretch);
    ui::AnimatedSprite* animatedSprite(const std::string &path, int x, int y, int w, int h,
                                         int frameCount, Uint32 frameDelay, bool stretch = false);
    ui::AnimatedSprite* animatedSprite(const unsigned char* data, size_t dataSize, int x, int y, int w, int h,
                                         int frameCount, Uint32 frameDelay, bool stretch = false);

    ui::ProgressBar* progressBar(int x, int y, int width, int height, float initValue = 0.0f, bool showText = true);

    void assignHotKey(ui::UIElement* element, const std::string &hotKey);
    void assignHotKey(ui::UIElement* element, const std::string &hotKey, std::function<void()> customCallback);
    void setFocus(ui::UIElement* element);

    // New Modal APIs:
    // General modal with a single button.
    ui::Modal* modal(const std::string &message, const std::string &buttonText, bool hasCancel, std::function<void()> onCloseCallback = nullptr);
    // Confirmation modal with two buttons ("Confirm" and "Cancel").
    ui::Modal* confirmModal(const std::string &message, std::function<void()> onConfirmCallback = nullptr, std::function<void()> onCancelCallback = nullptr);
    // Information modal with an "OK" button.
    ui::Modal* infoModal(const std::string &message, std::function<void()> onCloseCallback = nullptr);

    void run();
    void setTheme(const std::string &themeName);
    void addElement(std::shared_ptr<ui::UIElement> element);
};
