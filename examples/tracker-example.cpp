#include "../src/UI.h"
#include <memory>
#include <vector>
#include <iostream>
#include <SDL2/SDL.h>
#include "../Theme/ThemeGlobals.h"
#include <functional>
#include <algorithm>
#include <cctype>

// --- Custom TrackerGrid element ---
namespace ui {
    class TrackerGrid : public UIElement {
    public:
        int columns;
        int rows;
        int currentColumn;
        int currentRow;
        // Callback when a cell is activated.
        std::function<void(int col, int row)> onCellActivated;
    
        TrackerGrid(int x_, int y_, int w_, int h_, int cols, int rows_)
          : UIElement(x_, y_, w_, h_), columns(cols), rows(rows_), currentColumn(0), currentRow(0) {}
    
        virtual void render(SDL_Renderer* renderer) override {
            int cellWidth = width / columns;
            int cellHeight = height / rows;
            for (int r = 0; r < rows; r++) {
                for (int c = 0; c < columns; c++) {
                    SDL_Rect cellRect = { x + c * cellWidth, y + r * cellHeight, cellWidth, cellHeight };
                    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
                    SDL_RenderDrawRect(renderer, &cellRect);
                    if (r == currentRow && c == currentColumn) {
                        SDL_SetRenderDrawColor(renderer, 255, 121, 198, 100); // semi-transparent pink
                        SDL_RenderFillRect(renderer, &cellRect);
                    }
                }
            }
        }
    
        virtual void handleEvent(const SDL_Event &e) override {
            if (e.type == SDL_KEYDOWN) {
                switch(e.key.keysym.sym) {
                    case SDLK_UP:
                        currentRow = (currentRow - 1 + rows) % rows;
                        break;
                    case SDLK_DOWN:
                        currentRow = (currentRow + 1) % rows;
                        break;
                    case SDLK_LEFT:
                        currentColumn = (currentColumn - 1 + columns) % columns;
                        break;
                    case SDLK_RIGHT:
                        currentColumn = (currentColumn + 1) % columns;
                        break;
                    case SDLK_RETURN:
                        if (onCellActivated)
                            onCellActivated(currentColumn, currentRow);
                        break;
                    default:
                        break;
                }
            }
        }
    
        virtual bool isInteractive() const override { return true; }
    
        virtual SDL_Rect getFocusRect() const override {
            return SDL_Rect{ x, y, width, height };
        }
    
        virtual void activate() override {
            hasFocus = true;
        }
    };
} // namespace ui

// --- Custom ShuttleControl element ---
namespace ui {
    class ShuttleControl : public UIElement {
    public:
        int buttonWidth;
        int buttonHeight;
        int focusedButton; // 0: Play, 1: Pause, 2: Record.
    
        ShuttleControl(int x_, int y_, int w_, int h_)
          : UIElement(x_, y_, w_, h_), buttonWidth(80), buttonHeight(40), focusedButton(0) {}
    
        virtual void render(SDL_Renderer* renderer) override {
            int spacing = 10;
            int totalWidth = 3 * buttonWidth + 2 * spacing;
            int startX = x + (width - totalWidth) / 2;
            int btnY = y + (height - buttonHeight) / 2;
            const char* labels[3] = {"Play", "Pause", "Record"};
            for (int i = 0; i < 3; i++) {
                int btnX = startX + i * (buttonWidth + spacing);
                SDL_Rect btnRect = { btnX, btnY, buttonWidth, buttonHeight };
                // Draw button background.
                drawFilledRect(renderer, btnRect, g_currentTheme->buttonColors().buttonBackground);
                // Draw button border.
                SDL_SetRenderDrawColor(renderer, 
                    g_currentTheme->buttonColors().buttonBorderDark.r,
                    g_currentTheme->buttonColors().buttonBorderDark.g,
                    g_currentTheme->buttonColors().buttonBorderDark.b,
                    g_currentTheme->buttonColors().buttonBorderDark.a);
                SDL_RenderDrawRect(renderer, &btnRect);
                // If this button is focused, draw an extra highlighted border.
                if (i == focusedButton) {
                    // Use the theme's defaultHighlight instead of g_currentTheme->highlight.
                    SDL_SetRenderDrawColor(renderer, 
                        255,
                        0,
                        0,
                        255);
                    SDL_Rect hlRect = { btnRect.x - 1, btnRect.y - 1, btnRect.w + 2, btnRect.h + 2 };
                    SDL_RenderDrawRect(renderer, &hlRect);
                }
                // Render button label.
                if (globalFont) {
                    SDL_Color textColor = {
                        g_currentTheme->buttonColors().buttonText.r,
                        g_currentTheme->buttonColors().buttonText.g,
                        g_currentTheme->buttonColors().buttonText.b,
                        g_currentTheme->buttonColors().buttonText.a
                    };
                    SDL_Surface* surface = TTF_RenderText_Solid(globalFont, labels[i], textColor);
                    if (surface) {
                        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
                        int textW = surface->w, textH = surface->h;
                        SDL_FreeSurface(surface);
                        SDL_Rect dst = { btnX + (buttonWidth - textW) / 2, btnY + (buttonHeight - textH) / 2, textW, textH };
                        SDL_RenderCopy(renderer, texture, nullptr, &dst);
                        SDL_DestroyTexture(texture);
                    }
                }
            }
        }
    
        virtual void handleEvent(const SDL_Event &e) override {
            if (e.type == SDL_KEYDOWN) {
                switch(e.key.keysym.sym) {
                    case SDLK_LEFT:
                        focusedButton = (focusedButton - 1 + 3) % 3;
                        break;
                    case SDLK_RIGHT:
                        focusedButton = (focusedButton + 1) % 3;
                        break;
                    case SDLK_RETURN:
                        if (focusedButton == 0)
                            std::cout << "Play pressed" << std::endl;
                        else if (focusedButton == 1)
                            std::cout << "Pause pressed" << std::endl;
                        else if (focusedButton == 2)
                            std::cout << "Record pressed" << std::endl;
                        break;
                    default:
                        break;
                }
            }
        }
    
        virtual bool isInteractive() const override { return true; }
    
        virtual SDL_Rect getFocusRect() const override {
            return SDL_Rect{ x, y, width, height };
        }
    
        virtual void activate() override {
            hasFocus = true;
        }
    };
} // namespace ui

// --- Main Demo Application ---
int main() {
    // Create the UI façade with a 1024x896 window.
    auto ui = std::make_unique<UI>("Audio Tracker", 1024, 896);
    
    // --- Context Menu ---
    std::vector<ui::TopMenuItem> menus = {
        { "File", { 
            {"Open", [](){ std::cout << "Open selected" << std::endl; }}, 
            {"Save", [](){ std::cout << "Save selected" << std::endl; }},
            {"Quit", [uiPtr = ui.get()](){
                 uiPtr->confirmModal("Are you sure you want to quit?",
                     [](){ exit(0); },
                     [](){ std::cout << "Quit cancelled." << std::endl; }
                 );
            }}
        }},
        { "Theme", { 
            {"Default", [uiPtr = ui.get()](){ uiPtr->setTheme("FrameworkDefault"); }},
            {"Solarized Dark", [uiPtr = ui.get()](){ uiPtr->setTheme("SolarizedDark"); }},
            {"Solarized Light", [uiPtr = ui.get()](){ uiPtr->setTheme("SolarizedLight"); }},
            {"Molokai", [uiPtr = ui.get()](){ uiPtr->setTheme("Molokai"); }}
        }},
        { "Help", { 
            {"Manual", [](){ std::cout << "Manual selected" << std::endl; }},
            {"Credits", [](){ std::cout << "Credits selected" << std::endl; }}
        }}
    };
    ui::ContextMenu* ctxMenu = ui->contextMenu(menus);
    
    // --- Tracker Grid (4 columns x 16 rows) ---
    int gridX = 50;
    int gridY = 150;
    int gridWidth = 600;
    int gridHeight = 400;
    ui::TrackerGrid* tracker = new ui::TrackerGrid(gridX, gridY, gridWidth, gridHeight, 4, 16);
    tracker->onCellActivated = [](int col, int row) {
         std::cout << "Activated cell: track " << col << ", step " << row << std::endl;
    };
    ui->addElement(std::shared_ptr<ui::UIElement>(tracker));
    
    // --- Shuttle Control (Play, Pause, Record) ---
    int shuttleX = 50;
    int shuttleY = gridY + gridHeight + 20;
    int shuttleWidth = 400;
    int shuttleHeight = 60;
    ui::ShuttleControl* shuttle = new ui::ShuttleControl(shuttleX, shuttleY, shuttleWidth, shuttleHeight);
    ui->addElement(std::shared_ptr<ui::UIElement>(shuttle));
    
    // --- Hotkey Registration ---
    // Ctrl+f: Focus the context menu and select "File".
    ui->assignHotKey(ctxMenu, "f", [ctxMenu, uiPtr = ui.get()](){
        ctxMenu->activeItemIndex = 0; // File is index 0.
        ctxMenu->expanded = true;
        uiPtr->setFocus(ctxMenu);
    });
    
    // Ctrl+q: Trigger the Quit action.
    ui->assignHotKey(ctxMenu, "q", [ctxMenu]() {
        ctxMenu->activeItemIndex = 0; // File is index 0.
        ctxMenu->expanded = true;
        if (!ctxMenu->items.empty() && ctxMenu->items[0].subCallbacks.size() >= 3)
            ctxMenu->items[0].subCallbacks[2](); // "Quit" callback.
    });
    
    // Ctrl+p: Focus the shuttle control.
    ui->assignHotKey(shuttle, "p", [shuttle, uiPtr = ui.get()](){
        uiPtr->setFocus(shuttle);
        std::cout << "Shuttle control focused." << std::endl;
    });
    
    // (Optional hotkeys for other elements could be added similarly.)
    
    ui->run();
    return 0;
}
