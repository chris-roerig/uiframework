#include "uiframework/UIElements/CheckBox.h"
#include "uiframework/Theme/ThemeBase.h"
#include "uiframework/Helpers.h"
#include "uiframework/UICore.h"
#include "uiframework/Constants.h"
#include "uiframework/ErrorHandling.h"
#include "uiframework/Utils/BorderRenderer.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>

namespace ui {

void CheckBox::renderImpl(const RenderContext& ctx) {
    SDL_Rect rect = { x, y, width, height };
    ThemeableElementColors tc = ctx.checkboxColors();
    
    // Determine colors based on enabled state
    Color bgColor = enabled ? tc.checkboxEnabled : tc.checkboxDisabled;
    Color checkColor = enabled ? tc.checkboxChecked : tc.checkboxDisabled;
    
    // Draw checkbox background
    drawFilledRect(ctx.renderer, rect, bgColor);
    
    // Draw 3D border for the checkbox
    ui::BorderRenderer::draw3DBorder(ctx.renderer, rect, tc.checkboxBorderLight, tc.checkboxBorderDark);
    
    // Draw focus indicator (only when enabled)
    if (hasFocus && enabled) {
        SDL_Rect focusRect = getFocusRect();
        SDL_SetRenderDrawColor(ctx.renderer, checkColor.r, checkColor.g, checkColor.b, checkColor.a);
        SDL_RenderDrawRect(ctx.renderer, &focusRect);
    }
    
    // If checked, draw a filled square
    if (checked) {
        SDL_Rect checkRect = {
            x + ui::Constants::BORDER_OFFSET + 2,
            y + ui::Constants::BORDER_OFFSET + 2,
            width - 2 * (ui::Constants::BORDER_OFFSET + 2),
            height - 2 * (ui::Constants::BORDER_OFFSET + 2)
        };
        drawFilledRect(ctx.renderer, checkRect, checkColor);
    }
}

void CheckBox::activate() {
    setChecked(!checked);
}

void CheckBox::setChecked(bool state) {
    if (checked != state) {
        checked = state;
        if (onToggle && coreRef) {
            bool currentState = checked;
            coreRef->queueCallback([this, currentState]() {
                if (onToggle) {
                    try {
                        onToggle(currentState);
                    } catch (const std::exception& e) {
                        std::cerr << "Error in checkbox callback: " << e.what() << std::endl;
                    }
                }
            });
        }
    }
}

std::pair<int, int> CheckBox::getPreferredSize(TTF_Font* font) const {
    return {20, 20}; // Fixed checkbox size
}

std::pair<int, int> CheckBox::getMinimumSize() const {
    return {20, 20}; // Same as preferred - fixed size
}

} // namespace ui
