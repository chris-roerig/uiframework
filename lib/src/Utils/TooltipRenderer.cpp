#include "uiframework/Utils/TooltipRenderer.h"
#include "uiframework/Theme/ThemeBase.h"
#include "uiframework/Helpers.h"
#include <algorithm>

namespace ui {

void TooltipRenderer::renderTooltip(SDL_Renderer* renderer, TTF_Font* font, 
                                  std::shared_ptr<Theme> theme, const std::string& text,
                                  int mouseX, int mouseY, int screenWidth, int screenHeight) {
    if (!renderer || !font || !theme || text.empty()) return;

    // Get tooltip colors from theme
    auto tooltipColors = theme->tooltipColors();
    
    // Measure text dimensions
    int textWidth, textHeight;
    if (TTF_SizeText(font, text.c_str(), &textWidth, &textHeight) != 0) {
        return; // Failed to measure text
    }
    
    // Add padding around text
    const int padding = 8;
    int tooltipWidth = textWidth + 2 * padding;
    int tooltipHeight = textHeight + 2 * padding;
    
    // Calculate optimal position
    SDL_Point pos = calculateTooltipPosition(mouseX, mouseY, tooltipWidth, tooltipHeight,
                                           screenWidth, screenHeight);
    
    // Create tooltip rectangle
    SDL_Rect tooltipRect = { pos.x, pos.y, tooltipWidth, tooltipHeight };
    
    // Draw tooltip background
    drawFilledRect(renderer, tooltipRect, tooltipColors.tooltipBackground);
    
    // Draw tooltip border
    SDL_SetRenderDrawColor(renderer, tooltipColors.tooltipBorder.r, tooltipColors.tooltipBorder.g,
                          tooltipColors.tooltipBorder.b, tooltipColors.tooltipBorder.a);
    SDL_RenderDrawRect(renderer, &tooltipRect);
    
    // Render text
    SDL_Color textColor = { tooltipColors.tooltipText.r, tooltipColors.tooltipText.g, 
                           tooltipColors.tooltipText.b, tooltipColors.tooltipText.a };
    SDL_Surface* textSurface = TTF_RenderText_Blended(font, text.c_str(), textColor);
    if (textSurface) {
        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        if (textTexture) {
            SDL_Rect textRect = { 
                pos.x + padding, 
                pos.y + padding, 
                textWidth, 
                textHeight 
            };
            SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);
            SDL_DestroyTexture(textTexture);
        }
        SDL_FreeSurface(textSurface);
    }
}

bool TooltipRenderer::shouldShowTooltip(Uint32 hoverStartTime, Uint32 currentTime, Uint32 delayMs) {
    if (hoverStartTime == 0) return false;
    return (currentTime - hoverStartTime) >= delayMs;
}

SDL_Point TooltipRenderer::calculateTooltipPosition(int mouseX, int mouseY, 
                                                  int tooltipWidth, int tooltipHeight,
                                                  int screenWidth, int screenHeight) {
    SDL_Point pos;
    
    // Default position: below and to the right of mouse cursor
    const int offset = 10;
    pos.x = mouseX + offset;
    pos.y = mouseY + offset;
    
    // Adjust if tooltip would go off right edge
    if (pos.x + tooltipWidth > screenWidth) {
        pos.x = mouseX - tooltipWidth - offset;
    }
    
    // Adjust if tooltip would go off bottom edge
    if (pos.y + tooltipHeight > screenHeight) {
        pos.y = mouseY - tooltipHeight - offset;
    }
    
    // Ensure tooltip doesn't go off left or top edges
    pos.x = std::max(0, pos.x);
    pos.y = std::max(0, pos.y);
    
    return pos;
}

} // namespace ui
