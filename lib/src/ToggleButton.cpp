#include "uiframework/UIElements/ToggleButton.h"
#include "uiframework/Theme/ThemeBase.h"
#include "uiframework/Helpers.h"
#include "uiframework/UICore.h"
#include "uiframework/Constants.h"

namespace ui {

void ToggleButton::activate() {
    if (!enabled) return;
    
    // Toggle the state
    isToggled = !isToggled;
    
    // Call the toggle callback with the new state
    if (onToggle && coreRef) {
        coreRef->queueCallback([this]() {
            onToggle(isToggled);
        });
    }
}

void ToggleButton::renderImpl(const RenderContext& ctx) {
    // Update state based on enabled status
    if (!enabled && currentState != ButtonState::Disabled) {
        currentState = ButtonState::Disabled;
    } else if (enabled && currentState == ButtonState::Disabled) {
        currentState = ButtonState::Normal;
    }
    
    // Retrieve button colors from the theme
    auto colors = ctx.buttonColors();
    SDL_Rect rect = { x, y, width, height };
    SDL_Rect contentRect = getContentRect();

    // Determine background and text colors based on state
    Color bg, textColor;
    if (currentState == ButtonState::Disabled) {
        bg = colors.buttonDisabled;
        textColor = colors.buttonTextDisabled;
    } else {
        bg = colors.buttonBackground;
        textColor = colors.buttonText;
        
        // Apply toggle state - toggled buttons appear pressed
        if (isToggled) {
            bg = Color(static_cast<uint8_t>(bg.r * Constants::BUTTON_PRESSED_DARKEN_FACTOR),
                      static_cast<uint8_t>(bg.g * Constants::BUTTON_PRESSED_DARKEN_FACTOR),
                      static_cast<uint8_t>(bg.b * Constants::BUTTON_PRESSED_DARKEN_FACTOR),
                      bg.a);
        }
        
        // Apply state-specific styling on top of toggle state
        switch (currentState) {
            case ButtonState::Hover:
                // Lighten background for hover (works on both normal and toggled)
                bg = Color(static_cast<uint8_t>(std::min(255, bg.r + 20)),
                          static_cast<uint8_t>(std::min(255, bg.g + 20)),
                          static_cast<uint8_t>(std::min(255, bg.b + 20)),
                          bg.a);
                break;
            case ButtonState::Pressed:
                // Additional darkening when pressed (on top of toggle state)
                bg = Color(static_cast<uint8_t>(bg.r * Constants::BUTTON_PRESSED_DARKEN_FACTOR),
                          static_cast<uint8_t>(bg.g * Constants::BUTTON_PRESSED_DARKEN_FACTOR),
                          static_cast<uint8_t>(bg.b * Constants::BUTTON_PRESSED_DARKEN_FACTOR),
                          bg.a);
                break;
            default:
                break;
        }
    }
    
    // Draw button background
    drawFilledRect(ctx.renderer, rect, bg);
    
    // Draw enhanced focus border when focused and enabled
    if (hasFocus && enabled) {
        SDL_Rect focusRect = getFocusRect();
        // Use theme focus color for better visibility
        auto focusColors = ctx.focusColors();
        SDL_SetRenderDrawColor(ctx.renderer, focusColors.focusBorder.r, 
                              focusColors.focusBorder.g, focusColors.focusBorder.b, 
                              focusColors.focusBorder.a);
        SDL_RenderDrawRect(ctx.renderer, &focusRect);
        
        // Add inner focus border for better visibility
        SDL_Rect innerFocus = { focusRect.x + 1, focusRect.y + 1, 
                               focusRect.w - 2, focusRect.h - 2 };
        SDL_RenderDrawRect(ctx.renderer, &innerFocus);
    }
    
    // Draw the button content (icon and/or text) - same as Button
    if (iconTexture || (!getTextInternal().empty() && ctx.font)) {
        SDL_Rect contentRect = getContentRect();
        
        // Calculate layout for icon and text
        int iconSpace = iconTexture ? (iconWidth + 4) : 0; // 4px spacing between icon and text
        int totalContentWidth = iconSpace;
        
        // Calculate text width if needed
        int textWidth = 0;
        TextCacheEntry* textCache = nullptr;
        if (!getTextInternal().empty() && ctx.font) {
            SDL_Color sdlColor = { textColor.r, textColor.g, textColor.b, textColor.a };
            textCache = getCachedText("main", getTextInternal(), sdlColor, ctx.renderer, ctx.font);
            if (textCache && textCache->texture) {
                textWidth = textCache->width;
                totalContentWidth += textWidth;
            }
        }
        
        // Center the combined content
        int startX = contentRect.x + (contentRect.w - totalContentWidth) / 2;
        int centerY = contentRect.y + contentRect.h / 2;
        
        // Draw icon if present
        if (iconTexture) {
            SDL_Rect iconDst = {
                startX,
                centerY - iconHeight / 2,
                iconWidth,
                iconHeight
            };
            SDL_RenderCopy(ctx.renderer, iconTexture, nullptr, &iconDst);
            startX += iconWidth + 4; // Move past icon + spacing
        }
        
        // Draw text if present
        if (textCache && textCache->texture) {
            SDL_Rect textDst = {
                startX,
                centerY - textCache->height / 2,
                textCache->width,
                textCache->height
            };
            SDL_RenderCopy(ctx.renderer, textCache->texture, nullptr, &textDst);
        }
    }
}

} // namespace ui
