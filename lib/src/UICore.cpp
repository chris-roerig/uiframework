#include "uiframework/UICore.h"
#include "uiframework/Helpers.h"
#include "uiframework/Resources/FontManager.h"
#include "uiframework/ErrorHandling.h"
#include <iostream>
#include <atomic>
#include <sstream>
#include <typeinfo>

namespace ui {

// --- SDLResources Implementation ---
SDLResources::SDLResources(const char* title, int width, int height) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        throw InitializationException("SDL_Init failed: " + std::string(SDL_GetError()));
    }
    sdl_initialized = true;
    
    // TTF initialization is now handled by FontManager
    
    window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              width, height, SDL_WINDOW_SHOWN);
    if (!window) {
        throw InitializationException("SDL_CreateWindow failed: " + std::string(SDL_GetError()));
    }
    
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        throw InitializationException("SDL_CreateRenderer failed: " + std::string(SDL_GetError()));
    }
    
    SDL_StartTextInput();
    
    // Get default font from FontManager
    font = ui::FontManager::getInstance().getFont("", 10);
    if (!font) {
        std::cerr << "Warning: Could not load default font from FontManager" << std::endl;
    }
}

SDLResources::~SDLResources() {
    SDL_StopTextInput();
    
    // Font is managed by FontManager, don't close it here
    font = nullptr;
    
    if (renderer) {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }
    
    if (window) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
    
    // TTF cleanup is handled by FontManager
    
    if (sdl_initialized) {
        SDL_Quit();
        sdl_initialized = false;
    }
}

void SDLResources::loadFont(const std::string& fontPath, int fontSize) {
    // Use FontManager instead of direct loading
    font = ui::FontManager::getInstance().getFont(fontPath, fontSize);
    if (!font) {
        std::cerr << "Warning: FontManager could not load font: " << (fontPath.empty() ? "default" : fontPath) << std::endl;
    }
}

// --- UICore Implementation ---
UICore::UICore(const char* title, int w, int h, std::shared_ptr<Theme> theme)
    : currentTheme(theme), focusManager(std::make_unique<FocusManager>()), width(w), height(h) {
    
    if (!theme) {
        throw std::invalid_argument("Theme cannot be null");
    }
    
    if (w <= 0 || h <= 0) {
        throw std::invalid_argument("Window dimensions must be positive");
    }
    
    try {
        resources = std::make_unique<SDLResources>(title, w, h);
    } catch (const InitializationException& e) {
        throw; // Re-throw initialization exceptions
    } catch (const std::exception& e) {
        throw InitializationException("Unexpected error during initialization: " + std::string(e.what()));
    }
}

std::string UICore::generateElementId() {
    static std::atomic<uint64_t> counter{0};
    std::ostringstream oss;
    oss << "element_" << counter.fetch_add(1);
    return oss.str();
}

void UICore::validateCoordinates(int x, int y, int w, int h) const {
    if (x < 0 || y < 0) {
        throw std::invalid_argument("Element coordinates cannot be negative: x=" + std::to_string(x) + ", y=" + std::to_string(y));
    }
    if (w <= 0 || h <= 0) {
        throw std::invalid_argument("Element dimensions must be positive: width=" + std::to_string(w) + ", height=" + std::to_string(h));
    }
    if (x + w > width || y + h > height) {
        throw std::invalid_argument("Element extends beyond window boundaries: element(" + 
                                  std::to_string(x) + "," + std::to_string(y) + "," + 
                                  std::to_string(w) + "x" + std::to_string(h) + 
                                  ") exceeds window(" + std::to_string(width) + "x" + std::to_string(height) + ")");
    }
}

std::string UICore::addElement(std::shared_ptr<UIElement> element) {
    if (!element) {
        throw std::invalid_argument("Cannot add null element");
    }
    
    try {
        validateCoordinates(element->x, element->y, element->width, element->height);
    } catch (const std::invalid_argument& e) {
        // Add element type information to the error
        std::string elementType = typeid(*element).name();
        // Clean up the mangled name for common types
        if (elementType.find("Button") != std::string::npos) elementType = "Button";
        else if (elementType.find("Label") != std::string::npos) elementType = "Label";
        else if (elementType.find("TextBox") != std::string::npos) elementType = "TextBox";
        else if (elementType.find("Canvas") != std::string::npos) elementType = "Canvas";
        else if (elementType.find("LayoutContainer") != std::string::npos) elementType = "LayoutContainer";
        else if (elementType.find("ListView") != std::string::npos) elementType = "ListView";
        else if (elementType.find("ProgressBar") != std::string::npos) elementType = "ProgressBar";
        else if (elementType.find("Slider") != std::string::npos) elementType = "Slider";
        
        throw std::invalid_argument("Element validation failed for " + elementType + ": " + e.what());
    }
    
    std::lock_guard<std::mutex> lock(elementsMutex);
    
    std::string id = generateElementId();
    element->setId(id);
    element->setCoreReference(this);
    
    elements.push_back(element);
    elementsMap[id] = element;
    
    // Register with FocusManager
    focusManager->registerElement(id, element);
    
    return id;
}

void UICore::removeElement(const std::string& elementId) {
    std::lock_guard<std::mutex> lock(elementsMutex);
    
    // O(1) lookup in map
    auto mapIt = elementsMap.find(elementId);
    if (mapIt == elementsMap.end()) {
        return; // Element not found
    }
    
    auto element = mapIt->second;
    
    // Remove from map (O(1))
    elementsMap.erase(mapIt);
    
    // Remove from vector (O(n) but necessary for render order)
    elements.erase(
        std::remove(elements.begin(), elements.end(), element),
        elements.end()
    );
    
    // Unregister from FocusManager
    focusManager->unregisterElement(elementId);
}

std::shared_ptr<ui::UIElement> UICore::getElement(const std::string& elementId) const {
    std::lock_guard<std::mutex> lock(elementsMutex);
    
    // O(1) lookup in map
    auto it = elementsMap.find(elementId);
    return (it != elementsMap.end()) ? it->second : nullptr;
}

void UICore::setTheme(std::shared_ptr<ui::Theme> theme) {
    if (!theme) {
        throw std::invalid_argument("Theme cannot be null");
    }
    
    std::lock_guard<std::mutex> lock(themeMutex);
    currentTheme = theme;
}

std::shared_ptr<ui::Theme> UICore::getTheme() const {
    std::lock_guard<std::mutex> lock(themeMutex);
    return currentTheme;
}

bool UICore::registerHotKey(SDL_Keycode key, std::function<void()> callback) {
    if (!callback) {
        return false;
    }
    
    std::lock_guard<std::mutex> lock(hotKeysMutex);
    
    if (hotKeys.find(key) != hotKeys.end()) {
        return false; // Key already registered
    }
    
    hotKeys[key] = callback;
    return true;
}

void UICore::unregisterHotKey(SDL_Keycode key) {
    std::lock_guard<std::mutex> lock(hotKeysMutex);
    hotKeys.erase(key);
}

void UICore::setFocus(const std::string& elementId) {
    focusManager->setFocus(elementId);
}

std::string UICore::getFocusedElementId() const {
    return focusManager->getFocusedElementId();
}

void UICore::focusNext() {
    focusManager->focusNext();
}

void UICore::focusPrevious() {
    focusManager->focusPrevious();
}

void UICore::setFocusOrder(const std::vector<std::string>& elementIds) {
    focusManager->setFocusOrder(elementIds);
}

void UICore::queueCallback(std::function<void()> callback) {
    focusManager->queueCallback(callback);
}

SDL_Keycode UICore::keycodeFromString(const std::string &s) {
    if (s.empty())
        return SDLK_UNKNOWN;
    char ch = s[0];
    ch = static_cast<char>(std::tolower(ch));
    if (std::isdigit(ch))
        return SDLK_0 + (ch - '0');
    return ch; // For letters and other characters.
}

void UICore::showQuitConfirmation(bool &quit) {
    // Implementation for quit confirmation
    // This would create a modal dialog
    quit = true; // Simplified for now
}

void UICore::run() {
    if (!resources) {
        throw RenderException("Resources not initialized");
    }
    
    bool quit = false;
    SDL_Event e;
    
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                showQuitConfirmation(quit);
                continue;
            }
            
            // Handle hotkeys
            if (e.type == SDL_KEYDOWN) {
                // Handle Tab navigation first
                if (e.key.keysym.sym == SDLK_TAB) {
                    if (e.key.keysym.mod & KMOD_SHIFT) {
                        focusManager->focusPrevious();
                    } else {
                        focusManager->focusNext();
                    }
                    continue;
                }
                
                std::lock_guard<std::mutex> lock(hotKeysMutex);
                auto it = hotKeys.find(e.key.keysym.sym);
                if (it != hotKeys.end()) {
                    it->second();
                    continue;
                }
            }
            
            // Handle element events
            {
                std::lock_guard<std::mutex> lock(elementsMutex);
                for (auto& element : elements) {
                    if (element && element->isVisible()) {
                        element->handleEvent(e);
                    }
                }
            }
            
            // Process pending focus changes and callbacks
            focusManager->processPendingFocusChanges();
            focusManager->processPendingCallbacks();
        }
        
        // Render
        try {
            SDL_SetRenderDrawColor(resources->getRenderer(), 0, 0, 0, ui::Constants::FULL_ALPHA);
            SDL_RenderClear(resources->getRenderer());
            
            auto theme = getTheme();
            {
                std::lock_guard<std::mutex> lock(elementsMutex);
                for (auto& element : elements) {
                    if (element && element->isVisible()) {
                        element->render(resources->getRenderer(), resources->getFont(), theme);
                    }
                }
            }
            
            SDL_RenderPresent(resources->getRenderer());
        } catch (const std::exception& e) {
            std::cerr << "Render error: " << e.what() << std::endl;
        }
        
        SDL_Delay(16); // ~60 FPS
    }
}

} // namespace ui
