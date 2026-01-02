/**
 * @file UI.h
 * @brief Main UI framework interface providing high-level widget creation and management
 *
 * This is the primary interface for the UI framework, offering a simplified API for creating
 * and managing UI elements. All widgets are thread-safe and use modern C++17 features.
 *
 * @author UI Framework Team
 * @version 2.0
 * @date 2024
 */

#pragma once
#include <functional>
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include "ErrorHandling.h"
#include "UICore.h"
#include "UIElements.h"
#include "uiframework/Resources/ElementPool.h"
#include "uiframework/RealTime/UIUpdateQueue.h"

/**
 * @class UI
 * @brief Main UI framework class providing high-level widget creation and management
 *
 * The UI class serves as the primary interface for creating and managing UI elements.
 * It provides a simplified API that handles resource management, thread safety, and
 * proper cleanup automatically.
 *
 * Key features:
 * - Thread-safe operations with automatic synchronization
 * - RAII-based resource management with smart pointers
 * - Comprehensive error handling with specific exception types
 * - 17+ widget types with consistent API
 * - Performance optimizations (texture caching, font caching)
 *
 * @section thread_safety Thread Safety Guarantees
 *
 * **THREAD-SAFE METHODS** (can be called from any thread):
 * - All widget creation methods (createButton, createLabel, etc.)
 * - Element management (removeElement, getElement, addElement)
 * - Focus management (setFocus, focusNext, focusPrevious, etc.)
 * - Hotkey management (assignHotKey)
 * - Modal creation (createModal, createConfirmModal, createInfoModal)
 * - Theme management (setTheme)
 * - Utility methods (getWidth, getHeight)
 *
 * **NOT THREAD-SAFE** (must be called from main thread only):
 * - run() - Main event loop, blocks until window closes
 * - Constructor and destructor
 *
 * **IMPLEMENTATION DETAILS:**
 * - Internal mutexes protect all shared state (elements, themes, hotkeys, focus)
 * - Widget creation is atomic and returns fully initialized objects
 * - Element removal is synchronized and handles focus cleanup automatically
 * - Focus operations are queued and processed safely during event loop
 *
 * @example
 * @code
 * try {
 *     UI ui("My Application", 800, 600);
 *
 *     auto button = ui.createButton("Click me!", 10, 10, [](){
 *         std::cout << "Button clicked!" << std::endl;
 *     });
 *
 *     auto label = ui.createLabel("Hello World", 10, 50);
 *
 *     ui.run();
 * } catch (const ui::UIException& e) {
 *     std::cerr << "UI Error: " << e.what() << std::endl;
 * }
 * @endcode
 */
class UI {
  private:
    std::unique_ptr<ui::UICore> core;
    std::unique_ptr<ui::ConstraintManager> constraintManager;
    std::unordered_map<std::string, int> focusOrderHints;
    std::unique_ptr<ui::ElementPool> elementPool;
    std::unique_ptr<ui::UIUpdateQueue> updateQueue;
    std::vector<ui::ElementCache> elementCache; // Non-blocking element cache
    int gridSize = 0; // Grid size for snapping (0 = disabled)

    // Helper method to register element and return shared_ptr
    template <typename T>
    std::shared_ptr<T> registerElement(std::shared_ptr<T> element) {
        if (!element) {
            throw ui::UIException("Cannot register null element");
        }
        core->addElement(element);
        // Set constraint manager reference for constraint-based positioning
        element->setConstraintManager(constraintManager.get());
        return element;
    }

  public:
    /**
     * @brief Constructs a new UI instance with the specified window properties
     *
     * Creates a new UI framework instance with an SDL window of the given dimensions.
     * Initializes all necessary SDL subsystems, font management, and core resources.
     *
     * @param title Window title displayed in the title bar
     * @param width Window width in pixels (must be > 0)
     * @param height Window height in pixels (must be > 0)
     *
     * @throws ui::InitializationException if SDL initialization fails
     * @throws ui::UIException if window creation fails
     * @throws std::invalid_argument if width or height <= 0
     *
     * @example
     * @code
     * UI ui("My App", 1024, 768);  // Creates 1024x768 window
     * @endcode
     */
    UI(const char* title, int width, int height);

    /**
     * @brief Destructor - automatically cleans up all resources
     *
     * Properly destroys all UI elements, releases SDL resources, and performs
     * cleanup. Uses RAII principles for automatic resource management.
     */
    ~UI() = default;

    // ========== Widget Creation Methods ==========

    /**
     * @brief Creates a text label widget
     *
     * Creates a non-interactive text display widget with automatic text caching
     * for optimal performance. Labels support theme-based styling and automatic
     * text truncation.
     *
     * @param text Text content to display
     * @param x X coordinate in pixels
     * @param y Y coordinate in pixels
     * @return std::shared_ptr<ui::Label> Shared pointer to the created label
     *
     * @throws ui::UIException if label creation fails
     *
     * @example
     * @code
     * auto title = ui.createLabel("Application Title", 10, 10);
     * title->setSize(200, 30);
     * @endcode
     */
    std::shared_ptr<ui::Label> createLabel(const std::string& text, int x, int y);

    /**
     * @brief Creates a clickable button widget
     *
     * Creates an interactive button with click callback support. Buttons provide
     * visual feedback (hover, press states) and keyboard navigation support.
     *
     * @param text Button label text
     * @param x X coordinate in pixels
     * @param y Y coordinate in pixels
     * @param callback Function called when button is clicked
     * @return std::shared_ptr<ui::Button> Shared pointer to the created button
     *
     * @throws ui::UIException if button creation fails
     *
     * @example
     * @code
     * auto saveBtn = ui.createButton("Save", 10, 50, [&](){
     *     saveDocument();
     *     showMessage("Document saved!");
     * });
     * @endcode
     */
    std::shared_ptr<ui::Button> createButton(const std::string& text, int x, int y,
                                             std::function<void()> callback);
    
    /**
     * @brief Creates a button with focus order hint
     * @param text Button text
     * @param x X coordinate
     * @param y Y coordinate  
     * @param callback Click callback
     * @param focusOrder Focus order hint (lower = earlier in tab sequence)
     * @return Shared pointer to the created button
     */
    std::shared_ptr<ui::Button> createButton(const std::string& text, int x, int y,
                                             std::function<void()> callback, int focusOrder);

    /**
     * @brief Creates a text input widget
     *
     * Creates an editable text input field with cursor support, text selection,
     * and keyboard input handling. Supports placeholder text and auto-highlighting.
     *
     * @param defaultText Initial text content or placeholder
     * @param x X coordinate in pixels
     * @param y Y coordinate in pixels
     * @param autoHighlight If true, automatically selects all text when focused
     * @return std::shared_ptr<ui::TextBox> Shared pointer to the created text box
     *
     * @throws ui::UIException if text box creation fails
     *
     * @example
     * @code
     * auto nameInput = ui.createTextBox("Enter your name...", 10, 90, true);
     * nameInput->setSize(200, 30);
     * @endcode
     */
    std::shared_ptr<ui::TextBox> createTextBox(const std::string& defaultText, int x, int y,
                                               bool autoHighlight = true);
    
    /**
     * @brief Creates a text box with focus order hint
     * @param defaultText Initial text content or placeholder
     * @param x X coordinate
     * @param y Y coordinate
     * @param autoHighlight Auto-select text when focused
     * @param focusOrder Focus order hint (lower = earlier in tab sequence)
     * @return Shared pointer to the created text box
     */
    std::shared_ptr<ui::TextBox> createTextBox(const std::string& defaultText, int x, int y,
                                               bool autoHighlight, int focusOrder);
    /**
     * @brief Creates a checkbox widget with state callback
     *
     * Creates a toggleable checkbox with visual state indication and callback
     * support for state changes. Supports keyboard navigation and theming.
     *
     * @param state Initial checked state (true = checked, false = unchecked)
     * @param x X coordinate in pixels
     * @param y Y coordinate in pixels
     * @param callback Function called when checkbox state changes
     * @return std::shared_ptr<ui::CheckBox> Shared pointer to the created checkbox
     *
     * @throws ui::UIException if checkbox creation fails
     *
     * @example
     * @code
     * bool enableFeature = false;
     * auto checkbox = ui.createCheckBox(enableFeature, 10, 130, [&](bool checked){
     *     enableFeature = checked;
     *     std::cout << "Feature " << (checked ? "enabled" : "disabled") << std::endl;
     * });
     * @endcode
     */
    std::shared_ptr<ui::CheckBox> createCheckBox(bool state, int x, int y,
                                                 std::function<void(bool)> callback);

    /**
     * @brief Creates a dropdown selection widget
     *
     * Creates a dropdown menu for selecting from multiple options. Supports
     * keyboard navigation, search, and dynamic option updates.
     *
     * @param options Vector of option strings to display
     * @param current Index of initially selected option
     * @param x X coordinate in pixels
     * @param y Y coordinate in pixels
     * @param callback Function called when selection changes (receives new index)
     * @return std::shared_ptr<ui::OptionSelect> Shared pointer to the created option select
     *
     * @throws ui::UIException if option select creation fails
     * @throws std::out_of_range if current index is invalid
     *
     * @example
     * @code
     * std::vector<std::string> themes = {"Dark", "Light", "Auto"};
     * auto themeSelect = ui.createOptionSelect(themes, 0, 10, 170, [&](int index){
     *     applyTheme(themes[index]);
     * });
     * @endcode
     */
    std::shared_ptr<ui::OptionSelect> createOptionSelect(const std::vector<std::string>& options,
                                                         int current, int x, int y,
                                                         std::function<void(int)> callback);
    std::shared_ptr<ui::Canvas> createCanvas(int x, int y, int width, int height);
    std::shared_ptr<ui::ContextMenu> createContextMenu(const std::vector<ui::TopMenuItem>& menus);
    std::shared_ptr<ui::ListView> createListView(const std::vector<std::string>& items, int x,
                                                 int y, int width, int height, int itemHeight = 30);
    std::shared_ptr<ui::Image> createImage(const std::string& path, int x, int y, int width, int height,
                                           bool stretch = false);
    std::shared_ptr<ui::Image> createImage(const unsigned char* data, size_t dataSize, int x, int y,
                                           int width, int height, bool stretch = false);
    std::shared_ptr<ui::Sprite> createSprite(const std::string& path, int x, int y, int width, int height,
                                             bool stretch);
    std::shared_ptr<ui::Sprite> createSprite(const unsigned char* data, size_t dataSize, int x,
                                             int y, int width, int height, bool stretch);
    std::shared_ptr<ui::AnimatedSprite> createAnimatedSprite(const std::string& path, int x, int y,
                                                             int width, int height, int frameCount,
                                                             Uint32 frameDelay,
                                                             bool stretch = false);
    std::shared_ptr<ui::AnimatedSprite> createAnimatedSprite(const unsigned char* data,
                                                             size_t dataSize, int x, int y, int width,
                                                             int height, int frameCount,
                                                             Uint32 frameDelay,
                                                             bool stretch = false);
    std::shared_ptr<ui::ProgressBar> createProgressBar(int x, int y, int width, int height,
                                                       float initValue = 0.0f,
                                                       bool showText = true);
    std::shared_ptr<ui::HSlider> createHSlider(int x, int y, int width, int height,
                                               float min = 0.0f, float max = 100.0f,
                                               float initial = 0.0f);
    std::shared_ptr<ui::VSlider> createVSlider(int x, int y, int width, int height,
                                               float min = 0.0f, float max = 100.0f,
                                               float initial = 0.0f);
    std::shared_ptr<ui::KnobSlider> createKnobSlider(int x, int y, int size, float min = 0.0f,
                                                     float max = 100.0f, float initial = 0.0f);
    std::shared_ptr<ui::VirtualKeyboard> createVirtualKeyboard(int x, int y, int width, int height,
                                                               std::function<void(char)> callback);
    std::shared_ptr<ui::CycleList> createCycleList(int x, int y, int width, int height,
                                                   const std::vector<std::string>& items);
    std::shared_ptr<ui::TabbedPanel> createTabbedPanel(int x, int y, int width, int height);

    // Backward compatibility methods (deprecated - will be removed in future versions)
    [[deprecated("Use createLabel instead")]]
    ui::Label* label(const std::string& text, int x, int y);
    [[deprecated("Use createButton instead")]]
    ui::Button* button(const std::string& text, int x, int y, std::function<void()> callback);
    [[deprecated("Use createTextBox instead")]]
    ui::TextBox* textBox(const std::string& defaultText, int x, int y, bool autoHighlight = true);
    [[deprecated("Use createCheckBox instead")]]
    ui::CheckBox* checkBox(bool state, int x, int y, std::function<void(bool)> callback);
    [[deprecated("Use createOptionSelect instead")]]
    ui::OptionSelect* optionSelect(int current, const std::vector<std::string>& options, int x,
                                   int y, std::function<void(int)> callback);
    [[deprecated("Use createCanvas instead")]]
    ui::Canvas* canvas(int x, int y, int width, int height);
    [[deprecated("Use createContextMenu instead")]]
    ui::ContextMenu* contextMenu(const std::vector<ui::TopMenuItem>& menus);
    [[deprecated("Use createListView instead")]]
    ui::ListView* listView(const std::vector<std::string>& items, int x, int y, int w, int h,
                           int itemHeight = 30);
    [[deprecated("Use createImage instead")]]
    ui::Image* image(const std::string& path, int x, int y, int w, int h, bool stretch = false);
    [[deprecated("Use createImage instead")]]
    ui::Image* image(const unsigned char* data, size_t dataSize, int x, int y, int w, int h,
                     bool stretch = false);
    [[deprecated("Use createSprite instead")]]
    ui::Sprite* sprite(const std::string& path, int x, int y, int w, int h, bool stretch);
    [[deprecated("Use createSprite instead")]]
    ui::Sprite* sprite(const unsigned char* data, size_t dataSize, int x, int y, int w, int h,
                       bool stretch);
    [[deprecated("Use createAnimatedSprite instead")]]
    ui::AnimatedSprite* animatedSprite(const std::string& path, int x, int y, int w, int h,
                                       int frameCount, Uint32 frameDelay, bool stretch = false);
    [[deprecated("Use createAnimatedSprite instead")]]
    ui::AnimatedSprite* animatedSprite(const unsigned char* data, size_t dataSize, int x, int y,
                                       int w, int h, int frameCount, Uint32 frameDelay,
                                       bool stretch = false);
    [[deprecated("Use createProgressBar instead")]]
    ui::ProgressBar* progressBar(int x, int y, int width, int height, float initValue = 0.0f,
                                 bool showText = true);

    // Element management
    void removeElement(const std::string& elementId);
    void removeElement(uint64_t numericId); // Performance version
    std::shared_ptr<ui::UIElement> getElement(const std::string& elementId);
    std::shared_ptr<ui::UIElement> getElement(uint64_t numericId); // Performance version

    // Hotkey management - now uses element IDs instead of raw pointers
    void assignHotKey(const std::string& elementId, const std::string& hotKey);
    void assignHotKey(const std::string& elementId, const std::string& hotKey,
                      std::function<void()> customCallback);
    void assignHotKey(std::shared_ptr<ui::UIElement> element, const std::string& hotKey);
    void assignHotKey(std::shared_ptr<ui::UIElement> element, const std::string& hotKey,
                      std::function<void()> customCallback);

    // Focus management - now uses element IDs
    void setFocus(const std::string& elementId);
    void setFocus(std::shared_ptr<ui::UIElement> element);
    std::string getFocusedElementId() const;
    
    // Focus cycling (Phase 2)
    void focusNext();
    void focusPrevious();
    void setFocusOrder(const std::vector<std::string>& elementIds);
    
    // Focus groups and trapping (Phase 4)
    void createFocusGroup(const std::string& groupName, const std::vector<std::string>& elementIds);
    void setActiveFocusGroup(const std::string& groupName);
    void clearActiveFocusGroup();
    void trapFocus(const std::string& groupName);
    void releaseFocusTrap();
    
    /**
     * @brief Apply focus order based on hints provided during element creation
     * Automatically sorts elements by their focus order hints and applies the order
     */
    void applyFocusOrderHints();

    // Modal APIs with better error handling
    std::shared_ptr<ui::Modal> createModal(const std::string& message,
                                           const std::string& buttonText, bool hasCancel,
                                           std::function<void()> onCloseCallback = nullptr);
    std::shared_ptr<ui::Modal> createConfirmModal(const std::string& message,
                                                  std::function<void()> onConfirmCallback = nullptr,
                                                  std::function<void()> onCancelCallback = nullptr);
    std::shared_ptr<ui::Modal> createInfoModal(const std::string& message,
                                               std::function<void()> onCloseCallback = nullptr);

    // Backward compatibility for modals (deprecated)
    [[deprecated("Use createModal instead")]]
    ui::Modal* modal(const std::string& message, const std::string& buttonText, bool hasCancel,
                     std::function<void()> onCloseCallback = nullptr);
    [[deprecated("Use createConfirmModal instead")]]
    ui::Modal* confirmModal(const std::string& message,
                            std::function<void()> onConfirmCallback = nullptr,
                            std::function<void()> onCancelCallback = nullptr);
    [[deprecated("Use createInfoModal instead")]]
    ui::Modal* infoModal(const std::string& message,
                         std::function<void()> onCloseCallback = nullptr);

    // Core functionality
    void run();
    void setTheme(const std::string& themeName);
    void addElement(std::shared_ptr<ui::UIElement> element);

    // Utility methods
    int getWidth() const;
    int getHeight() const;
    
    // Constraint system access
    ui::ConstraintManager* getConstraintManager() const;
    
    // Grid snapping system
    void setGridSize(int gridSize);
    int getGridSize() const;
    
    // Element pooling for high-frequency scenarios (audio instruments, real-time apps)
    void enableElementPooling(size_t labelCount = 50, size_t buttonCount = 20);
    std::shared_ptr<ui::Label> createLabelPooled(const std::string& text, int x, int y);
    std::shared_ptr<ui::Button> createButtonPooled(const std::string& text, int x, int y, 
                                                   std::function<void()> callback);
    void releasePooledElement(const std::string& elementId);
    struct PoolStats { size_t labelsAvailable, buttonsAvailable, labelsInUse, buttonsInUse; };
    PoolStats getPoolStats() const;
    
    // Real-time safe update methods (lock-free, high-frequency safe)
    bool realtimeSetText(const std::string& elementId, const std::string& text);
    bool realtimeSetPosition(const std::string& elementId, int x, int y);
    bool realtimeSetSize(const std::string& elementId, int width, int height);
    bool realtimeSetValue(const std::string& elementId, float value);
    bool realtimeSetVisibility(const std::string& elementId, bool visible);
    bool realtimeCallback(std::function<void()> callback);
    
    // High-resolution timing methods (microsecond precision)
    bool realtimeSetTextAtTime(const std::string& elementId, const std::string& text, 
                              std::chrono::high_resolution_clock::time_point when);
    bool realtimeSetPositionAtTime(const std::string& elementId, int x, int y,
                                  std::chrono::high_resolution_clock::time_point when);
    bool realtimeSetValueAtTime(const std::string& elementId, float value,
                               std::chrono::high_resolution_clock::time_point when);
    bool realtimeSetVisibilityAtTime(const std::string& elementId, bool visible,
                                    std::chrono::high_resolution_clock::time_point when);
    
    // Bulk update methods (SIMD optimized for 100+ elements)
    size_t realtimeBulkSetText(const std::vector<std::string>& elementIds, 
                              const std::vector<std::string>& textValues);
    size_t realtimeBulkSetPosition(const std::vector<std::string>& elementIds,
                                  const std::vector<std::pair<int, int>>& positions);
    size_t realtimeBulkSetValue(const std::vector<std::string>& elementIds,
                               const std::vector<float>& values);
    size_t realtimeBulkSetVisibility(const std::vector<std::string>& elementIds,
                                    const std::vector<bool>& visibility);
    
    // Process queued updates (call from main thread only)
    void processRealtimeUpdates();
};
