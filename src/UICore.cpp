#include "UICore.h"
#include "Helpers.h"
#include <iostream>
#include <atomic>
#include <sstream>

namespace ui {

// --- SDLResources Implementation ---
SDLResources::SDLResources(const char* title, int width, int height) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        throw InitializationException("SDL_Init failed: " + std::string(SDL_GetError()));
    }
    sdl_initialized = true;
    
    if (TTF_Init() == -1) {
        throw InitializationException("TTF_Init failed: " + std::string(TTF_GetError()));
    }
    ttf_initialized = true;
    
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
    
    // Load default font
    try {
        loadFont("", 16); // Will use default font
    } catch (const std::exception& e) {
        std::cerr << "Warning: Could not load default font: " << e.what() << std::endl;
    }
}

SDLResources::~SDLResources() {
    SDL_StopTextInput();
    
    if (font) {
        TTF_CloseFont(font);
        font = nullptr;
    }
    
    if (renderer) {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }
    
    if (window) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
    
    if (ttf_initialized) {
        TTF_Quit();
        ttf_initialized = false;
    }
    
    if (sdl_initialized) {
        SDL_Quit();
        sdl_initialized = false;
    }
}

void SDLResources::loadFont(const std::string& fontPath, int fontSize) {
    if (font) {
        TTF_CloseFont(font);
        font = nullptr;
    }
    
    if (fontPath.empty()) {
        // Try local font first, then system fonts
        const char* defaultFonts[] = {
            "Assets/default_font.ttf",  // Local project font
            "../Assets/default_font.ttf",  // Local project font (alt path)
            "/System/Library/Fonts/Helvetica.ttc",  // macOS
            "/System/Library/Fonts/Arial.ttf",  // macOS
            "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",  // Linux
            "/usr/share/fonts/TTF/DejaVuSans.ttf",  // Linux alternative
            "C:\\Windows\\Fonts\\arial.ttf"  // Windows
        };
        
        for (const char* defaultFont : defaultFonts) {
            font = TTF_OpenFont(defaultFont, fontSize);
            if (font) {
                std::cout << "Loaded font: " << defaultFont << std::endl;
                return;
            }
        }
        
        // If no system font found, create a minimal fallback
        std::cerr << "Warning: Could not load any system font, UI text may not display properly" << std::endl;
        // Don't throw exception, let the app continue without font
    } else {
        font = TTF_OpenFont(fontPath.c_str(), fontSize);
        if (!font) {
            std::cerr << "Warning: Could not load font: " << fontPath << " - " << std::string(TTF_GetError()) << std::endl;
        }
    }
}

// --- UICore Implementation ---
UICore::UICore(const char* title, int w, int h, std::shared_ptr<Theme> theme)
    : currentTheme(theme), width(w), height(h) {
    
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
        throw std::invalid_argument("Element coordinates cannot be negative");
    }
    if (w <= 0 || h <= 0) {
        throw std::invalid_argument("Element dimensions must be positive");
    }
    if (x + w > width || y + h > height) {
        throw std::invalid_argument("Element extends beyond window boundaries");
    }
}

std::string UICore::addElement(std::shared_ptr<UIElement> element) {
    if (!element) {
        throw std::invalid_argument("Cannot add null element");
    }
    
    validateCoordinates(element->x, element->y, element->width, element->height);
    
    std::lock_guard<std::mutex> lock(elementsMutex);
    
    std::string id = generateElementId();
    element->setId(id);
    element->setCoreReference(this);
    
    elements.push_back(element);
    elementRegistry[id] = element;
    
    return id;
}

void UICore::removeElement(const std::string& elementId) {
    std::lock_guard<std::mutex> lock(elementsMutex);
    
    // Remove from registry
    elementRegistry.erase(elementId);
    
    // Remove from elements vector
    elements.erase(
        std::remove_if(elements.begin(), elements.end(),
            [&elementId](const std::shared_ptr<UIElement>& elem) {
                return elem && elem->getId() == elementId;
            }),
        elements.end()
    );
    
    // Clear focus if this element was focused
    if (focusedElementId == elementId) {
        focusedElementId.clear();
    }
}

std::shared_ptr<UIElement> UICore::getElement(const std::string& elementId) const {
    std::lock_guard<std::mutex> lock(elementsMutex);
    
    auto it = elementRegistry.find(elementId);
    if (it != elementRegistry.end()) {
        return it->second.lock(); // Convert weak_ptr to shared_ptr
    }
    return nullptr;
}

void UICore::setTheme(std::shared_ptr<Theme> theme) {
    if (!theme) {
        throw std::invalid_argument("Theme cannot be null");
    }
    
    std::lock_guard<std::mutex> lock(themeMutex);
    currentTheme = theme;
}

std::shared_ptr<Theme> UICore::getTheme() const {
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
    // If elements mutex is already locked (we're in event processing), defer the focus change
    if (!elementsMutex.try_lock()) {
        std::lock_guard<std::mutex> focusLock(focusMutex);
        pendingFocusChanges.push(elementId);
        return;
    }
    
    // We got the lock, proceed normally
    std::lock_guard<std::mutex> lock(elementsMutex, std::adopt_lock);
    
    // Clear focus from current element
    if (!focusedElementId.empty()) {
        // Direct lookup without calling getElement to avoid recursive lock
        auto it = elementRegistry.find(focusedElementId);
        if (it != elementRegistry.end()) {
            auto currentFocused = it->second.lock();
            if (currentFocused) {
                currentFocused->hasFocus = false;
                currentFocused->onFocusLost();
            }
        }
    }
    
    // Set focus to new element
    // Direct lookup without calling getElement to avoid recursive lock
    auto it = elementRegistry.find(elementId);
    if (it != elementRegistry.end()) {
        auto newFocused = it->second.lock();
        if (newFocused && newFocused->isInteractive()) {
            focusedElementId = elementId;
            newFocused->hasFocus = true;
            newFocused->onFocusGained();
        } else {
            focusedElementId.clear();
        }
    } else {
        focusedElementId.clear();
    }
}

std::string UICore::getFocusedElementId() const {
    return focusedElementId;
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
            
            // Process pending focus changes after releasing the mutex
            std::vector<std::string> focusChanges;
            {
                std::lock_guard<std::mutex> focusLock(focusMutex);
                while (!pendingFocusChanges.empty()) {
                    focusChanges.push_back(pendingFocusChanges.front());
                    pendingFocusChanges.pop();
                }
            }
            
            // Process focus changes without holding any locks
            for (const auto& elementId : focusChanges) {
                setFocus(elementId);
            }
        }
        
        // Render
        try {
            SDL_SetRenderDrawColor(resources->getRenderer(), 0, 0, 0, 255);
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
