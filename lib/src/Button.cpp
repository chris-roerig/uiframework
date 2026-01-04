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
    // Update animation state
    updateAnimation(SDL_GetTicks());
    
    // Process real-time updates
    int textBufferIndex = currentTextBuffer.load();
    if (!textBuffers[textBufferIndex].empty()) {
        text = textBuffers[textBufferIndex];
        invalidateTextCache();
    }
    
    int enabledBufferIndex = currentEnabledBuffer.load();
    if (enabledBuffers[enabledBufferIndex] != enabled) {
        enabled = enabledBuffers[enabledBufferIndex];
    }
    
    // Update state based on enabled status
    if (!enabled && currentState != ButtonState::Disabled) {
        currentState = ButtonState::Disabled;
    } else if (enabled && currentState == ButtonState::Disabled) {
        currentState = ButtonState::Normal;
    }
    
    // Retrieve button colors from the theme
    auto colors = ctx.buttonColors();
    SDL_Rect rect = { x, y, width, height };

    // Determine background and text colors based on state with animation support
    Color bg, textColor;
    if (currentState == ButtonState::Disabled) {
        bg = colors.buttonBackground;
        textColor = colors.buttonTextDisabled;
    } else {
        // Base colors
        Color normalBg = colors.buttonBackground;
        Color hoverBg = Color(static_cast<uint8_t>(std::min(255, normalBg.r + 20)),
                             static_cast<uint8_t>(std::min(255, normalBg.g + 20)),
                             static_cast<uint8_t>(std::min(255, normalBg.b + 20)),
                             normalBg.a);
        Color pressedBg = Color(static_cast<uint8_t>(normalBg.r * Constants::BUTTON_PRESSED_DARKEN_FACTOR),
                               static_cast<uint8_t>(normalBg.g * Constants::BUTTON_PRESSED_DARKEN_FACTOR),
                               static_cast<uint8_t>(normalBg.b * Constants::BUTTON_PRESSED_DARKEN_FACTOR),
                               normalBg.a);
        
        textColor = colors.buttonText;
        
        // Apply animated color transitions
        if (animationsEnabled && isAnimating()) {
            float progress = getAnimationProgress();
            
            // Interpolate between previous and current state colors
            Color fromBg, toBg;
            
            // Determine source and target colors based on state transition
            switch (previousState) {
                case ButtonState::Normal:
                    fromBg = normalBg;
                    break;
                case ButtonState::Hover:
                    fromBg = hoverBg;
                    break;
                case ButtonState::Pressed:
                    fromBg = pressedBg;
                    break;
                default:
                    fromBg = normalBg;
                    break;
            }
            
            switch (currentState) {
                case ButtonState::Normal:
                    toBg = normalBg;
                    break;
                case ButtonState::Hover:
                    toBg = hoverBg;
                    break;
                case ButtonState::Pressed:
                    toBg = pressedBg;
                    break;
                default:
                    toBg = normalBg;
                    break;
            }
            
            // Linear interpolation between colors
            bg = Color(
                static_cast<uint8_t>(fromBg.r + (toBg.r - fromBg.r) * progress),
                static_cast<uint8_t>(fromBg.g + (toBg.g - fromBg.g) * progress),
                static_cast<uint8_t>(fromBg.b + (toBg.b - fromBg.b) * progress),
                static_cast<uint8_t>(fromBg.a + (toBg.a - fromBg.a) * progress)
            );
        } else {
            // No animation, use direct state colors
            switch (currentState) {
                case ButtonState::Hover:
                    bg = hoverBg;
                    break;
                case ButtonState::Pressed:
                    bg = pressedBg;
                    break;
                default:
                    bg = normalBg;
                    break;
            }
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
        previousState = currentState;
        currentState = ButtonState::Hover;
        
        // Start hover animation if enabled
        if (animationsEnabled) {
            startAnimation(hoverAnimationDuration);
        }
    }
}

void Button::onMouseLeave() {
    if (enabled && currentState == ButtonState::Hover) {
        previousState = currentState;
        currentState = ButtonState::Normal;
        
        // Start reverse hover animation if enabled
        if (animationsEnabled) {
            startAnimation(hoverAnimationDuration);
        }
    }
}

void Button::onMouseDown(int x, int y) {
    if (enabled) {
        previousState = currentState;
        currentState = ButtonState::Pressed;
        
        // Start press animation if enabled
        if (animationsEnabled) {
            startAnimation(pressAnimationDuration);
        }
    }
}

void Button::onMouseUp(int x, int y) {
    if (enabled && currentState == ButtonState::Pressed) {
        previousState = currentState;
        // Return to hover if mouse is still over button, normal otherwise
        currentState = getIsHovered() ? ButtonState::Hover : ButtonState::Normal;
        
        // Start release animation if enabled
        if (animationsEnabled) {
            Uint32 duration = getIsHovered() ? hoverAnimationDuration : hoverAnimationDuration;
            startAnimation(duration);
        }
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

// Real-time safe methods (lock-free, audio thread safe)
void Button::realtimeSetText(const std::string& newText) {
    int nextBuffer = 1 - currentTextBuffer.load();
    textBuffers[nextBuffer] = newText;
    currentTextBuffer.store(nextBuffer);
}

void Button::realtimeSetEnabled(bool enabled) {
    int nextBuffer = 1 - currentEnabledBuffer.load();
    enabledBuffers[nextBuffer] = enabled;
    currentEnabledBuffer.store(nextBuffer);
}

} // namespace ui
