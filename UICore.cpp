#include "UICore.h"
#include "ThemeFrameworkDefault.h"  // For default theme fallback.
#include "ThemeGlobals.h"
#include <SDL2/SDL_ttf.h>
#include <iostream>

namespace ui {

// Define the global theme pointer.
std::shared_ptr<Theme> g_currentTheme = std::make_shared<ThemeFrameworkDefault>();

// Global font pointer and constant.
static TTF_Font* globalFont = nullptr;
static const int FONT_SIZE = 16;

static void initFont() {
    if (!globalFont) {
        if (TTF_Init() == -1) {
            std::cerr << "TTF_Init error: " << TTF_GetError() << std::endl;
            return;
        }
        globalFont = TTF_OpenFont("default_font.ttf", FONT_SIZE);
        if (!globalFont)
            std::cerr << "TTF_OpenFont error: " << TTF_GetError() << std::endl;
    }
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

// Define helper functions as static (only visible in this file).
static void drawFilledRect(SDL_Renderer* renderer, const SDL_Rect &rect, const Color &color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, &rect);
}

static void drawLine(SDL_Renderer* renderer, int x1, int y1, int x2, int y2, const Color &color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
}

// --- Label ---
void Label::render(SDL_Renderer* renderer) {
    ThemeableElementColors tc = g_currentTheme->labelColors();
    renderText(renderer, text, x, y, tc.labelText);
}

// --- Button ---
void Button::render(SDL_Renderer* renderer) {
    // Get theme colors for button.
    ThemeableElementColors tc = g_currentTheme->buttonColors();
    SDL_Rect rect = { x, y, width, height };
    // Draw the button background.
    drawFilledRect(renderer, rect, tc.buttonBackground);
    
    // Render the button text centered.
    initFont();
    if (!globalFont) return;
    SDL_Color sdlColor = { tc.buttonText.r, tc.buttonText.g, tc.buttonText.b, tc.buttonText.a };
    SDL_Surface* surface = TTF_RenderText_Solid(globalFont, text.c_str(), sdlColor);
    if (surface) {
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_Rect dst;
        dst.w = surface->w;
        dst.h = surface->h;
        dst.x = x + (width - dst.w) / 2;
        dst.y = y + (height - dst.h) / 2;
        SDL_FreeSurface(surface);
        SDL_RenderCopy(renderer, texture, nullptr, &dst);
        SDL_DestroyTexture(texture);
    }
    
    // Draw 3D border.
    SDL_SetRenderDrawColor(renderer, tc.buttonBorderLight.r, tc.buttonBorderLight.g, tc.buttonBorderLight.b, tc.buttonBorderLight.a);
    SDL_RenderDrawLine(renderer, x, y, x + width, y);        // Top edge.
    SDL_RenderDrawLine(renderer, x, y, x, y + height);       // Left edge.
    SDL_SetRenderDrawColor(renderer, tc.buttonBorderDark.r, tc.buttonBorderDark.g, tc.buttonBorderDark.b, tc.buttonBorderDark.a);
    SDL_RenderDrawLine(renderer, x, y + height, x + width, y + height); // Bottom edge.
    SDL_RenderDrawLine(renderer, x + width, y, x + width, y + height);  // Right edge.
}


void Button::handleEvent(const SDL_Event &e) {
    if (hasFocus && e.type == SDL_KEYDOWN) {
        if (e.key.keysym.sym == SDLK_RETURN || e.key.keysym.sym == SDLK_SPACE) {
            if (onClick) onClick();
        }
    }
}

// --- TextBox ---
void TextBox::render(SDL_Renderer* renderer) {
    initFont();
    if (!globalFont) return;
    const int padding = 5;
    int boxHeight = TTF_FontLineSkip(globalFont) + 2 * padding;
    SDL_Rect rect = { x, y, width, boxHeight };
    
    // Retrieve text input colors from theme.
    ThemeableElementColors tc = g_currentTheme->textInputColors();
    drawFilledRect(renderer, rect, tc.textInputBackground);
    
    // Draw 3D border using theme border colors.
    SDL_SetRenderDrawColor(renderer, tc.textInputBorderLight.r, tc.textInputBorderLight.g, tc.textInputBorderLight.b, tc.textInputBorderLight.a);
    SDL_RenderDrawLine(renderer, x, y, x + width, y);          // Top edge.
    SDL_RenderDrawLine(renderer, x, y, x, y + boxHeight);        // Left edge.
    SDL_SetRenderDrawColor(renderer, tc.textInputBorderDark.r, tc.textInputBorderDark.g, tc.textInputBorderDark.b, tc.textInputBorderDark.a);
    SDL_RenderDrawLine(renderer, x, y + boxHeight, x + width, y + boxHeight); // Bottom edge.
    SDL_RenderDrawLine(renderer, x + width, y, x + width, y + boxHeight);     // Right edge.
    
    // Render the text.
    SDL_Color sdlColor = { tc.textInputText.r, tc.textInputText.g, tc.textInputText.b, tc.textInputText.a };
    SDL_Surface* surface = TTF_RenderText_Solid(globalFont, content.c_str(), sdlColor);
    if (!surface) return;
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    int textW = surface->w, textH = surface->h;
    SDL_FreeSurface(surface);
    int availableWidth = width - 2 * padding;
    int offsetX = (textW > availableWidth) ? (textW - availableWidth) : 0;
    SDL_Rect dst = { x + padding - offsetX, y + padding, textW, textH };
    SDL_Rect clipRect = { x + padding, y + padding, availableWidth, boxHeight - 2 * padding };
    SDL_RenderSetClipRect(renderer, &clipRect);
    SDL_RenderCopy(renderer, texture, nullptr, &dst);
    SDL_RenderSetClipRect(renderer, nullptr);
    SDL_DestroyTexture(texture);
}


void TextBox::handleEvent(const SDL_Event &e) {
    if (hasFocus) {
        if (e.type == SDL_TEXTINPUT)
            content += e.text.text;
        if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_BACKSPACE && !content.empty())
            content.pop_back();
    }
}

SDL_Rect TextBox::getFocusRect() const {
    const int padding = 5;
    initFont();
    int boxHeight = TTF_FontLineSkip(globalFont) + 2 * padding;
    return SDL_Rect{ x - 2, y - 2, width + 4, boxHeight + 4 };
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
        // Collapsed: Draw a single cell using the collapsed height.
        SDL_Rect cellRect = { x, y, width, collapsedHeight };
        ThemeableElementColors tc = g_currentTheme->optionSelectColors();
        drawFilledRect(renderer, cellRect, tc.selectOptionUnselected);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderDrawRect(renderer, &cellRect);
        
        // Render the active option text (left aligned with padding).
        if (globalFont) {
            SDL_Color textColor = { 0, 0, 0, 255 };
            SDL_Surface* surface = TTF_RenderText_Solid(globalFont, options[activeIndex].c_str(), textColor);
            if (surface) {
                SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
                int textW = surface->w, textH = surface->h;
                SDL_FreeSurface(surface);
                SDL_Rect dst = { x + padding, y + (collapsedHeight - textH) / 2, textW, textH };
                SDL_RenderCopy(renderer, texture, nullptr, &dst);
                SDL_DestroyTexture(texture);
            }
        }
        
        // Draw a downward arrow on the right.
        int arrowPadding = 5;
        // For a downward arrow, define three points:
        SDL_Point arrow[3];
        // Top-left of arrow
        arrow[0] = { x + width - arrowPadding - 10, y + collapsedHeight / 2 - 3 };
        // Top-right of arrow
        arrow[1] = { x + width - arrowPadding, y + collapsedHeight / 2 - 3 };
        // Bottom point (centered)
        arrow[2] = { x + width - arrowPadding - 5, y + collapsedHeight / 2 + 3 };
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderDrawLines(renderer, arrow, 3);
        // Connect last to first to complete the triangle.
        SDL_RenderDrawLine(renderer, arrow[2].x, arrow[2].y, arrow[0].x, arrow[0].y);
    } else {
        // Expanded state: Draw each option in a fixed, smaller cell height.
        int cellHeight = 25;
        ThemeableElementColors tc = g_currentTheme->optionSelectColors();
        for (size_t i = 0; i < options.size(); i++) {
            SDL_Rect cellRect = { x, y + static_cast<int>(i) * cellHeight, width, cellHeight };
            if (static_cast<int>(i) == selectedIndex)
                drawFilledRect(renderer, cellRect, tc.selectOptionSelected);
            else
                drawFilledRect(renderer, cellRect, tc.selectOptionUnselected);
            
            // Draw a 3D border: top & left light; bottom & right dark.
            SDL_SetRenderDrawColor(renderer, tc.selectOptionBorderLight.r, tc.selectOptionBorderLight.g, tc.selectOptionBorderLight.b, tc.selectOptionBorderLight.a);
            SDL_RenderDrawLine(renderer, cellRect.x, cellRect.y, cellRect.x + cellRect.w, cellRect.y);
            SDL_RenderDrawLine(renderer, cellRect.x, cellRect.y, cellRect.x, cellRect.y + cellRect.h);
            SDL_SetRenderDrawColor(renderer, tc.selectOptionBorderDark.r, tc.selectOptionBorderDark.g, tc.selectOptionBorderDark.b, tc.selectOptionBorderDark.a);
            SDL_RenderDrawLine(renderer, cellRect.x, cellRect.y + cellRect.h, cellRect.x + cellRect.w, cellRect.y + cellRect.h);
            SDL_RenderDrawLine(renderer, cellRect.x + cellRect.w, cellRect.y, cellRect.x + cellRect.w, cellRect.y + cellRect.h);
            
            // Center the option text.
            if (globalFont) {
                SDL_Color textColor = { 0, 0, 0, 255 };
                SDL_Surface* surface = TTF_RenderText_Solid(globalFont, options[i].c_str(), textColor);
                if (surface) {
                    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
                    int textW = surface->w, textH = surface->h;
                    SDL_FreeSurface(surface);
                    SDL_Rect dst = { cellRect.x + (cellRect.w - textW) / 2,
                                     cellRect.y + (cellRect.h - textH) / 2,
                                     textW, textH };
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
    const int padding = 5;
    initFont();
    if (!expanded) {
        int collapsedHeight = TTF_FontLineSkip(globalFont) + 2 * padding;
        return SDL_Rect{ x - 2, y - 2, width + 4, collapsedHeight + 4 };
    } else {
        // When expanded, outline the entire drop-down list.
        int cellHeight = 25;
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

// --- UICore ---
UICore::UICore(const char* title, int width, int height, std::shared_ptr<Theme> theme)
    : currentTheme(theme)
{
    g_currentTheme = currentTheme;
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
        std::cerr << "SDL_Init error: " << SDL_GetError() << std::endl;
    window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                width, height, SDL_WINDOW_SHOWN);
    if (!window)
        std::cerr << "SDL_CreateWindow error: " << SDL_GetError() << std::endl;
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_StartTextInput();
}

UICore::~UICore() {
    SDL_StopTextInput();
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
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

void UICore::run() {
    bool quit = false;
    SDL_Event e;
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT)
                quit = true;
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
        SDL_SetRenderDrawColor(renderer, currentTheme->backgroundColor().r,
                               currentTheme->backgroundColor().g,
                               currentTheme->backgroundColor().b,
                               currentTheme->backgroundColor().a);
        SDL_RenderClear(renderer);
        for (auto& el : elements) {
            el->render(renderer);
            if (el->hasFocus && el->isInteractive()) {
                SDL_Rect focusRect = el->getFocusRect();
                SDL_SetRenderDrawColor(renderer, currentTheme->highlightColor().r,
                                       currentTheme->highlightColor().g,
                                       currentTheme->highlightColor().b,
                                       currentTheme->highlightColor().a);
                SDL_RenderDrawRect(renderer, &focusRect);
            }
        }
        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }
}

} // namespace ui
