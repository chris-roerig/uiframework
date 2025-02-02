#include "ContextMenu.h"
#include "ThemeGlobals.h"   // Provides: extern std::shared_ptr<Theme> g_currentTheme;
#include "ThemeBase.h"      // For ThemeableElementColors
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>

namespace ui {

// We assume the following helper functions and variable are declared in UICore.h (and defined in UICore.cpp):
extern void initFont();
extern void drawFilledRect(SDL_Renderer* renderer, const SDL_Rect &rect, const Color &color);
extern TTF_Font* globalFont;

static const int subItemHeight = 30; // Fixed height for each sub-menu item.
static const int hPadding = 10;      // Horizontal padding for top-level items.
static const int subHPadding = 10;   // Horizontal padding for sub-menu items.

ContextMenu::ContextMenu(int x_, int y_, int totalWidth_, int itemHeight_)
    : UIElement(x_, y_, totalWidth_, itemHeight_), activeItemIndex(0),
      subMenuSelectedIndex(0), expanded(false), totalWidth(totalWidth_), itemHeight(itemHeight_) {}

void ContextMenu::setItems(const std::vector<MenuItem>& newItems) {
    items = newItems;
    activeItemIndex = 0;
    subMenuSelectedIndex = 0;
}

void ContextMenu::render(SDL_Renderer* renderer) {
    initFont();
    // Determine the widths for top-level items based on their text.
    int numItems = items.size();
    if (numItems == 0)
        return;
    
    std::vector<SDL_Rect> itemRects;
    int currentX = x;
    const int hPadding = 10;
    for (int i = 0; i < numItems; i++) {
        int textW = 0, textH = 0;
        TTF_SizeText(globalFont, items[i].label.c_str(), &textW, &textH);
        int itemW = textW + 2 * hPadding;
        SDL_Rect rect = { currentX, y, itemW, itemHeight };
        itemRects.push_back(rect);
        currentX += itemW;
    }
    
    // Draw the full-width menu bar background using the context menu background color.
    SDL_Rect barRect = { 0, y, totalWidth, itemHeight };
    Color cmBg = g_currentTheme->contextMenuColors().contextMenuBackground;
    drawFilledRect(renderer, barRect, cmBg);
       
    // Render each top-level item.
    for (int i = 0; i < numItems; i++) {
        SDL_Rect itemRect = itemRects[i];
        // When the context menu is active (has focus), highlight the active item.
        if (this->hasFocus && i == activeItemIndex)
            drawFilledRect(renderer, itemRect, g_currentTheme->contextMenuColors().contextMenuHighlight);
        else
            drawFilledRect(renderer, itemRect, g_currentTheme->contextMenuColors().contextMenuBackground);
                    
        // Render the top-level label centered.
        if (globalFont) {
            SDL_Color textColor = { g_currentTheme->contextMenuColors().contextMenuText.r,
                                      g_currentTheme->contextMenuColors().contextMenuText.g,
                                      g_currentTheme->contextMenuColors().contextMenuText.b,
                                      g_currentTheme->contextMenuColors().contextMenuText.a };
            SDL_Surface* surface = TTF_RenderText_Solid(globalFont, items[i].label.c_str(), textColor);
            if (surface) {
                SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
                int textW = surface->w, textH = surface->h;
                SDL_FreeSurface(surface);
                SDL_Rect dst = { itemRect.x + (itemRect.w - textW) / 2,
                                 itemRect.y + (itemRect.h - textH) / 2,
                                 textW, textH };
                SDL_RenderCopy(renderer, texture, nullptr, &dst);
                SDL_DestroyTexture(texture);
            }
        }
    }

    // Draw a bottom border using the theme's context menu border color.
    Color cmBorder = g_currentTheme->contextMenuColors().contextMenuBorder;
    SDL_SetRenderDrawColor(renderer, cmBorder.r, cmBorder.g, cmBorder.b, cmBorder.a);
    SDL_RenderDrawLine(renderer, 0, y + itemHeight - 1, totalWidth, y + itemHeight - 1);
 

    // (Submenu rendering remains unchanged.)
    if (expanded && activeItemIndex < numItems) {
        const MenuItem &activeItem = items[activeItemIndex];
        int parentWidth = itemRects[activeItemIndex].w;
        
        // Determine maximum sub-item text width.
        int maxSubTextW = 0;
        int tempH = 0;
        for (const auto &subText : activeItem.subItemLabels) {
            int w = 0;
            TTF_SizeText(globalFont, subText.c_str(), &w, &tempH);
            if (w > maxSubTextW)
                maxSubTextW = w;
        }
        const int subHPadding = 10;
        int subMenuWidth = maxSubTextW + 2 * subHPadding;
        if (subMenuWidth < parentWidth)
            subMenuWidth = parentWidth;
        
        int subMenuX = itemRects[activeItemIndex].x;
        int subMenuY = y + itemHeight;
        int subMenuHeight = subItemHeight * activeItem.subItemLabels.size();
        SDL_Rect subRect = { subMenuX, subMenuY, subMenuWidth, subMenuHeight };
        drawFilledRect(renderer, subRect, g_currentTheme->optionSelectColors().selectOptionUnselected);
        
        ThemeableElementColors tc = g_currentTheme->optionSelectColors();
        for (size_t i = 0; i < activeItem.subItemLabels.size(); i++) {
            SDL_Rect cellRect = { subMenuX, subMenuY + static_cast<int>(i) * subItemHeight, subMenuWidth, subItemHeight };
            if ((int)i == subMenuSelectedIndex)
                drawFilledRect(renderer, cellRect, tc.selectOptionSelected);
            else
                drawFilledRect(renderer, cellRect, tc.selectOptionUnselected);
            
            if (globalFont) {
                SDL_Color textColor = { 0, 0, 0, 255 };
                SDL_Surface* surface = TTF_RenderText_Solid(globalFont, activeItem.subItemLabels[i].c_str(), textColor);
                if (surface) {
                    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
                    int textW = surface->w, textH = surface->h;
                    SDL_FreeSurface(surface);
                    SDL_Rect dst = { cellRect.x + subHPadding,
                                     cellRect.y + (cellRect.h - textH) / 2,
                                     textW, textH };
                    SDL_RenderCopy(renderer, texture, nullptr, &dst);
                    SDL_DestroyTexture(texture);
                }
            }
        }
    }
}

void ContextMenu::handleEvent(const SDL_Event &e) {
    if (e.type == SDL_KEYDOWN) {
        if (!expanded) {
            // Not expanded: Navigate top-level items.
            if (e.key.keysym.sym == SDLK_LEFT) {
                activeItemIndex = (activeItemIndex - 1 + items.size()) % items.size();
            } else if (e.key.keysym.sym == SDLK_RIGHT) {
                activeItemIndex = (activeItemIndex + 1) % items.size();
            } else if (e.key.keysym.sym == SDLK_RETURN) {
                // Expand submenu if available.
                if (!items[activeItemIndex].subItemLabels.empty()) {
                    expanded = true;
                    subMenuSelectedIndex = 0;
                }
            }
        } else {
            // Expanded state.
            if (e.key.keysym.sym == SDLK_UP) {
                subMenuSelectedIndex = (subMenuSelectedIndex - 1 + items[activeItemIndex].subItemLabels.size())
                                       % items[activeItemIndex].subItemLabels.size();
            } else if (e.key.keysym.sym == SDLK_DOWN) {
                subMenuSelectedIndex = (subMenuSelectedIndex + 1) % items[activeItemIndex].subItemLabels.size();
            }
            // New behavior: left/right arrow collapse the submenu and switch active top-level item.
            else if (e.key.keysym.sym == SDLK_LEFT) {
                expanded = false;
                activeItemIndex = (activeItemIndex - 1 + items.size()) % items.size();
            } else if (e.key.keysym.sym == SDLK_RIGHT) {
                expanded = false;
                activeItemIndex = (activeItemIndex + 1) % items.size();
            } else if (e.key.keysym.sym == SDLK_RETURN) {
                // Execute the callback for the selected sub-item.
                if (subMenuSelectedIndex < items[activeItemIndex].subCallbacks.size()) {
                    auto cb = items[activeItemIndex].subCallbacks[subMenuSelectedIndex];
                    if (cb)
                        cb();
                }
                expanded = false;
            } else if (e.key.keysym.sym == SDLK_ESCAPE) {
                expanded = false;
            }
        }
    }
}


SDL_Rect ContextMenu::getFocusRect() const {
    // We do not draw an overall focus border.
    return SDL_Rect{0,0,0,0};
}

} // namespace ui
