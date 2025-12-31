#include "uiframework/UIElements/ContextMenu.h"
#include "uiframework/Theme/ThemeBase.h"
#include "uiframework/Helpers.h"
#include "uiframework/UICore.h"
#include "uiframework/Constants.h"
#include "uiframework/ErrorHandling.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <algorithm>

namespace ui {

ContextMenu::ContextMenu(int x_, int y_, int w_, int h_, const std::vector<TopMenuItem>& menus)
    : UIElement(x_, y_, w_, h_) {
    setMenuItems(menus);
}

void ContextMenu::renderMenuBar(SDL_Renderer* renderer, TTF_Font* font, std::shared_ptr<Theme> theme) {
    ThemeableElementColors tc = theme->contextMenuColors();
    
    // Draw menu bar background
    SDL_Rect menuBarRect = { x, y, width, height };
    drawFilledRect(renderer, menuBarRect, tc.contextMenuBackground);
    
    // Draw menu bar border
    SDL_SetRenderDrawColor(renderer, tc.contextMenuBorder.r, tc.contextMenuBorder.g, 
                          tc.contextMenuBorder.b, tc.contextMenuBorder.a);
    SDL_RenderDrawRect(renderer, &menuBarRect);
}

void ContextMenu::renderMenuItems(SDL_Renderer* renderer, TTF_Font* font, std::shared_ptr<Theme> theme) {
    ThemeableElementColors tc = theme->contextMenuColors();
    
    // Calculate item widths if needed
    if (font) {
        calculateItemWidths(font);
    }
    
    // Draw top-level menu items
    int currentX = x + ui::Constants::MENU_PADDING; // Start with some padding
    for (int i = 0; i < static_cast<int>(items.size()); i++) {
        const auto& item = items[i];
        SDL_Rect itemRect = { currentX, y, item.width, height };
        
        // Highlight active or hovered item
        if (i == activeItemIndex || i == hoveredItemIndex) {
            drawFilledRect(renderer, itemRect, tc.contextMenuActiveItem);
        }
        
        // Draw item text
        if (font && !item.label.empty()) {
            SDL_Color textColor = { tc.contextMenuText.r, tc.contextMenuText.g, 
                                  tc.contextMenuText.b, tc.contextMenuText.a };
            SDL_Surface* surface = TTF_RenderText_Solid(font, item.label.c_str(), textColor);
            if (surface) {
                SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
                if (texture) {
                    SDL_Rect textRect = { 
                        currentX + (item.width - surface->w) / 2,
                        y + (height - surface->h) / 2,
                        surface->w, 
                        surface->h 
                    };
                    SDL_RenderCopy(renderer, texture, nullptr, &textRect);
                    SDL_DestroyTexture(texture);
                }
                SDL_FreeSurface(surface);
            }
        }
        
        currentX += item.width;
    }
}

void ContextMenu::renderSubmenuItems(SDL_Renderer* renderer, TTF_Font* font, std::shared_ptr<Theme> theme, 
                                    const MenuItem& activeItem, const SDL_Rect& subMenuRect) {
    ThemeableElementColors tc = theme->contextMenuColors();
    
    // Draw submenu items
    for (int i = 0; i < static_cast<int>(activeItem.subItemLabels.size()); i++) {
        SDL_Rect subItemRect = { subMenuRect.x, subMenuRect.y + i * ui::Constants::SUBMENU_ITEM_HEIGHT, subMenuRect.w, ui::Constants::SUBMENU_ITEM_HEIGHT };
        
        // Highlight selected or hovered submenu item
        if (i == subMenuSelectedIndex || i == hoveredSubIndex) {
            drawFilledRect(renderer, subItemRect, tc.contextMenuActiveItem);
        }
        
        // Draw submenu item text
        if (font && !activeItem.subItemLabels[i].empty()) {
            SDL_Color textColor = { tc.contextMenuText.r, tc.contextMenuText.g, 
                                  tc.contextMenuText.b, tc.contextMenuText.a };
            SDL_Surface* surface = TTF_RenderText_Solid(font, activeItem.subItemLabels[i].c_str(), textColor);
            if (surface) {
                SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
                if (texture) {
                    SDL_Rect textRect = { 
                        subMenuRect.x + ui::Constants::SUBMENU_TEXT_PADDING,
                        subMenuRect.y + i * ui::Constants::SUBMENU_ITEM_HEIGHT + (ui::Constants::SUBMENU_ITEM_HEIGHT - surface->h) / 2,
                        surface->w, 
                        surface->h 
                    };
                    SDL_RenderCopy(renderer, texture, nullptr, &textRect);
                    SDL_DestroyTexture(texture);
                }
                SDL_FreeSurface(surface);
            }
        }
        
        // Draw separator line
        if (i < static_cast<int>(activeItem.subItemLabels.size()) - 1) {
            SDL_SetRenderDrawColor(renderer, tc.contextMenuBorder.r, tc.contextMenuBorder.g, 
                                  tc.contextMenuBorder.b, tc.contextMenuBorder.a);
            SDL_RenderDrawLine(renderer, subMenuRect.x, subMenuRect.y + (i + 1) * ui::Constants::SUBMENU_ITEM_HEIGHT, 
                             subMenuRect.x + subMenuRect.w, subMenuRect.y + (i + 1) * ui::Constants::SUBMENU_ITEM_HEIGHT);
        }
    }
}

void ContextMenu::renderSubmenu(SDL_Renderer* renderer, TTF_Font* font, std::shared_ptr<Theme> theme) {
    if (!expanded || activeItemIndex < 0 || activeItemIndex >= static_cast<int>(items.size())) {
        return;
    }
    
    const auto& activeItem = items[activeItemIndex];
    if (activeItem.subItemLabels.empty()) {
        return;
    }
    
    ThemeableElementColors tc = theme->contextMenuColors();
    
    // Calculate submenu position
    SDL_Rect activeItemRect = getItemRect(activeItemIndex);
    int subMenuY = y + height;
    int subMenuWidth = ui::Constants::SUBMENU_DEFAULT_WIDTH; // Default width
    
    // Calculate actual submenu width based on text
    if (font) {
        int maxWidth = ui::Constants::SUBMENU_MIN_WIDTH;
        for (const auto& subLabel : activeItem.subItemLabels) {
            int textWidth = 0;
            TTF_SizeText(font, subLabel.c_str(), &textWidth, nullptr);
            maxWidth = std::max(maxWidth, textWidth + ui::Constants::MENU_ITEM_PADDING);
        }
        subMenuWidth = maxWidth;
    }
    
    int subMenuHeight = static_cast<int>(activeItem.subItemLabels.size()) * ui::Constants::SUBMENU_ITEM_HEIGHT;
    SDL_Rect subMenuRect = { activeItemRect.x, subMenuY, subMenuWidth, subMenuHeight };
    
    // Draw submenu background
    drawFilledRect(renderer, subMenuRect, tc.contextMenuBackground);
    
    // Draw submenu border
    SDL_SetRenderDrawColor(renderer, tc.contextMenuBorder.r, tc.contextMenuBorder.g, 
                          tc.contextMenuBorder.b, tc.contextMenuBorder.a);
    SDL_RenderDrawRect(renderer, &subMenuRect);
    
    // Draw submenu items
    renderSubmenuItems(renderer, font, theme, activeItem, subMenuRect);
}

void ContextMenu::renderImpl(const RenderContext& ctx) {
    // Render menu bar background and border
    renderMenuBar(ctx.renderer, ctx.font, ctx.theme);
    
    // Render top-level menu items
    renderMenuItems(ctx.renderer, ctx.font, ctx.theme);
    
    // Render submenu if expanded
    renderSubmenu(ctx.renderer, ctx.font, ctx.theme);
    
    // Draw focus indicator
    if (hasFocus) {
        ThemeableElementColors tc = theme->contextMenuColors();
        SDL_Rect focusRect = getFocusRect();
        SDL_SetRenderDrawColor(renderer, tc.contextMenuActiveItem.r, tc.contextMenuActiveItem.g, 
                              tc.contextMenuActiveItem.b, tc.contextMenuActiveItem.a);
        SDL_RenderDrawRect(renderer, &focusRect);
    }
}

void ContextMenu::handleEvent(const SDL_Event &e) {
    if (!visible) return;
    
    if (e.type == SDL_MOUSEBUTTONDOWN) {
        if (e.button.button == SDL_BUTTON_LEFT) {
            int mouseX = e.button.x;
            int mouseY = e.button.y;
            
            if (expanded) {
                // Check if clicking on submenu
                int subItem = getSubItemAt(mouseX, mouseY);
                if (subItem >= 0) {
                    selectSubItem(subItem);
                    activate();
                    return;
                }
            }
            
            // Check if clicking on top-level menu
            int topItem = getItemAt(mouseX, mouseY);
            if (topItem >= 0) {
                if (topItem == activeItemIndex && expanded) {
                    collapseMenu();
                } else {
                    setActiveItem(topItem);
                    expandMenu();
                }
            } else {
                collapseMenu();
            }
        }
    } else if (e.type == SDL_MOUSEMOTION) {
        int mouseX = e.motion.x;
        int mouseY = e.motion.y;
        
        if (expanded) {
            hoveredSubIndex = getSubItemAt(mouseX, mouseY);
        }
        
        int newHoveredItem = getItemAt(mouseX, mouseY);
        if (newHoveredItem != hoveredItemIndex) {
            hoveredItemIndex = newHoveredItem;
            if (expanded && hoveredItemIndex >= 0) {
                setActiveItem(hoveredItemIndex);
            }
        }
    } else if (e.type == SDL_KEYDOWN && hasFocus) {
        switch (e.key.keysym.sym) {
            case SDLK_ESCAPE:
                collapseMenu();
                break;
            case SDLK_RETURN:
                if (expanded) {
                    activate();
                } else if (activeItemIndex >= 0) {
                    expandMenu();
                }
                break;
            case SDLK_LEFT:
                if (activeItemIndex > 0) {
                    setActiveItem(activeItemIndex - 1);
                    if (expanded) {
                        expandMenu();
                    }
                }
                break;
            case SDLK_RIGHT:
                if (activeItemIndex < static_cast<int>(items.size()) - 1) {
                    setActiveItem(activeItemIndex + 1);
                    if (expanded) {
                        expandMenu();
                    }
                }
                break;
            case SDLK_UP:
                if (expanded && subMenuSelectedIndex > 0) {
                    subMenuSelectedIndex--;
                }
                break;
            case SDLK_DOWN:
                if (expanded) {
                    if (activeItemIndex >= 0 && activeItemIndex < static_cast<int>(items.size())) {
                        int maxSub = static_cast<int>(items[activeItemIndex].subItemLabels.size()) - 1;
                        if (subMenuSelectedIndex < maxSub) {
                            subMenuSelectedIndex++;
                        }
                    }
                } else if (activeItemIndex >= 0) {
                    expandMenu();
                }
                break;
        }
    }
}

SDL_Rect ContextMenu::getFocusRect() const {
    return SDL_Rect{ x - 2, y - 2, width + 4, height + 4 };
}

void ContextMenu::activate() {
    if (expanded && activeItemIndex >= 0 && activeItemIndex < static_cast<int>(items.size()) && coreRef) {
        const auto& activeItem = items[activeItemIndex];
        if (subMenuSelectedIndex >= 0 && subMenuSelectedIndex < static_cast<int>(activeItem.subCallbacks.size())) {
            if (activeItem.subCallbacks[subMenuSelectedIndex]) {
                auto callback = activeItem.subCallbacks[subMenuSelectedIndex];
                coreRef->queueCallback([callback]() {
                    try {
                        callback();
                    } catch (const std::exception& e) {
                        std::cerr << "Error in context menu callback: " << e.what() << std::endl;
                    }
                });
            }
            collapseMenu();
        }
    }
}

void ContextMenu::setMenuItems(const std::vector<TopMenuItem>& menus) {
    items.clear();
    for (const auto& topItem : menus) {
        MenuItem item;
        item.label = topItem.label;
        for (const auto& subItem : topItem.subItems) {
            item.subItemLabels.push_back(subItem.label);
            item.subCallbacks.push_back(subItem.callback);
        }
        items.push_back(item);
    }
    activeItemIndex = -1;
    expanded = false;
}

void ContextMenu::addMenuItem(const TopMenuItem& menu) {
    MenuItem item;
    item.label = menu.label;
    for (const auto& subItem : menu.subItems) {
        item.subItemLabels.push_back(subItem.label);
        item.subCallbacks.push_back(subItem.callback);
    }
    items.push_back(item);
}

void ContextMenu::clearMenuItems() {
    items.clear();
    activeItemIndex = -1;
    expanded = false;
}

void ContextMenu::setActiveItem(int index) {
    if (index >= 0 && index < static_cast<int>(items.size())) {
        activeItemIndex = index;
        subMenuSelectedIndex = 0; // Reset submenu selection
    }
}

void ContextMenu::expandMenu() {
    if (activeItemIndex >= 0 && activeItemIndex < static_cast<int>(items.size())) {
        expanded = true;
        subMenuSelectedIndex = 0;
    }
}

void ContextMenu::collapseMenu() {
    expanded = false;
    subMenuSelectedIndex = -1;
    hoveredSubIndex = -1;
}

void ContextMenu::selectSubItem(int index) {
    if (activeItemIndex >= 0 && activeItemIndex < static_cast<int>(items.size())) {
        const auto& activeItem = items[activeItemIndex];
        if (index >= 0 && index < static_cast<int>(activeItem.subItemLabels.size())) {
            subMenuSelectedIndex = index;
        }
    }
}

int ContextMenu::getItemAt(int mouseX, int mouseY) const {
    if (mouseY < y || mouseY >= y + height) {
        return -1;
    }
    
    int currentX = x + 5;
    for (int i = 0; i < static_cast<int>(items.size()); i++) {
        if (mouseX >= currentX && mouseX < currentX + items[i].width) {
            return i;
        }
        currentX += items[i].width;
    }
    
    return -1;
}

int ContextMenu::getSubItemAt(int mouseX, int mouseY) const {
    if (!expanded || activeItemIndex < 0 || activeItemIndex >= static_cast<int>(items.size())) {
        return -1;
    }
    
    SDL_Rect activeItemRect = getItemRect(activeItemIndex);
    int subMenuY = y + height;
    int subMenuHeight = static_cast<int>(items[activeItemIndex].subItemLabels.size()) * 25;
    
    if (mouseX >= activeItemRect.x && mouseX < activeItemRect.x + ui::Constants::SUBMENU_DEFAULT_WIDTH && // Default submenu width
        mouseY >= subMenuY && mouseY < subMenuY + subMenuHeight) {
        return (mouseY - subMenuY) / 25;
    }
    
    return -1;
}

SDL_Rect ContextMenu::getItemRect(int itemIndex) const {
    if (itemIndex < 0 || itemIndex >= static_cast<int>(items.size())) {
        return {0, 0, 0, 0};
    }
    
    int currentX = x + 5;
    for (int i = 0; i < itemIndex; i++) {
        currentX += items[i].width;
    }
    
    return {currentX, y, items[itemIndex].width, height};
}

SDL_Rect ContextMenu::getSubItemRect(int subItemIndex) const {
    if (!expanded || activeItemIndex < 0 || activeItemIndex >= static_cast<int>(items.size())) {
        return {0, 0, 0, 0};
    }
    
    const auto& activeItem = items[activeItemIndex];
    if (subItemIndex < 0 || subItemIndex >= static_cast<int>(activeItem.subItemLabels.size())) {
        return {0, 0, 0, 0};
    }
    
    SDL_Rect activeItemRect = getItemRect(activeItemIndex);
    int subMenuY = y + height + subItemIndex * 25;
    
    return {activeItemRect.x, subMenuY, ui::Constants::SUBMENU_DEFAULT_WIDTH, ui::Constants::SUBMENU_ITEM_HEIGHT};
}

void ContextMenu::calculateItemWidths(TTF_Font* font) {
    if (!font) {
        // Use default widths
        for (auto& item : items) {
            item.width = 80;
        }
        return;
    }
    
    for (auto& item : items) {
        int textWidth = 0;
        TTF_SizeText(font, item.label.c_str(), &textWidth, nullptr);
        item.width = textWidth + ui::Constants::MENU_ITEM_PADDING; // Add padding
    }
}

} // namespace ui
