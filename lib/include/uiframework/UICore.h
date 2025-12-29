#pragma once

#include "uiframework/Theme/ThemeBase.h"
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

    class UICore {
    private:
        std::unique_ptr<SDLResources> resources;
        std::vector<std::shared_ptr<UIElement>> elements;
    std::unordered_map<std::string, std::shared_ptr<UIElement>> elementsMap;
        std::shared_ptr<Theme> currentTheme;
        std::unordered_map<SDL_Keycode, std::function<void()>> hotKeys;
        
        // Thread safety
        mutable std::mutex elementsMutex;
        mutable std::mutex themeMutex;
        mutable std::mutex hotKeysMutex;
        
        // Focus management with element IDs instead of indices
        std::string focusedElementId;
        std::unordered_map<std::string, std::weak_ptr<UIElement>> elementRegistry;
        std::queue<std::string> pendingFocusChanges;
        mutable std::mutex focusMutex;
        std::vector<std::function<void()>> pendingCallbacks;
        
        bool modalActive = false;
        int width, height;
        
        // Helper methods
        std::string generateElementId();
        void validateCoordinates(int x, int y, int w, int h) const;
        
    public:
        static SDL_Keycode keycodeFromString(const std::string &s);
        
        UICore(const char* title, int width, int height, std::shared_ptr<Theme> theme);
        ~UICore() = default;
        
        // Thread-safe element management
        std::string addElement(std::shared_ptr<UIElement> element);
        void removeElement(const std::string& elementId);
        std::shared_ptr<UIElement> getElement(const std::string& elementId) const;
        
        // Thread-safe theme management
        void setTheme(std::shared_ptr<Theme> theme);
        std::shared_ptr<Theme> getTheme() const;
        
        // Thread-safe hotkey management
        bool registerHotKey(SDL_Keycode key, std::function<void()> callback);
        void unregisterHotKey(SDL_Keycode key);
        
        // Focus management
        void setFocus(const std::string& elementId);
        std::string getFocusedElementId() const;
        
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
