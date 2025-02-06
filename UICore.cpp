#include "UICore.h"
#include "Theme/ThemeFrameworkDefault.h"
#include "UIElements/Modal.h"
#include "UIElements/ContextMenu.h"
#include "UIElements/Label.h"
#include "UIElements/Button.h"
#include "Theme/ThemeGlobals.h"
#include "UIElements/TextBox.h"
#include <SDL2/SDL_ttf.h>
#include <iostream>

namespace ui {

// Define the global theme pointer.
std::shared_ptr<Theme> g_currentTheme = std::make_shared<ThemeFrameworkDefault>();

// Global font pointer and constant.
TTF_Font* globalFont = nullptr;
static const int FONT_SIZE = 14;

void initFont() {
    if (!globalFont) {
        if (TTF_Init() == -1) {
            std::cerr << "TTF_Init error: " << TTF_GetError() << std::endl;
            return;
        }
        globalFont = TTF_OpenFont("Assets/default_font.ttf", FONT_SIZE);
        if (!globalFont)
            std::cerr << "TTF_OpenFont error: " << TTF_GetError() << std::endl;
    }
}

void drawFilledRect(SDL_Renderer* renderer, const SDL_Rect &rect, const Color &color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, &rect);
}

void drawLine(SDL_Renderer* renderer, int x1, int y1, int x2, int y2, const Color &color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
}

void renderText(SDL_Renderer* renderer, const std::string &text, int x, int y, const Color &color) {
    initFont();
    if (!globalFont) return;
    SDL_Color sdlColor = { color.r, color.g, color.b, color.a };
    SDL_Surface* surface = TTF_RenderText_Solid(globalFont, text.c_str(), sdlColor);
    if (!surface) return;
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect dst = { x, y, surface->w, surface->h };
    SDL_FreeSurface(surface);
    SDL_RenderCopy(renderer, texture, nullptr, &dst);
    SDL_DestroyTexture(texture);
}


// --- CheckBox ---
void CheckBox::render(SDL_Renderer* renderer) {
    SDL_Rect rect = { x, y, width, height };
    ThemeableElementColors tc = g_currentTheme->checkboxColors();
    drawFilledRect(renderer, rect, tc.checkboxEnabled);
    
    // Draw 3D border for the checkbox.
    SDL_SetRenderDrawColor(renderer, tc.checkboxBorderLight.r, tc.checkboxBorderLight.g, tc.checkboxBorderLight.b, tc.checkboxBorderLight.a);
    SDL_RenderDrawLine(renderer, rect.x, rect.y, rect.x + rect.w, rect.y);         // Top edge.
    SDL_RenderDrawLine(renderer, rect.x, rect.y, rect.x, rect.y + rect.h);         // Left edge.
    SDL_SetRenderDrawColor(renderer, tc.checkboxBorderDark.r, tc.checkboxBorderDark.g, tc.checkboxBorderDark.b, tc.checkboxBorderDark.a);
    SDL_RenderDrawLine(renderer, rect.x, rect.y + rect.h, rect.x + rect.w, rect.y + rect.h); // Bottom edge.
    SDL_RenderDrawLine(renderer, rect.x + rect.w, rect.y, rect.x + rect.w, rect.y + rect.h); // Right edge.
    
    // If checked, draw a check mark using the checkbox checked color.
    if (checked) {
        drawLine(renderer, x, y, x + width, y + height, tc.checkboxChecked);
        drawLine(renderer, x + width, y, x, y + height, tc.checkboxChecked);
    }
}


void CheckBox::handleEvent(const SDL_Event &e) {
    if (hasFocus && e.type == SDL_KEYDOWN) {
        if (e.key.keysym.sym == SDLK_SPACE) {
            checked = !checked;
            if (onToggle) onToggle(checked);
        }
    }
}

// --- OptionSelect: render and event handling ---
void OptionSelect::render(SDL_Renderer* renderer) {
    const int padding = 5;
    initFont();
    // Collapsed state uses dynamic height (like TextBox)
    int collapsedHeight = TTF_FontLineSkip(globalFont) + 2 * padding;
    
    if (!expanded) {
        // Collapsed: Draw one cell using the collapsed height.
        SDL_Rect cellRect = { x, y, width, collapsedHeight };
        ThemeableElementColors tc = g_currentTheme->optionSelectColors();
        drawFilledRect(renderer, cellRect, tc.selectOptionUnselected);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderDrawRect(renderer, &cellRect);
        
        // Compute available width for text, leaving room for the arrow.
        int arrowWidth = 10;
        int availWidth = width - padding - arrowWidth;
        std::string displayText = options[activeIndex];
        
        // Measure text width.
        int textW = 0, textH = 0;
        TTF_SizeText(globalFont, displayText.c_str(), &textW, &textH);
        // If text is too wide, truncate and append ellipsis.
        if (textW > availWidth) {
            std::string ellipsis = "...";
            int ellipsisW = 0;
            TTF_SizeText(globalFont, ellipsis.c_str(), &ellipsisW, &textH);
            // Remove characters until it fits.
            while (!displayText.empty()) {
                std::string candidate = displayText + ellipsis;
                TTF_SizeText(globalFont, candidate.c_str(), &textW, &textH);
                if (textW <= availWidth)
                    break;
                displayText.pop_back();
            }
            displayText += ellipsis;
        }
        
        // Render the active option text (left aligned with padding).
        if (globalFont) {
            SDL_Color textColor = { 0, 0, 0, 255 };
            SDL_Surface* surface = TTF_RenderText_Solid(globalFont, displayText.c_str(), textColor);
            if (surface) {
                SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
                int finalTextW = surface->w, finalTextH = surface->h;
                SDL_FreeSurface(surface);
                SDL_Rect dst = { x + padding, y + (collapsedHeight - finalTextH) / 2, finalTextW, finalTextH };
                SDL_RenderCopy(renderer, texture, nullptr, &dst);
                SDL_DestroyTexture(texture);
            }
        }
        
        // Draw a downward arrow on the right.
        int arrowPadding = 5;
        SDL_Point arrow[3];
        arrow[0] = { x + width - arrowPadding - 10, y + collapsedHeight / 2 - 3 };
        arrow[1] = { x + width - arrowPadding, y + collapsedHeight / 2 - 3 };
        arrow[2] = { x + width - arrowPadding - 5, y + collapsedHeight / 2 + 3 };
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderDrawLines(renderer, arrow, 3);
        SDL_RenderDrawLine(renderer, arrow[2].x, arrow[2].y, arrow[0].x, arrow[0].y);
    } else {
        // Expanded state: For each option, use a fixed cell height (e.g., 30 pixels).
        int cellHeight = 30;
        ThemeableElementColors tc = g_currentTheme->optionSelectColors();
        for (size_t i = 0; i < options.size(); i++) {
            SDL_Rect cellRect = { x, y + static_cast<int>(i) * cellHeight, width, cellHeight };
            if (static_cast<int>(i) == selectedIndex)
                drawFilledRect(renderer, cellRect, tc.selectOptionSelected);
            else
                drawFilledRect(renderer, cellRect, tc.selectOptionUnselected);
            
            // Draw 3D border using theme border colors.
            SDL_SetRenderDrawColor(renderer, tc.selectOptionBorderLight.r, tc.selectOptionBorderLight.g,
                                    tc.selectOptionBorderLight.b, tc.selectOptionBorderLight.a);
            SDL_RenderDrawLine(renderer, cellRect.x, cellRect.y, cellRect.x + cellRect.w, cellRect.y);
            SDL_RenderDrawLine(renderer, cellRect.x, cellRect.y, cellRect.x, cellRect.y + cellRect.h);
            SDL_SetRenderDrawColor(renderer, tc.selectOptionBorderDark.r, tc.selectOptionBorderDark.g,
                                    tc.selectOptionBorderDark.b, tc.selectOptionBorderDark.a);
            SDL_RenderDrawLine(renderer, cellRect.x, cellRect.y + cellRect.h, cellRect.x + cellRect.w, cellRect.y + cellRect.h);
            SDL_RenderDrawLine(renderer, cellRect.x + cellRect.w, cellRect.y, cellRect.x + cellRect.w, cellRect.y + cellRect.h);
            
            // Truncate text if necessary.
            int availableWidth = cellRect.w - 2 * padding;
            std::string optionText = options[i];
            int textW = 0, textH = 0;
            TTF_SizeText(globalFont, optionText.c_str(), &textW, &textH);
            if (textW > availableWidth) {
                std::string ellipsis = "...";
                int ellipsisW = 0;
                TTF_SizeText(globalFont, ellipsis.c_str(), &ellipsisW, &textH);
                while (!optionText.empty()) {
                    std::string candidate = optionText + ellipsis;
                    TTF_SizeText(globalFont, candidate.c_str(), &textW, &textH);
                    if (textW <= availableWidth)
                        break;
                    optionText.pop_back();
                }
                optionText += ellipsis;
            }
            
            // Render the option text centered.
            if (globalFont) {
                SDL_Color textColor = { 0, 0, 0, 255 };
                SDL_Surface* surface = TTF_RenderText_Solid(globalFont, optionText.c_str(), textColor);
                if (surface) {
                    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
                    int finalTextW = surface->w, finalTextH = surface->h;
                    SDL_FreeSurface(surface);
                    SDL_Rect dst = { cellRect.x + (cellRect.w - finalTextW) / 2,
                                     cellRect.y + (cellRect.h - finalTextH) / 2,
                                     finalTextW, finalTextH };
                    SDL_RenderCopy(renderer, texture, nullptr, &dst);
                    SDL_DestroyTexture(texture);
                }
            }
        }
    }
}

void OptionSelect::handleEvent(const SDL_Event &e) {
    if (expanded) {
        if (e.type == SDL_KEYDOWN) {
            if (e.key.keysym.sym == SDLK_UP) {
                selectedIndex = (selectedIndex - 1 + options.size()) % options.size();
            } else if (e.key.keysym.sym == SDLK_DOWN) {
                selectedIndex = (selectedIndex + 1) % options.size();
            } else if (e.key.keysym.sym == SDLK_RETURN) {
                activeIndex = selectedIndex;
                expanded = false;
                if (onSelect) onSelect(activeIndex);
            }
        }
    } else {
        if (hasFocus && e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_RETURN) {
            expanded = true;
            selectedIndex = activeIndex;
        }
    }
}


// Override getFocusRect so the focus outline uses the collapsed height when not expanded.
SDL_Rect OptionSelect::getFocusRect() const {
    if (!expanded) {
        // Collapsed state: use dynamic height as before.
        const int padding = 5;
        initFont();
        int collapsedHeight = TTF_FontLineSkip(globalFont) + 2 * padding;
        return SDL_Rect{ x - 2, y - 2, width + 4, collapsedHeight + 4 };
    } else {
        // Expanded state: use the fixed cell height (30 pixels) for each option.
        int cellHeight = 30;
        return SDL_Rect{ x - 2, y - 2, width + 4, static_cast<int>(options.size()) * cellHeight + 4 };
    }
}

// --- Canvas ---
void Canvas::render(SDL_Renderer* renderer) {
    SDL_Rect rect = { x, y, width, height };
    // Use the theme's background color.
    drawFilledRect(renderer, rect, g_currentTheme->backgroundColor());
    for (auto& cmd : drawCommands)
        cmd(renderer);
}

void Canvas::rect(const SDL_Rect &r, const Color &color) {
    drawCommands.push_back([=](SDL_Renderer* renderer) {
        SDL_Rect rect = { x + r.x, y + r.y, r.w, r.h };
        drawFilledRect(renderer, rect, color);
    });
}

void Canvas::line(int x1, int y1, int x2, int y2, const Color &color) {
    drawCommands.push_back([=](SDL_Renderer* renderer) {
        drawLine(renderer, x + x1, y + y1, x + x2, y + y2, color);
    });
}

// --- UICore::UICore ---
UICore::UICore(const char* title, int w, int h, std::shared_ptr<Theme> theme)
    : width(w), height(h), currentTheme(theme) 
{
    g_currentTheme = currentTheme;
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL_Init error: " << SDL_GetError() << std::endl;
    }
    window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                w, h, SDL_WINDOW_SHOWN);
    if (!window)
        std::cerr << "SDL_CreateWindow error: " << SDL_GetError() << std::endl;
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_StartTextInput();
}

// --- UICore Destructor ---
UICore::~UICore() {
    SDL_StopTextInput();
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
}

SDL_Keycode UICore::keycodeFromString(const std::string &s) {
    if (s.empty())
        return SDLK_UNKNOWN;
    char ch = s[0];
    ch = static_cast<char>(std::tolower(ch));
    if (std::isdigit(ch))
        return SDLK_0 + (ch - '0');
    return ch; // For letters and other characters.
}

void UICore::addElement(std::shared_ptr<UIElement> element) {
    elements.push_back(element);
    if (focusedIndex == -1 && element->isInteractive())
        focusedIndex = elements.size() - 1;
}

void UICore::setTheme(std::shared_ptr<Theme> theme) {
    currentTheme = theme;
    g_currentTheme = currentTheme;
}


void UICore::setFocus(UIElement* element) {
    // Clear current focus on all elements.
    for (auto &el : elements) {
        el->hasFocus = false;
    }
    focusedIndex = -1;
    
    // Find the element in the elements vector and set its focus.
    for (size_t i = 0; i < elements.size(); i++) {
        if (elements[i].get() == element) {
            elements[i]->hasFocus = true;
            focusedIndex = static_cast<int>(i);
            break;
        }
    }
}

void UICore::showQuitConfirmation(bool &quit) {
    int modalW = 400, modalH = 150;
    int modalX = (width - modalW) / 2;
    int modalY = (height - modalH) / 2;
    auto confirmModal = std::make_shared<ui::Modal>(modalX, modalY, modalW, modalH, "Are you sure you want to quit?");
    
    // Clear any existing button labels and callbacks.
    confirmModal->buttonLabels.clear();
    confirmModal->buttonCallbacks.clear();
    
    // Set up the "Confirm" button.
    confirmModal->buttonLabels.push_back("Confirm");
    confirmModal->buttonCallbacks.push_back([&quit, confirmModal]() {
         quit = true;
         confirmModal->dismissed = true;
    });
    
    // Set up the "Cancel" button.
    confirmModal->buttonLabels.push_back("Cancel");
    confirmModal->buttonCallbacks.push_back([confirmModal]() {
         confirmModal->dismissed = true;
    });
    
    // When dismissed, reset modalActive.
    confirmModal->onDismiss = [this]() {
         modalActive = false;
    };
    
    // Mark that a modal is active and add it.
    modalActive = true;
    addElement(confirmModal);
}


void UICore::run() {
    bool quit = false;
    SDL_Event e;
    while (!quit) {
        // Remove dismissed modals from the element list.
        for (auto it = elements.begin(); it != elements.end();) {
            if (auto m = dynamic_cast<ui::Modal*>(it->get())) {
                if (m->dismissed)
                    it = elements.erase(it);
                else
                    ++it;
            } else {
                ++it;
            }
        }
        
        while (SDL_PollEvent(&e)) {
            // If a hot key (with CTRL) is pressed, try to trigger it.
            if (e.type == SDL_KEYDOWN && (e.key.keysym.mod & KMOD_CTRL)) {
                 SDL_Keycode key = e.key.keysym.sym;
                 auto it = hotKeys.find(key);
                 if (it != hotKeys.end()) {
                     it->second(); // Trigger the registered hot key callback.
                     // Optionally, skip further event processing for this iteration.
                     continue;
                 }
            }
            // Global ESC quits only if no modal is active.
            if (!modalActive && e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) {
                showQuitConfirmation(quit);
                continue; // Skip further event processing this iteration.
            }
            if (e.type == SDL_QUIT)
                quit = true;
            
            // If a modal is active, route events only to modal elements.
            if (modalActive) {
                for (auto &el : elements) {
                    if (auto m = dynamic_cast<ui::Modal*>(el.get())) {
                        if (!m->dismissed)
                            m->handleEvent(e);
                    }
                }
            } else {
                // Normal event handling.
                if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_TAB) {
                    if (focusedIndex >= 0 && focusedIndex < (int)elements.size())
                        elements[focusedIndex]->hasFocus = false;
                    int start = focusedIndex;
                    do {
                        focusedIndex = (focusedIndex + 1) % elements.size();
                    } while (!elements[focusedIndex]->isInteractive() && focusedIndex != start);
                    elements[focusedIndex]->hasFocus = true;
                }
                if (focusedIndex >= 0 && focusedIndex < (int)elements.size())
                    elements[focusedIndex]->handleEvent(e);
            }
        }
        
        // Remove dismissed modals again.
        for (auto it = elements.begin(); it != elements.end();) {
            if (auto m = dynamic_cast<ui::Modal*>(it->get())) {
                if (m->dismissed)
                    it = elements.erase(it);
                else
                    ++it;
            } else {
                ++it;
            }
        }
        
        SDL_SetRenderDrawColor(renderer, currentTheme->backgroundColor().r,
                               currentTheme->backgroundColor().g,
                               currentTheme->backgroundColor().b,
                               currentTheme->backgroundColor().a);
        SDL_RenderClear(renderer);
        
        // First pass: render all elements except expanded ContextMenu.
        for (auto &el : elements) {
            if (auto cm = dynamic_cast<ui::ContextMenu*>(el.get())) {
                if (cm->expanded)
                    continue;
            }
            el->render(renderer);
            if (!modalActive && el->hasFocus && el->isInteractive()) {
                SDL_Rect focusRect = el->getFocusRect();
                SDL_SetRenderDrawColor(renderer, currentTheme->highlightColor().r,
                                       currentTheme->highlightColor().g,
                                       currentTheme->highlightColor().b,
                                       currentTheme->highlightColor().a);
                SDL_RenderDrawRect(renderer, &focusRect);
            }
        }
        
        // Second pass: render expanded ContextMenu elements on top.
        for (auto &el : elements) {
            if (auto cm = dynamic_cast<ui::ContextMenu*>(el.get())) {
                if (cm->expanded) {
                    cm->render(renderer);
                    // Optionally, draw a focus outline for the context menu if desired.
                }
            }
        }
        
        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }
}

} // namespace ui
