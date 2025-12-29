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
#include <memory>
#include "ErrorHandling.h"
#include "UICore.h"
#include "UIWidgetFactory.h"
#include "UILayoutFactory.h"
#include "UIDeprecated.h"

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
 * - Layout management system (VBox, HBox, Grid)
 * - Performance optimizations (texture caching, font caching)
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
class UI : public UIWidgetFactory, public UILayoutFactory, public UIDeprecated {
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
     */
    UI(const char* title, int width, int height);

    /**
     * @brief Destructor - automatically cleans up all resources
     *
     * Properly destroys all UI elements, releases SDL resources, and performs
     * cleanup. Uses RAII principles for automatic resource management.
     */
    ~UI() = default;

    // ========== Core Framework Methods ==========

    /**
     * @brief Starts the main event loop
     *
     * Runs the UI framework's main event loop, handling user input, rendering,
     * and managing the application lifecycle. This method blocks until the
     * application is closed.
     *
     * The event loop handles:
     * - SDL events (mouse, keyboard, window events)
     * - Widget updates and rendering
     * - Theme application
     * - Focus management
     * - Performance optimizations
     *
     * @throws ui::UIException if rendering fails
     *
     * @example
     * @code
     * UI ui("My App", 800, 600);
     * // Create widgets...
     * ui.run();  // Blocks until window is closed
     * @endcode
     */
    void run();

    /**
     * @brief Sets the UI theme
     *
     * Changes the visual appearance of all UI elements by applying a new theme.
     * Themes control colors, fonts, spacing, and visual effects.
     *
     * @param theme Shared pointer to the new theme to apply
     *
     * @throws ui::UIException if theme application fails
     *
     * @example
     * @code
     * auto darkTheme = std::make_shared<ui::ThemeMolokai>();
     * ui.setTheme(darkTheme);
     * @endcode
     */
    void setTheme(std::shared_ptr<ui::Theme> theme);

    /**
     * @brief Gets the current theme
     *
     * @return std::shared_ptr<ui::Theme> Current theme being used
     */
    std::shared_ptr<ui::Theme> getTheme() const;

    /**
     * @brief Registers a hotkey callback
     *
     * Associates a keyboard shortcut with a callback function. When the specified
     * key is pressed, the callback will be executed.
     *
     * @param key SDL keycode for the hotkey
     * @param callback Function to call when hotkey is pressed
     * @return bool True if hotkey was registered successfully
     *
     * @example
     * @code
     * ui.registerHotKey(SDLK_s, [&](){ saveDocument(); });
     * ui.registerHotKey(SDLK_q, [&](){ quit(); });
     * @endcode
     */
    bool registerHotKey(SDL_Keycode key, std::function<void()> callback);

    /**
     * @brief Unregisters a hotkey
     *
     * @param key SDL keycode to unregister
     */
    void unregisterHotKey(SDL_Keycode key);

    // ========== Utility Methods ==========

    /**
     * @brief Gets the renderer for custom drawing
     *
     * @return SDL_Renderer* Pointer to the SDL renderer
     */
    SDL_Renderer* getRenderer() const;

    /**
     * @brief Gets the current font
     *
     * @return TTF_Font* Pointer to the current font
     */
    TTF_Font* getFont() const;

    /**
     * @brief Gets the window width
     *
     * @return int Window width in pixels
     */
    int getWidth() const;

    /**
     * @brief Gets the window height
     *
     * @return int Window height in pixels
     */
    int getHeight() const;
};
