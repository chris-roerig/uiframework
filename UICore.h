#pragma once

#include "Theme/ThemeBase.h"
#include "Theme/ThemeGlobals.h"
#include "UIElements.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include <memory>
#include <unordered_map>

namespace ui {
    class UICore {
    public:
        SDL_Window* window = nullptr;
        SDL_Renderer* renderer = nullptr;
        std::vector<std::shared_ptr<UIElement>> elements;
        std::shared_ptr<Theme> currentTheme;
        bool modalActive = false;
        std::unordered_map<SDL_Keycode, std::function<void()>> hotKeys;
        int focusedIndex = -1;
        int width, height;

        static SDL_Keycode keycodeFromString(const std::string &s);
        UICore(const char* title, int width, int height, std::shared_ptr<Theme> theme);
        ~UICore();
        bool registerHotKey(SDL_Keycode key, std::function<void()> callback);
        void addElement(std::shared_ptr<UIElement> element);
        void setTheme(std::shared_ptr<Theme> theme);
        void setFocus(UIElement* element);
        void showQuitConfirmation(bool &quit);
        void run();
    };
}
