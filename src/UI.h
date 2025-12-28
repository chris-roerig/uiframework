#pragma once
#include "UICore.h"
#include "UIElements.h"
#include "../lib/Layout/VBoxLayout.h"
#include "../lib/Layout/HBoxLayout.h"
#include "../lib/Layout/GridLayout.h"
#include <memory>
#include <functional>
#include <vector>
#include <string>

class UI {
private:
    std::unique_ptr<ui::UICore> core;
    
    // Helper method to register element and return shared_ptr
    template<typename T>
    std::shared_ptr<T> registerElement(std::shared_ptr<T> element) {
        if (!element) {
            throw ui::UIException("Cannot register null element");
        }
        core->addElement(element);
        return element;
    }
    
public:
    UI(const char* title, int width, int height);
    ~UI() = default;

    // Element creation methods - now return shared_ptr for proper memory management
    std::shared_ptr<ui::Label> createLabel(const std::string &text, int x, int y);
    std::shared_ptr<ui::Button> createButton(const std::string &text, int x, int y, std::function<void()> callback);
    std::shared_ptr<ui::TextBox> createTextBox(const std::string &defaultText, int x, int y, bool autoHighlight = true);
    std::shared_ptr<ui::CheckBox> createCheckBox(bool state, int x, int y, std::function<void(bool)> callback);
    std::shared_ptr<ui::OptionSelect> createOptionSelect(int current, const std::vector<std::string> &options, int x, int y, std::function<void(int)> callback);
    std::shared_ptr<ui::Canvas> createCanvas(int x, int y, int width, int height);
    std::shared_ptr<ui::ContextMenu> createContextMenu(const std::vector<ui::TopMenuItem>& menus);
    std::shared_ptr<ui::ListView> createListView(const std::vector<std::string>& items, int x, int y, int w, int h, int itemHeight = 30);
    std::shared_ptr<ui::Image> createImage(const std::string &path, int x, int y, int w, int h, bool stretch = false);
    std::shared_ptr<ui::Image> createImage(const unsigned char* data, size_t dataSize, int x, int y, int w, int h, bool stretch = false);
    std::shared_ptr<ui::Sprite> createSprite(const std::string &path, int x, int y, int w, int h, bool stretch);
    std::shared_ptr<ui::Sprite> createSprite(const unsigned char* data, size_t dataSize, int x, int y, int w, int h, bool stretch);
    std::shared_ptr<ui::AnimatedSprite> createAnimatedSprite(const std::string &path, int x, int y, int w, int h,
                                         int frameCount, Uint32 frameDelay, bool stretch = false);
    std::shared_ptr<ui::AnimatedSprite> createAnimatedSprite(const unsigned char* data, size_t dataSize, int x, int y, int w, int h,
                                         int frameCount, Uint32 frameDelay, bool stretch = false);
    std::shared_ptr<ui::ProgressBar> createProgressBar(int x, int y, int width, int height, float initValue = 0.0f, bool showText = true);
    std::shared_ptr<ui::HSlider> createHSlider(int x, int y, int width, int height, float min = 0.0f, float max = 100.0f, float initial = 0.0f);
    std::shared_ptr<ui::VSlider> createVSlider(int x, int y, int width, int height, float min = 0.0f, float max = 100.0f, float initial = 0.0f);

    // Backward compatibility methods (deprecated - will be removed in future versions)
    [[deprecated("Use createLabel instead")]]
    ui::Label* label(const std::string &text, int x, int y);
    [[deprecated("Use createButton instead")]]
    ui::Button* button(const std::string &text, int x, int y, std::function<void()> callback);
    [[deprecated("Use createTextBox instead")]]
    ui::TextBox* textBox(const std::string &defaultText, int x, int y, bool autoHighlight = true);
    [[deprecated("Use createCheckBox instead")]]
    ui::CheckBox* checkBox(bool state, int x, int y, std::function<void(bool)> callback);
    [[deprecated("Use createOptionSelect instead")]]
    ui::OptionSelect* optionSelect(int current, const std::vector<std::string> &options, int x, int y, std::function<void(int)> callback);
    [[deprecated("Use createCanvas instead")]]
    ui::Canvas* canvas(int x, int y, int width, int height);
    [[deprecated("Use createContextMenu instead")]]
    ui::ContextMenu* contextMenu(const std::vector<ui::TopMenuItem>& menus);
    [[deprecated("Use createListView instead")]]
    ui::ListView* listView(const std::vector<std::string>& items, int x, int y, int w, int h, int itemHeight = 30);
    [[deprecated("Use createImage instead")]]
    ui::Image* image(const std::string &path, int x, int y, int w, int h, bool stretch = false);
    [[deprecated("Use createImage instead")]]
    ui::Image* image(const unsigned char* data, size_t dataSize, int x, int y, int w, int h, bool stretch = false);
    [[deprecated("Use createSprite instead")]]
    ui::Sprite* sprite(const std::string &path, int x, int y, int w, int h, bool stretch);
    [[deprecated("Use createSprite instead")]]
    ui::Sprite* sprite(const unsigned char* data, size_t dataSize, int x, int y, int w, int h, bool stretch);
    [[deprecated("Use createAnimatedSprite instead")]]
    ui::AnimatedSprite* animatedSprite(const std::string &path, int x, int y, int w, int h,
                                         int frameCount, Uint32 frameDelay, bool stretch = false);
    [[deprecated("Use createAnimatedSprite instead")]]
    ui::AnimatedSprite* animatedSprite(const unsigned char* data, size_t dataSize, int x, int y, int w, int h,
                                         int frameCount, Uint32 frameDelay, bool stretch = false);
    [[deprecated("Use createProgressBar instead")]]
    ui::ProgressBar* progressBar(int x, int y, int width, int height, float initValue = 0.0f, bool showText = true);

    // Element management
    void removeElement(const std::string& elementId);
    std::shared_ptr<ui::UIElement> getElement(const std::string& elementId);
    
    // Hotkey management - now uses element IDs instead of raw pointers
    void assignHotKey(const std::string& elementId, const std::string &hotKey);
    void assignHotKey(const std::string& elementId, const std::string &hotKey, std::function<void()> customCallback);
    void assignHotKey(std::shared_ptr<ui::UIElement> element, const std::string &hotKey);
    void assignHotKey(std::shared_ptr<ui::UIElement> element, const std::string &hotKey, std::function<void()> customCallback);
    
    // Focus management - now uses element IDs
    void setFocus(const std::string& elementId);
    void setFocus(std::shared_ptr<ui::UIElement> element);
    std::string getFocusedElementId() const;

    // Modal APIs with better error handling
    std::shared_ptr<ui::Modal> createModal(const std::string &message, const std::string &buttonText, bool hasCancel, std::function<void()> onCloseCallback = nullptr);
    std::shared_ptr<ui::Modal> createConfirmModal(const std::string &message, std::function<void()> onConfirmCallback = nullptr, std::function<void()> onCancelCallback = nullptr);
    std::shared_ptr<ui::Modal> createInfoModal(const std::string &message, std::function<void()> onCloseCallback = nullptr);

    // Backward compatibility for modals (deprecated)
    [[deprecated("Use createModal instead")]]
    ui::Modal* modal(const std::string &message, const std::string &buttonText, bool hasCancel, std::function<void()> onCloseCallback = nullptr);
    [[deprecated("Use createConfirmModal instead")]]
    ui::Modal* confirmModal(const std::string &message, std::function<void()> onConfirmCallback = nullptr, std::function<void()> onCancelCallback = nullptr);
    [[deprecated("Use createInfoModal instead")]]
    ui::Modal* infoModal(const std::string &message, std::function<void()> onCloseCallback = nullptr);

    // Core functionality
    void run();
    void setTheme(const std::string &themeName);
    void addElement(std::shared_ptr<ui::UIElement> element);
    
    // Layout creation methods
    std::shared_ptr<ui::LayoutContainer> createVBoxLayout(int x, int y, int width, int height, int spacing = 5);
    std::shared_ptr<ui::LayoutContainer> createHBoxLayout(int x, int y, int width, int height, int spacing = 5);
    std::shared_ptr<ui::LayoutContainer> createGridLayout(int x, int y, int width, int height, int rows, int columns, int spacing = 5);
    
    // Utility methods
    int getWidth() const;
    int getHeight() const;
};
