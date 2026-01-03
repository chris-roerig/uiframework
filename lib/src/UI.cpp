#include "uiframework/UI.h"
#include <iostream>
#include <algorithm>
#include "uiframework/ErrorHandling.h"
#include "uiframework/Theme/ThemeFrameworkDefault.h"
#include "uiframework/Theme/ThemeMolokai.h"
#include "uiframework/Theme/ThemeSolarizedDark.h"
#include "uiframework/Theme/ThemeSolarizedLight.h"
#include "uiframework/Resources/EmbeddedFontRegistry.h"

// Constructor
UI::UI(const char* title, int width, int height) {
    try {
        auto defaultTheme = std::make_shared<ui::ThemeFrameworkDefault>();
        core = std::make_unique<ui::UICore>(title, width, height, defaultTheme);
        constraintManager = std::make_unique<ui::ConstraintManager>();
        updateQueue = std::make_unique<ui::UIUpdateQueue>();
        
        // Set up frame callback for realtime update processing
        core->setFrameCallback([this]() {
            processRealtimeUpdates();
        });
    } catch (const ui::UIException& e) {
        std::cerr << "UI initialization failed: " << e.what() << std::endl;
        throw;
    }
}

// --- New API Methods (returning shared_ptr) ---

std::shared_ptr<ui::Label> UI::createLabel(const std::string& text, int x, int y) {
    auto label = std::make_shared<ui::Label>(x, y, ui::Constants::DEFAULT_LABEL_WIDTH,
                                             ui::Constants::DEFAULT_LABEL_HEIGHT, text);
    if (core->getFont()) {
        label->autoSize(core->getFont());
    }
    return registerElement(label);
}

std::shared_ptr<ui::Button> UI::createButton(const std::string& text, int x, int y,
                                             std::function<void()> callback) {
    auto button =
        std::make_shared<ui::Button>(x, y, ui::Constants::DEFAULT_BUTTON_WIDTH,
                                     ui::Constants::DEFAULT_BUTTON_HEIGHT, text, callback);
    return registerElement(button);
}

std::shared_ptr<ui::Button> UI::createButton(const std::string& text, int x, int y,
                                             std::function<void()> callback, int focusOrder) {
    auto button = createButton(text, x, y, callback);
    // Store focus order hint for later use in setFocusOrder
    focusOrderHints[button->getId()] = focusOrder;
    return button;
}

std::shared_ptr<ui::TextBox> UI::createTextBox(const std::string& defaultText, int x, int y,
                                               bool autoHighlight) {
    auto textBox = std::make_shared<ui::TextBox>(x, y, ui::Constants::DEFAULT_TEXTBOX_WIDTH,
                                                 ui::Constants::DEFAULT_TEXTBOX_HEIGHT, defaultText,
                                                 autoHighlight);
    return registerElement(textBox);
}

std::shared_ptr<ui::TextBox> UI::createTextBox(const std::string& defaultText, int x, int y,
                                               bool autoHighlight, int focusOrder) {
    auto textBox = createTextBox(defaultText, x, y, autoHighlight);
    // Store focus order hint for later use in setFocusOrder
    focusOrderHints[textBox->getId()] = focusOrder;
    return textBox;
}

std::shared_ptr<ui::CheckBox> UI::createCheckBox(bool state, int x, int y,
                                                 std::function<void(bool)> callback) {
    auto checkBox =
        std::make_shared<ui::CheckBox>(x, y, ui::Constants::DEFAULT_CHECKBOX_SIZE, state, callback);
    return registerElement(checkBox);
}

std::shared_ptr<ui::OptionSelect> UI::createOptionSelect(const std::vector<std::string>& options,
                                                         int current, int x, int y,
                                                         std::function<void(int)> callback) {
    auto optionSelect = std::make_shared<ui::OptionSelect>(
        x, y, ui::Constants::DEFAULT_OPTIONSELECT_WIDTH, ui::Constants::DEFAULT_OPTIONSELECT_HEIGHT,
        current, options, callback);
    return registerElement(optionSelect);
}

std::shared_ptr<ui::Canvas> UI::createCanvas(int x, int y, int width, int height) {
    auto canvas = std::make_shared<ui::Canvas>(x, y, width, height);
    return registerElement(canvas);
}

std::shared_ptr<ui::ContextMenu> UI::createContextMenu(const std::vector<ui::TopMenuItem>& menus) {
    auto contextMenu = std::make_shared<ui::ContextMenu>(
        0, 0, ui::Constants::SUBMENU_DEFAULT_WIDTH, ui::Constants::DEFAULT_BUTTON_HEIGHT, menus);
    return registerElement(contextMenu);
}

std::shared_ptr<ui::ListView> UI::createListView(const std::vector<std::string>& items, int x,
                                                 int y, int width, int height, int itemHeight) {
    auto listView = std::make_shared<ui::ListView>(x, y, width, height, items, itemHeight);
    return registerElement(listView);
}

std::shared_ptr<ui::Image> UI::createImage(const std::string& path, int x, int y, int width, int height,
                                           bool stretch) {
    auto image = std::make_shared<ui::Image>(x, y, width, height, path, stretch);
    return registerElement(image);
}

std::shared_ptr<ui::Image> UI::createImage(const unsigned char* data, size_t dataSize, int x, int y,
                                           int width, int height, bool stretch) {
    auto image = std::make_shared<ui::Image>(x, y, width, height, data, dataSize, stretch);
    return registerElement(image);
}

std::shared_ptr<ui::Sprite> UI::createSprite(const std::string& path, int x, int y, int width, int height,
                                             bool stretch) {
    auto sprite = std::make_shared<ui::Sprite>(x, y, width, height, path, stretch);
    return registerElement(sprite);
}

std::shared_ptr<ui::Sprite> UI::createSprite(const unsigned char* data, size_t dataSize, int x,
                                             int y, int width, int height, bool stretch) {
    auto sprite = std::make_shared<ui::Sprite>(x, y, width, height, data, dataSize, stretch);
    return registerElement(sprite);
}

std::shared_ptr<ui::AnimatedSprite> UI::createAnimatedSprite(const std::string& path, int x, int y,
                                                             int width, int height, int frameCount,
                                                             Uint32 frameDelay, bool stretch) {
    auto animatedSprite =
        std::make_shared<ui::AnimatedSprite>(x, y, width, height, path, frameCount, frameDelay, stretch);
    return registerElement(animatedSprite);
}

std::shared_ptr<ui::AnimatedSprite> UI::createAnimatedSprite(const unsigned char* data,
                                                             size_t dataSize, int x, int y, int width,
                                                             int height, int frameCount,
                                                             Uint32 frameDelay, bool stretch) {
    auto animatedSprite = std::make_shared<ui::AnimatedSprite>(x, y, width, height, data, dataSize,
                                                               frameCount, frameDelay, stretch);
    return registerElement(animatedSprite);
}

std::shared_ptr<ui::ProgressBar> UI::createProgressBar(int x, int y, int width, int height,
                                                       float initValue, bool showText) {
    auto progressBar = std::make_shared<ui::ProgressBar>(x, y, width, height, initValue, showText);
    return registerElement(progressBar);
}

std::shared_ptr<ui::HSlider> UI::createHSlider(int x, int y, int width, int height, float min,
                                               float max, float initial) {
    auto hslider = std::make_shared<ui::HSlider>(x, y, width, height, min, max, initial);
    return registerElement(hslider);
}

std::shared_ptr<ui::VSlider> UI::createVSlider(int x, int y, int width, int height, float min,
                                               float max, float initial) {
    auto vslider = std::make_shared<ui::VSlider>(x, y, width, height, min, max, initial);
    return registerElement(vslider);
}

std::shared_ptr<ui::KnobSlider> UI::createKnobSlider(int x, int y, int size, float min, float max,
                                                     float initial) {
    auto knobslider = std::make_shared<ui::KnobSlider>(x, y, size, size, min, max, initial);
    return registerElement(knobslider);
}

std::shared_ptr<ui::VirtualKeyboard> UI::createVirtualKeyboard(int x, int y, int width, int height,
                                                               std::function<void(char)> callback) {
    auto keyboard = std::make_shared<ui::VirtualKeyboard>(x, y, width, height, callback);
    return registerElement(keyboard);
}

std::shared_ptr<ui::CycleList> UI::createCycleList(int x, int y, int width, int height,
                                                   const std::vector<std::string>& items) {
    auto cycleList = std::make_shared<ui::CycleList>(x, y, width, height, items);
    return registerElement(cycleList);
}

std::shared_ptr<ui::TabbedPanel> UI::createTabbedPanel(int x, int y, int width, int height) {
    auto tabbedPanel = std::make_shared<ui::TabbedPanel>(x, y, width, height);
    return registerElement(tabbedPanel);
}

// --- Modal Methods ---

std::shared_ptr<ui::Modal> UI::createModal(const std::string& message,
                                           const std::string& buttonText, bool hasCancel,
                                           std::function<void()> onCloseCallback) {
    auto modal = std::make_shared<ui::Modal>(0, 0, ui::Constants::DEFAULT_MODAL_WIDTH,
                                             ui::Constants::DEFAULT_MODAL_HEIGHT, message,
                                             buttonText, hasCancel, onCloseCallback);
    // Center the modal
    modal->setPosition((getWidth() - ui::Constants::DEFAULT_MODAL_WIDTH) / 2,
                       (getHeight() - ui::Constants::DEFAULT_MODAL_HEIGHT) / 2);
    return registerElement(modal);
}

std::shared_ptr<ui::Modal> UI::createConfirmModal(const std::string& message,
                                                  std::function<void()> onConfirmCallback,
                                                  std::function<void()> onCancelCallback) {
    auto modal =
        std::make_shared<ui::Modal>(0, 0, 400, 200, message, "Confirm", true, onConfirmCallback);
    // Set cancel callback if provided
    if (onCancelCallback) {
        // This would need to be implemented in Modal class
    }
    modal->setPosition((getWidth() - 400) / 2, (getHeight() - 200) / 2);
    return registerElement(modal);
}

std::shared_ptr<ui::Modal> UI::createInfoModal(const std::string& message,
                                               std::function<void()> onCloseCallback) {
    return createModal(message, "OK", false, onCloseCallback);
}

// --- Element Management ---

void UI::removeElement(const std::string& elementId) {
    core->removeElement(elementId);
    focusOrderHints.erase(elementId);
}

void UI::removeElement(uint64_t numericId) {
    // Get string ID before removal for cleanup
    auto element = core->getElement(numericId);
    if (element) {
        focusOrderHints.erase(element->getId());
    }
    core->removeElement(numericId);
}

std::shared_ptr<ui::UIElement> UI::getElement(const std::string& elementId) {
    return core->getElement(elementId);
}

std::shared_ptr<ui::UIElement> UI::getElement(uint64_t numericId) {
    return core->getElement(numericId);
}

// --- Hotkey Management ---

void UI::assignHotKey(const std::string& elementId, const std::string& hotKey) {
    auto element = core->getElement(elementId);
    if (element) {
        SDL_Keycode key = ui::UICore::keycodeFromString(hotKey);
        core->registerHotKey(key, [element]() { element->activate(); });
    }
}

void UI::assignHotKey(const std::string& elementId, const std::string& hotKey,
                      std::function<void()> customCallback) {
    SDL_Keycode key = ui::UICore::keycodeFromString(hotKey);
    core->registerHotKey(key, customCallback);
}

void UI::assignHotKey(std::shared_ptr<ui::UIElement> element, const std::string& hotKey) {
    if (element) {
        assignHotKey(element->getId(), hotKey);
    }
}

void UI::assignHotKey(std::shared_ptr<ui::UIElement> element, const std::string& hotKey,
                      std::function<void()> customCallback) {
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

void UI::focusNext() {
    core->focusNext();
}

void UI::focusPrevious() {
    core->focusPrevious();
}

void UI::setFocusOrder(const std::vector<std::string>& elementIds) {
    core->setFocusOrder(elementIds);
}

// Focus groups and trapping (Phase 4)
void UI::createFocusGroup(const std::string& groupName, const std::vector<std::string>& elementIds) {
    core->createFocusGroup(groupName, elementIds);
}

void UI::setActiveFocusGroup(const std::string& groupName) {
    core->setActiveFocusGroup(groupName);
}

void UI::clearActiveFocusGroup() {
    core->clearActiveFocusGroup();
}

void UI::trapFocus(const std::string& groupName) {
    core->trapFocus(groupName);
}

void UI::releaseFocusTrap() {
    core->releaseFocusTrap();
}

void UI::applyFocusOrderHints() {
    if (focusOrderHints.empty()) return;
    
    // Create vector of pairs (elementId, focusOrder) and sort by focusOrder
    std::vector<std::pair<std::string, int>> sortedHints;
    for (const auto& hint : focusOrderHints) {
        sortedHints.emplace_back(hint.first, hint.second);
    }
    
    std::sort(sortedHints.begin(), sortedHints.end(), 
              [](const auto& a, const auto& b) { return a.second < b.second; });
    
    // Extract sorted element IDs
    std::vector<std::string> sortedIds;
    for (const auto& hint : sortedHints) {
        sortedIds.push_back(hint.first);
    }
    
    setFocusOrder(sortedIds);
}

// --- Core Functionality ---

void UI::run() {
    core->run();
}

void UI::setTheme(const std::string& themeName) {
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

int UI::getWidth() const {
    return core->getWidth();
}

int UI::getHeight() const {
    return core->getHeight();
}

ui::ConstraintManager* UI::getConstraintManager() const {
    return constraintManager.get();
}

void UI::setGridSize(int gridSize) {
    this->gridSize = gridSize;
    if (constraintManager) {
        constraintManager->setGridSize(gridSize);
    }
}

int UI::getGridSize() const {
    return gridSize;
}

void UI::bulkSetAnchors(const std::vector<std::string>& elementIds, 
                       const std::vector<std::string>& targetIds,
                       const std::vector<ui::AnchorType>& anchorTypes,
                       const std::vector<int>& offsets) {
    if (elementIds.size() != targetIds.size() || 
        elementIds.size() != anchorTypes.size() || 
        elementIds.size() != offsets.size()) {
        return; // Mismatched vector sizes
    }
    
    for (size_t i = 0; i < elementIds.size(); ++i) {
        auto element = getElement(elementIds[i]);
        auto target = getElement(targetIds[i]);
        if (element && target) {
            element->setAnchor(target, anchorTypes[i], offsets[i]);
        }
    }
}

void UI::bulkClearConstraints(const std::vector<std::string>& elementIds) {
    for (const auto& id : elementIds) {
        auto element = getElement(id);
        if (element) {
            element->clearConstraints();
        }
    }
}

// Element pooling for high-frequency scenarios
void UI::enableElementPooling(size_t labelCount, size_t buttonCount) {
    if (!elementPool) {
        elementPool = std::make_unique<ui::ElementPool>();
    }
    elementPool->preAllocate(labelCount, buttonCount);
}

std::shared_ptr<ui::Label> UI::createLabelPooled(const std::string& text, int x, int y) {
    if (!elementPool) {
        elementPool = std::make_unique<ui::ElementPool>();
    }
    
    auto label = elementPool->acquireLabel();
    label->setText(text);
    label->setPosition(x, y);
    
    // Register with core but don't use normal creation path
    core->addElement(label);
    return label;
}

std::shared_ptr<ui::Button> UI::createButtonPooled(const std::string& text, int x, int y, 
                                                   std::function<void()> callback) {
    if (!elementPool) {
        elementPool = std::make_unique<ui::ElementPool>();
    }
    
    auto button = elementPool->acquireButton();
    button->setText(text);
    button->setPosition(x, y);
    button->setCallback(callback);
    
    // Register with core but don't use normal creation path
    core->addElement(button);
    return button;
}

void UI::releasePooledElement(const std::string& elementId) {
    if (elementPool) {
        elementPool->releaseElement(elementId);
    }
    // Also remove from core
    core->removeElement(elementId);
}

UI::PoolStats UI::getPoolStats() const {
    if (!elementPool) {
        return {0, 0, 0, 0};
    }
    
    auto stats = elementPool->getStats();
    return {stats.labelsAvailable, stats.buttonsAvailable, stats.labelsInUse, stats.buttonsInUse};
}

// --- Real-Time Safe Update Methods ---

bool UI::realtimeSetText(const std::string& elementId, const std::string& text) {
    ui::UIUpdate update(ui::UIUpdate::SET_TEXT, elementId);
    update.textValue = text;
    return updateQueue->tryEnqueue(update);
}

bool UI::realtimeSetPosition(const std::string& elementId, int x, int y) {
    ui::UIUpdate update(ui::UIUpdate::SET_POSITION, elementId);
    update.data.position = {x, y};
    return updateQueue->tryEnqueue(update);
}

bool UI::realtimeSetSize(const std::string& elementId, int width, int height) {
    ui::UIUpdate update(ui::UIUpdate::SET_SIZE, elementId);
    update.data.size = {width, height};
    return updateQueue->tryEnqueue(update);
}

bool UI::realtimeSetValue(const std::string& elementId, float value) {
    ui::UIUpdate update(ui::UIUpdate::SET_VALUE, elementId);
    update.data.floatValue = {value};
    return updateQueue->tryEnqueue(update);
}

bool UI::realtimeSetVisibility(const std::string& elementId, bool visible) {
    ui::UIUpdate update(ui::UIUpdate::SET_VISIBILITY, elementId);
    update.data.visibility = {visible};
    return updateQueue->tryEnqueue(update);
}

bool UI::realtimeCallback(std::function<void()> callback) {
    ui::UIUpdate update(ui::UIUpdate::CUSTOM_CALLBACK, "");
    update.callback = callback;
    return updateQueue->tryEnqueue(update);
}

// High-resolution timing methods
bool UI::realtimeSetTextAtTime(const std::string& elementId, const std::string& text, 
                              std::chrono::high_resolution_clock::time_point when) {
    ui::UIUpdate update(ui::UIUpdate::SET_TEXT, elementId, when);
    update.textValue = text;
    return updateQueue->tryScheduleUpdate(update, when);
}

bool UI::realtimeSetPositionAtTime(const std::string& elementId, int x, int y,
                                  std::chrono::high_resolution_clock::time_point when) {
    ui::UIUpdate update(ui::UIUpdate::SET_POSITION, elementId, when);
    update.data.position.x = x;
    update.data.position.y = y;
    return updateQueue->tryScheduleUpdate(update, when);
}

bool UI::realtimeSetValueAtTime(const std::string& elementId, float value,
                               std::chrono::high_resolution_clock::time_point when) {
    ui::UIUpdate update(ui::UIUpdate::SET_VALUE, elementId, when);
    update.data.floatValue.value = value;
    return updateQueue->tryScheduleUpdate(update, when);
}

bool UI::realtimeSetVisibilityAtTime(const std::string& elementId, bool visible,
                                    std::chrono::high_resolution_clock::time_point when) {
    ui::UIUpdate update(ui::UIUpdate::SET_VISIBILITY, elementId, when);
    update.data.visibility.visible = visible;
    return updateQueue->tryScheduleUpdate(update, when);
}

void UI::processRealtimeUpdates() {
    // Process scheduled updates first
    updateQueue->processScheduledUpdates();
    
    // Use memory-predictable processing for deterministic performance
    size_t processedCount = updateQueue->processPredictable(elementCache);
    
    if (processedCount == 0) {
        return; // No updates to process
    }
    
    // Populate element cache with actual elements (single mutex lock)
    for (auto& cache : elementCache) {
        cache.element = getElement(cache.elementId);
        cache.valid = (cache.element != nullptr);
    }
    
    // Apply predictable batches using cached elements (no additional locks)
    const auto& batches = updateQueue->getPredictableBatches();
    size_t activeBatches = updateQueue->getActiveBatches();
    
    for (size_t i = 0; i < activeBatches && i < elementCache.size(); ++i) {
        const auto& cache = elementCache[i];
        if (!cache.valid || !cache.element) {
            continue; // Element not found
        }
        
        const auto& batch = batches[i];
        auto element = cache.element;
        
        // Apply all pending updates for this element
        if (batch.hasText && !batch.textValue.empty()) {
            std::string textStr(batch.textValue);
            if (auto label = std::dynamic_pointer_cast<ui::Label>(element)) {
                label->setText(textStr);
            } else if (auto button = std::dynamic_pointer_cast<ui::Button>(element)) {
                button->setText(textStr);
            }
        }
        
        if (batch.hasPosition) {
            element->setPosition(batch.position.x, batch.position.y);
        }
        
        if (batch.hasSize) {
            element->setSize(batch.size.width, batch.size.height);
        }
        
        if (batch.hasValue) {
            if (auto slider = std::dynamic_pointer_cast<ui::HSlider>(element)) {
                slider->setValue(batch.value);
            } else if (auto vslider = std::dynamic_pointer_cast<ui::VSlider>(element)) {
                vslider->setValue(batch.value);
            } else if (auto progress = std::dynamic_pointer_cast<ui::ProgressBar>(element)) {
                progress->setProgress(batch.value);
            }
        }
        
        if (batch.hasVisibility) {
            element->setVisible(batch.visible);
        }
    }
}

// Bulk update methods implementation
size_t UI::realtimeBulkSetText(const std::vector<std::string>& elementIds, 
                              const std::vector<std::string>& textValues) {
    if (!updateQueue) {
        return 0;
    }
    return updateQueue->tryBulkSetText(elementIds, textValues);
}

size_t UI::realtimeBulkSetPosition(const std::vector<std::string>& elementIds,
                                  const std::vector<std::pair<int, int>>& positions) {
    if (!updateQueue) {
        return 0;
    }
    return updateQueue->tryBulkSetPosition(elementIds, positions);
}

size_t UI::realtimeBulkSetValue(const std::vector<std::string>& elementIds,
                               const std::vector<float>& values) {
    if (!updateQueue) {
        return 0;
    }
    return updateQueue->tryBulkSetValue(elementIds, values);
}

size_t UI::realtimeBulkSetVisibility(const std::vector<std::string>& elementIds,
                                    const std::vector<bool>& visibility) {
    if (!updateQueue) {
        return 0;
    }
    return updateQueue->tryBulkSetVisibility(elementIds, visibility);
}

std::shared_ptr<ui::Button> UI::createButtonAnchored(const std::string& text, 
                                                     std::shared_ptr<ui::UIElement> target,
                                                     ui::AnchorType anchorType, int offset,
                                                     std::function<void()> callback) {
    auto button = createButton(text, 0, 0, callback);
    button->setAnchor(target, anchorType, offset);
    return button;
}

std::shared_ptr<ui::Label> UI::createLabelAnchored(const std::string& text,
                                                   std::shared_ptr<ui::UIElement> target,
                                                   ui::AnchorType anchorType, int offset) {
    auto label = createLabel(text, 0, 0);
    label->setAnchor(target, anchorType, offset);
    return label;
}
