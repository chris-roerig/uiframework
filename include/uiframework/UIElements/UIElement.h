#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <vector>
#include <functional>
#include <memory>
#include <unordered_map>
#include "../Constants.h"

namespace ui {

// Forward declaration
class UICore;

struct TextCacheEntry {
    SDL_Texture* texture = nullptr;
    int width = 0;
    int height = 0;
    std::string text;
    SDL_Color color;
    
    ~TextCacheEntry() {
        if (texture) {
            SDL_DestroyTexture(texture);
        }
    }
};

// Base class for all UI elements with proper resource management
class UIElement {
protected:
    std::string elementId;
    UICore* coreRef = nullptr; // Non-owning reference to core
    mutable std::unordered_map<std::string, std::unique_ptr<TextCacheEntry>> textCache;
    
    // Helper method for cached text rendering
    TextCacheEntry* getCachedText(const std::string& key, const std::string& text, 
                                  SDL_Color color, SDL_Renderer* renderer, TTF_Font* font) const;
    void invalidateTextCache() const;
    
public:
    int x, y, width, height;
    bool hasFocus = false;
    bool visible = true;
    
    UIElement(int x_, int y_, int w_, int h_)
      : x(x_), y(y_), width(w_), height(h_) {}
    
    virtual ~UIElement() = default;
    
    // Non-copyable due to unique_ptr in textCache
    UIElement(const UIElement&) = delete;
    UIElement& operator=(const UIElement&) = delete;
    
    // Movable
    UIElement(UIElement&&) = default;
    UIElement& operator=(UIElement&&) = default;
    
    // Core interface methods
    virtual void render(SDL_Renderer* renderer, TTF_Font* font, std::shared_ptr<class Theme> theme) = 0;
    virtual void handleEvent(const SDL_Event &e) {}
    virtual bool isInteractive() const { return false; }
    virtual SDL_Rect getFocusRect() const { 
        return SDL_Rect{ x - Constants::FOCUS_BORDER_WIDTH, y - Constants::FOCUS_BORDER_WIDTH, 
                        width + 2 * Constants::FOCUS_BORDER_WIDTH, height + 2 * Constants::FOCUS_BORDER_WIDTH }; 
    }
    virtual void activate() { /* default does nothing */ }
    
    // Focus event callbacks
    virtual void onFocusGained() { /* default does nothing */ }
    virtual void onFocusLost() { /* default does nothing */ }
    
    // Element management
    const std::string& getId() const { return elementId; }
    void setId(const std::string& id) { elementId = id; }
    void setCoreReference(UICore* core) { coreRef = core; }
    
    // Utility methods
    bool containsPoint(int px, int py) const {
        return px >= x && px < x + width && py >= y && py < y + height;
    }
    
    void setPosition(int newX, int newY) {
        x = newX;
        y = newY;
    }
    
    void setSize(int newWidth, int newHeight) {
        width = newWidth;
        height = newHeight;
    }
    
    void setVisible(bool isVisible) {
        visible = isVisible;
    }
    
    bool isVisible() const {
        return visible;
    }
};

} // namespace ui
