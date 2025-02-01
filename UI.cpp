#include "UI.h"
#include <SDL2/SDL_ttf.h>
#include <iostream>

namespace ui {

// Utility: Draw filled rectangle.
static void drawFilledRect(SDL_Renderer* renderer, const SDL_Rect& rect, const Color& color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, &rect);
}

// Utility: Draw line.
static void drawLine(SDL_Renderer* renderer, int x1, int y1, int x2, int y2, const Color& color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
}

// For text rendering we use SDL_ttf. For brevity, we use a global font pointer.
static TTF_Font* globalFont = nullptr;
static const int FONT_SIZE = 16;

// Ensure font is loaded.
static void initFont() {
    if (!globalFont) {
        if (TTF_Init() == -1) {
            std::cerr << "TTF_Init failed: " << TTF_GetError() << std::endl;
            return;
        }
        // Use a default font (adjust the path as needed).
        globalFont = TTF_OpenFont("default_font.ttf", FONT_SIZE);
        if (!globalFont) {
            std::cerr << "TTF_OpenFont failed: " << TTF_GetError() << std::endl;
        }
    }
}

// Render text helper.
static void renderText(SDL_Renderer* renderer, const std::string& text, int x, int y, const Color& color) {
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

// Label rendering.
void Label::render(SDL_Renderer* renderer) {
    renderText(renderer, text, x, y, fontColor);
}

// Button rendering.
void Button::render(SDL_Renderer* renderer) {
    SDL_Rect rect = { x, y, width, height };
    // Button background changes when focused.
    Color bg = hasFocus ? Color(100, 100, 100) : Color(150, 150, 150);
    drawFilledRect(renderer, rect, bg);
    // Render text centered.
    initFont();
    if (!globalFont) return;
    SDL_Surface* surface = TTF_RenderText_Solid(globalFont, text.c_str(), { fontColor.r, fontColor.g, fontColor.b, fontColor.a });
    if (!surface) return;
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

void Button::handleEvent(const SDL_Event& e) {
    if (e.type == SDL_KEYDOWN && hasFocus) {
        if (e.key.keysym.sym == SDLK_RETURN || e.key.keysym.sym == SDLK_SPACE) {
            if (onClick) onClick();
        }
    }
}

// TextBox rendering.
void TextBox::render(SDL_Renderer* renderer) {
    initFont();
    if (!globalFont) return;

    const int padding = 5;
    int fontLineSkip = TTF_FontLineSkip(globalFont);
    int boxHeight = fontLineSkip + 2 * padding;

    SDL_Rect rect = { x, y, width, boxHeight };
    drawFilledRect(renderer, rect, Color(255, 255, 255));
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &rect);

    SDL_Surface* surface = TTF_RenderText_Solid(globalFont, content.c_str(), { fontColor.r, fontColor.g, fontColor.b, fontColor.a });
    if (!surface) return;
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    int textW = surface->w;
    int textH = surface->h;
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

void TextBox::handleEvent(const SDL_Event& e) {
    if (hasFocus) {
        if (e.type == SDL_TEXTINPUT) {
            content += e.text.text;
        }
        if (e.type == SDL_KEYDOWN) {
            if (e.key.keysym.sym == SDLK_BACKSPACE && !content.empty()) {
                content.pop_back();
            }
        }
    }
}

SDL_Rect TextBox::getFocusRect() const {
    // Use the same padding and computed height as in render().
    const int padding = 5;
    initFont();  // Ensure font is loaded.
    int boxHeight = TTF_FontLineSkip(globalFont) + 2 * padding;
    return SDL_Rect{ x - 2, y - 2, width + 4, boxHeight + 4 };
}


// CheckBox rendering.
void CheckBox::render(SDL_Renderer* renderer) {
    SDL_Rect rect = { x, y, width, height };
    drawFilledRect(renderer, rect, Color(255, 255, 255));
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &rect);
    if (checked) {
        // Draw an X for checked.
        drawLine(renderer, x, y, x + width, y + height, Color(0, 0, 0));
        drawLine(renderer, x + width, y, x, y + height, Color(0, 0, 0));
    }
}

void CheckBox::handleEvent(const SDL_Event& e) {
    if (hasFocus && e.type == SDL_KEYDOWN) {
        if (e.key.keysym.sym == SDLK_SPACE) {
            checked = !checked;
            if (onToggle) onToggle(checked);
        }
    }
}

// OptionSelect rendering.
void OptionSelect::render(SDL_Renderer* renderer) {
    int numOptions = options.size();
    if (numOptions == 0)
        return;

    // Calculate height for each option cell.
    int cellHeight = height / numOptions;

    for (int i = 0; i < numOptions; i++) {
        // Define the rectangle for this option.
        SDL_Rect cellRect = { x, y + i * cellHeight, width, cellHeight };

        // If this option is the active setting, fill with light green;
        // otherwise, use a standard gray background.
        if (i == activeIndex) {
            drawFilledRect(renderer, cellRect, Color(144, 238, 144)); // Light green.
        } else {
            drawFilledRect(renderer, cellRect, Color(200, 200, 200));
        }

        // Draw a black border for each option.
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderDrawRect(renderer, &cellRect);

        // Render the option text centered within the cell.
        initFont();
        if (!globalFont)
            continue;
        SDL_Surface* surface = TTF_RenderText_Solid(globalFont, options[i].c_str(), {0, 0, 0, 255});
        if (!surface)
            continue;
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        int textW = surface->w;
        int textH = surface->h;
        SDL_FreeSurface(surface);
        SDL_Rect dst = {
            cellRect.x + (cellRect.w - textW) / 2,
            cellRect.y + (cellRect.h - textH) / 2,
            textW,
            textH
        };
        SDL_RenderCopy(renderer, texture, nullptr, &dst);
        SDL_DestroyTexture(texture);
    }
}

void OptionSelect::handleEvent(const SDL_Event& e) {
    if (hasFocus && e.type == SDL_KEYDOWN) {
        // Cycle selection with UP and DOWN keys.
        if (e.key.keysym.sym == SDLK_UP) {
            selectedIndex = (selectedIndex - 1 + options.size()) % options.size();
            if (onSelect) onSelect(selectedIndex);
        } else if (e.key.keysym.sym == SDLK_DOWN) {
            selectedIndex = (selectedIndex + 1) % options.size();
            if (onSelect) onSelect(selectedIndex);
        }
        // Update the active index to match the interactive selection.
        activeIndex = selectedIndex;
    }
}

// Canvas rendering.
void Canvas::render(SDL_Renderer* renderer) {
    SDL_Rect rect = { x, y, width, height };
    // Clear canvas area.
    drawFilledRect(renderer, rect, Color(255, 255, 255));
    // Execute all drawing commands.
    for (auto& cmd : drawCommands) {
        cmd(renderer);
    }
}

// Draw rectangle on canvas.
void Canvas::rect(const SDL_Rect& r, const Color& color) {
    drawCommands.push_back([=](SDL_Renderer* renderer) {
        SDL_Rect rect = { x + r.x, y + r.y, r.w, r.h };
        drawFilledRect(renderer, rect, color);
    });
}

// Draw line on canvas.
void Canvas::line(int x1, int y1, int x2, int y2, const Color& color) {
    drawCommands.push_back([=](SDL_Renderer* renderer) {
        drawLine(renderer, x + x1, y + y1, x + x2, y + y2, color);
    });
}

// UIManager implementation.
UIManager::UIManager(const char* title, int width, int height,
                     const Color& bg, const Color& fg)
    : backgroundColor(bg), fontColor(fg),
      highlightColor(Color(181, 137, 0)) // Default highlight (Solarized yellow)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error:" << SDL_GetError() << std::endl;
    }
    window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              width, height, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Window could not be created! SDL_Error:" << SDL_GetError() << std::endl;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    // Enable text input for TextBox.
    SDL_StartTextInput();
}

UIManager::~UIManager() {
    SDL_StopTextInput();
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
}

void UIManager::addElement(std::shared_ptr<UIElement> element) {
    elements.push_back(element);
    // If no interactive element is focused, and the added element is interactive, focus it.
    if (focusedIndex == -1 && element->isInteractive())
        focusedIndex = elements.size() - 1;
}

void UIManager::setBackgroundColor(const Color& color) {
    backgroundColor = color;
}

void UIManager::setFontColor(const Color& color) {
    fontColor = color;
}

void UIManager::setHighlightColor(const Color& color) {
    highlightColor = color;
}

// Main loop.
void UIManager::run() {
    bool quit = false;
    SDL_Event e;
    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) quit = true;
            // Global key events.
            if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_TAB) {
                    // Remove focus from current element.
                    if (focusedIndex >= 0 && focusedIndex < (int)elements.size())
                        elements[focusedIndex]->hasFocus = false;
                    // Cycle through only interactive elements.
                    int start = focusedIndex;
                    do {
                        focusedIndex = (focusedIndex + 1) % elements.size();
                    } while (!elements[focusedIndex]->isInteractive() && focusedIndex != start);
                    elements[focusedIndex]->hasFocus = true;
                }
            }
            // Pass event to the focused element.
            if (focusedIndex >= 0 && focusedIndex < (int)elements.size())
                elements[focusedIndex]->handleEvent(e);
        }
        // Render loop.
        SDL_SetRenderDrawColor(renderer, backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);
        SDL_RenderClear(renderer);
        for (auto& el : elements) {
            el->render(renderer);
            if (el->hasFocus && el->isInteractive()) {
                SDL_Rect focusRect = el->getFocusRect();
                SDL_SetRenderDrawColor(renderer, highlightColor.r, highlightColor.g, highlightColor.b, highlightColor.a);
                SDL_RenderDrawRect(renderer, &focusRect);
            }
        }
        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }
}

} // namespace ui

