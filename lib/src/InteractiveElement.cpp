#include "uiframework/UIElements/InteractiveElement.h"
#include "uiframework/UICore.h"

namespace ui {

InteractiveElement::InteractiveElement(int x, int y, int w, int h)
    : UIElement(x, y, w, h) {
}

void InteractiveElement::handleEvent(const SDL_Event& e) {
    // Early return for invisible or disabled elements
    if (!visible || !enabled) return;

    switch (e.type) {
        case SDL_MOUSEBUTTONDOWN:
            if (e.button.button == SDL_BUTTON_LEFT) {
                int mouseX = e.button.x;
                int mouseY = e.button.y;
                if (containsPoint(mouseX, mouseY)) {
                    isPressed = true;
                    // Auto-focus on click for consistency
                    if (coreRef) {
                        coreRef->setFocus(elementId);
                    }
                    onMouseDown(mouseX, mouseY);
                }
            }
            break;

        case SDL_MOUSEBUTTONUP:
            if (e.button.button == SDL_BUTTON_LEFT) {
                int mouseX = e.button.x;
                int mouseY = e.button.y;
                bool wasPressed = isPressed;
                isPressed = false;
                isDragging = false;
                
                // Call mouse up handler
                onMouseUp(mouseX, mouseY);
                
                // Activate if released over element
                if (wasPressed && containsPoint(mouseX, mouseY)) {
                    activate();
                }
            }
            break;

        case SDL_MOUSEMOTION: {
            int mouseX = e.motion.x;
            int mouseY = e.motion.y;
            bool nowContains = containsPoint(mouseX, mouseY);
            
            // Handle hover state changes
            if (nowContains && !isHovered) {
                isHovered = true;
                onMouseEnter();
            } else if (!nowContains && isHovered) {
                isHovered = false;
                onMouseLeave();
            }
            
            // Handle dragging
            if (isPressed) {
                if (!isDragging) {
                    isDragging = true;
                }
                onMouseDrag(mouseX, mouseY);
            }
            break;
        }

        case SDL_KEYDOWN:
            if (hasFocus) {
                SDL_Keycode key = e.key.keysym.sym;
                
                // Common activation keys
                if (key == SDLK_SPACE || key == SDLK_RETURN) {
                    activate();
                } else {
                    onKeyDown(key);
                }
            }
            break;

        case SDL_TEXTINPUT:
            if (hasFocus) {
                onTextInput(e.text.text);
            }
            break;
    }
}

} // namespace ui
