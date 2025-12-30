#pragma once

#include "uiframework/Theme/ThemeBase.h"
#include "uiframework/Focus/FocusManager.h"
#include "UIElements.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include <memory>
#include <unordered_map>
#include <mutex>
#include <queue>
#include <stdexcept>

namespace ui {
    class SDLResources {
    private:
        SDL_Window* window = nullptr;
        SDL_Renderer* renderer = nullptr;
        TTF_Font* font = nullptr;
        bool sdl_initialized = false;
        
    public:
        SDLResources(const char* title, int width, int height);
        ~SDLResources();
        
        // Non-copyable, movable
        SDLResources(const SDLResources&) = delete;
        SDLResources& operator=(const SDLResources&) = delete;
        SDLResources(SDLResources&&) = default;
        SDLResources& operator=(SDLResources&&) = default;
        
        SDL_Window* getWindow() const { return window; }
        SDL_Renderer* getRenderer() const { return renderer; }
        TTF_Font* getFont() const { return font; }
        
        void loadFont(const std::string& fontPath, int fontSize);
    };

    /**
     * @class UICore
     * @brief Core UI framework implementation with thread-safe element and resource management
     *
     * UICore provides the low-level implementation for the UI framework, handling
     * SDL resources, element management, theme management, and focus coordination.
     * All public methods are thread-safe unless explicitly noted.
     *
     * @section thread_safety Thread Safety Implementation
     *
     * **THREAD-SAFE METHODS** (protected by internal mutexes):
     * - Element management: addElement, removeElement, getElement
     * - Theme management: setTheme, getTheme
     * - Hotkey management: assignHotKey, removeHotKey
     * - Focus management: all focus-related operations (delegated to FocusManager)
     * - Utility methods: getWidth, getHeight, getRenderer, etc.
     *
     * **MUTEX PROTECTION:**
     * - elementsMutex: Protects elements vector and maps
     * - themeMutex: Protects theme operations
     * - hotKeysMutex: Protects hotkey assignments
     * - FocusManager has its own focusMutex for focus operations
     *
     * **NOT THREAD-SAFE** (main thread only):
     * - Constructor and destructor
     * - Event processing and rendering operations
     */
    class UICore {
    private:
        std::unique_ptr<SDLResources> resources;
        std::vector<std::shared_ptr<UIElement>> elements;
        std::unordered_map<std::string, std::shared_ptr<UIElement>> elementsMap;
        std::unordered_map<uint64_t, std::shared_ptr<UIElement>> numericElementsMap; // Performance lookup
        std::shared_ptr<Theme> currentTheme;
        std::unordered_map<SDL_Keycode, std::function<void()>> hotKeys;
        
        // Thread safety
        mutable std::mutex elementsMutex;
        mutable std::mutex themeMutex;
        mutable std::mutex hotKeysMutex;
        
        // Focus management (delegated to FocusManager)
        std::unique_ptr<FocusManager> focusManager;
        
        bool modalActive = false;
        int width, height;
        
        // Helper methods
        std::string generateElementId();
        uint64_t generateNumericId();
        void validateCoordinates(int x, int y, int w, int h) const;
        
    public:
        static SDL_Keycode keycodeFromString(const std::string &s);
        
        UICore(const char* title, int width, int height, std::shared_ptr<Theme> theme);
        ~UICore() = default;
        
        // Thread-safe element management
        std::string addElement(std::shared_ptr<UIElement> element);
        void removeElement(const std::string& elementId);
        void removeElement(uint64_t numericId); // Performance version
        std::shared_ptr<UIElement> getElement(const std::string& elementId) const;
        std::shared_ptr<UIElement> getElement(uint64_t numericId) const; // Performance version
        
        // Thread-safe theme management
        void setTheme(std::shared_ptr<Theme> theme);
        std::shared_ptr<Theme> getTheme() const;
        
        // Thread-safe hotkey management
        bool registerHotKey(SDL_Keycode key, std::function<void()> callback);
        void unregisterHotKey(SDL_Keycode key);
        
        // Focus management
        void setFocus(const std::string& elementId);
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
        
        // Callback management
        void queueCallback(std::function<void()> callback);
        
        void showQuitConfirmation(bool &quit);
        void run();
        
        // Getters for rendering
        SDL_Renderer* getRenderer() const { return resources->getRenderer(); }
        TTF_Font* getFont() const { return resources->getFont(); }
        int getWidth() const { return width; }
        int getHeight() const { return height; }
        bool isModalActive() const { return modalActive; }
        void setModalActive(bool active) { modalActive = active; }
    };
}
