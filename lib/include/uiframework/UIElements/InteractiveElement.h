#pragma once

#include "UIElement.h"
#include <SDL2/SDL.h>

namespace ui {

/**
 * Base class for interactive UI elements that handle mouse and keyboard events.
 * Provides common event handling patterns and state management.
 */
class InteractiveElement : public UIElement {
protected:
    bool isDragging = false;
    bool isHovered = false;
    bool isPressed = false;

    // Virtual event handlers for derived classes to override
    virtual void onMouseDown(int x, int y) {}
    virtual void onMouseUp(int x, int y) {}
    virtual void onMouseDrag(int x, int y) {}
    virtual void onMouseEnter() {}
    virtual void onMouseLeave() {}
    virtual void onKeyDown(const SDL_Keycode& key) {}
    virtual void onTextInput(const std::string& text) {}

public:
    InteractiveElement(int x, int y, int w, int h);
    virtual ~InteractiveElement() = default;

    // Final implementation of event handling with common patterns
    void handleEvent(const SDL_Event& e) override final;
    
    // Interactive elements can receive focus
    bool isInteractive() const override { return true; }

    // Helper method for activation (space/enter key or mouse click)
    virtual void activate() override {}

    // State accessors
    bool getIsDragging() const { return isDragging; }
    bool getIsHovered() const { return isHovered; }
    bool getIsPressed() const { return isPressed; }
};

} // namespace ui
