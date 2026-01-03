#include "uiframework/UIElements/Button.h"
#include "uiframework/Theme/ThemeBase.h"
#include "uiframework/Helpers.h"
#include "uiframework/UICore.h"
#include "uiframework/ErrorHandling.h"
#include "uiframework/Constants.h"
#include "uiframework/Utils/TextUtils.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

namespace ui {

void Button::renderImpl(const RenderContext& ctx) {
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
        
        // Apply state-specific styling
        switch (currentState) {
            case ButtonState::Hover:
                // Lighten background for hover
                bg = Color(static_cast<uint8_t>(std::min(255, bg.r + 20)),
                          static_cast<uint8_t>(std::min(255, bg.g + 20)),
                          static_cast<uint8_t>(std::min(255, bg.b + 20)),
                          bg.a);
                break;
            case ButtonState::Pressed:
                // Darken if pressed
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
    
    // Draw the button content (icon and/or text)
    if (iconTexture || (!text.empty() && ctx.font)) {
        SDL_Rect contentRect = getContentRect();
        
        // Calculate layout for icon and text
        int iconSpace = iconTexture ? (iconWidth + 4) : 0; // 4px spacing between icon and text
        int totalContentWidth = iconSpace;
        
        // Calculate text width if needed
        int textWidth = 0;
        TextCacheEntry* textCache = nullptr;
        if (!text.empty() && ctx.font) {
            SDL_Color sdlColor = { textColor.r, textColor.g, textColor.b, textColor.a };
            textCache = getCachedText("main", text, sdlColor, ctx.renderer, ctx.font);
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

void Button::onMouseEnter() {
    if (enabled && currentState == ButtonState::Normal) {
        currentState = ButtonState::Hover;
    }
}

void Button::onMouseLeave() {
    if (enabled && currentState == ButtonState::Hover) {
        currentState = ButtonState::Normal;
    }
}

void Button::onMouseDown(int x, int y) {
    if (enabled) {
        currentState = ButtonState::Pressed;
    }
}

void Button::onMouseUp(int x, int y) {
    if (enabled && currentState == ButtonState::Pressed) {
        // Return to hover if mouse is still over button, normal otherwise
        currentState = getIsHovered() ? ButtonState::Hover : ButtonState::Normal;
    }
}

void Button::setIcon(const std::string& path) {
    // Clean up existing icon
    if (iconTexture) {
        SDL_DestroyTexture(iconTexture);
        iconTexture = nullptr;
    }
    
    iconPath = path;
    
    if (path.empty()) {
        return;
    }
    
    // Load icon texture (requires renderer from core)
    if (coreRef) {
        SDL_Surface* surface = IMG_Load(path.c_str());
        if (surface) {
            iconTexture = SDL_CreateTextureFromSurface(coreRef->getRenderer(), surface);
            if (iconTexture) {
                iconWidth = surface->w;
                iconHeight = surface->h;
            }
            SDL_FreeSurface(surface);
        }
    }
}

void Button::clearIcon() {
    if (iconTexture) {
        SDL_DestroyTexture(iconTexture);
        iconTexture = nullptr;
    }
    iconPath.clear();
    iconWidth = 0;
    iconHeight = 0;
}

void Button::activate() {
    if (onClick && coreRef) {
        // Queue the button callback to avoid deadlock
        coreRef->queueCallback(onClick);
    }
}

std::pair<int, int> Button::getPreferredSize(TTF_Font* font) const {
    if (!font || text.empty()) {
        return {width, height};
    }
    
    auto [textW, textH] = TextUtils::getTextSize(text, font);
    return {textW + 20, textH + 10}; // Add padding
}

std::pair<int, int> Button::getMinimumSize() const {
    return {40, 20}; // Minimum clickable size
}

void Button::autoSize(TTF_Font* font) {
    auto [prefW, prefH] = getPreferredSize(font);
    width = prefW;
    height = prefH;
}

} // namespace ui
