#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <vector>
#include <string>
#include "uiframework/Resources/EmbeddedFont.h"

struct DragRect {
    std::string type;
    int x, y, width, height;
    SDL_Color color;
    bool isDragging = false;
    bool isResizing = false;
    int dragOffsetX = 0, dragOffsetY = 0;
    
    DragRect(const std::string& t, int px, int py, int w, int h, SDL_Color c)
        : type(t), x(px), y(py), width(w), height(h), color(c) {}
        
    bool contains(int mx, int my) const {
        return mx >= x && mx < x + width && my >= y && my < y + height;
    }
    
    bool isOnResizeHandle(int mx, int my) const {
        return mx >= x + width - 15 && mx < x + width &&
               my >= y + height - 15 && my < y + height;
    }
};

class StandaloneDragDesigner {
private:
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    TTF_Font* font = nullptr;
    std::vector<DragRect> rects;
    DragRect* draggedRect = nullptr;
    bool running = true;
    
    static constexpr int WINDOW_WIDTH = 1400;
    static constexpr int WINDOW_HEIGHT = 800;
    static constexpr int TOOLBAR_HEIGHT = 50;
    
public:
    bool init() {
        if (SDL_Init(SDL_INIT_VIDEO) < 0) return false;
        if (TTF_Init() == -1) return false;
        
        window = SDL_CreateWindow("Drag & Drop Layout Designer", 
                                 SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                 WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
        if (!window) return false;
        
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if (!renderer) return false;
        
        SDL_RWops* fontRW = SDL_RWFromConstMem(ui::embedded::console_font_data, ui::embedded::console_font_size);
        if (fontRW) {
            font = TTF_OpenFontRW(fontRW, 1, 10);
            if (font) {
                std::cout << "Loaded embedded font successfully" << std::endl;
            }
        }
        
        return true;
    }
    
    void cleanup() {
        if (font) TTF_CloseFont(font);
        if (renderer) SDL_DestroyRenderer(renderer);
        if (window) SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
    }
    
    void addButton() {
        static int count = 0;
        int x = 50 + (count * 20) % 400;
        int y = TOOLBAR_HEIGHT + 50 + (count * 20) % 300;
        rects.emplace_back("Button", x, y, 100, 30, SDL_Color{100, 150, 255, 255});
        count++;
    }
    
    void addLabel() {
        static int count = 0;
        int x = 100 + (count * 25) % 400;
        int y = TOOLBAR_HEIGHT + 100 + (count * 25) % 300;
        rects.emplace_back("Label", x, y, 80, 25, SDL_Color{150, 255, 150, 255});
        count++;
    }
    
    void addTextBox() {
        static int count = 0;
        int x = 150 + (count * 30) % 400;
        int y = TOOLBAR_HEIGHT + 150 + (count * 30) % 300;
        rects.emplace_back("TextBox", x, y, 120, 25, SDL_Color{255, 200, 100, 255});
        count++;
    }
    
    void addCanvas() {
        static int count = 0;
        int x = 200 + (count * 35) % 400;
        int y = TOOLBAR_HEIGHT + 200 + (count * 35) % 300;
        rects.emplace_back("Canvas", x, y, 100, 80, SDL_Color{200, 150, 200, 255});
        count++;
    }
    
    void addProgressBar() {
        static int count = 0;
        int x = 250 + (count * 40) % 400;
        int y = TOOLBAR_HEIGHT + 250 + (count * 40) % 300;
        rects.emplace_back("ProgressBar", x, y, 150, 25, SDL_Color{255, 150, 50, 255});
        count++;
    }
    
    void addSlider() {
        static int count = 0;
        int x = 300 + (count * 45) % 400;
        int y = TOOLBAR_HEIGHT + 300 + (count * 45) % 300;
        rects.emplace_back("Slider", x, y, 120, 20, SDL_Color{50, 200, 200, 255});
        count++;
    }
    
    void addCheckBox() {
        static int count = 0;
        int x = 350 + (count * 50) % 400;
        int y = TOOLBAR_HEIGHT + 350 + (count * 50) % 250;
        rects.emplace_back("CheckBox", x, y, 20, 20, SDL_Color{255, 100, 100, 255});
        count++;
    }
    
    void addListView() {
        static int count = 0;
        int x = 400 + (count * 55) % 400;
        int y = TOOLBAR_HEIGHT + 400 + (count * 55) % 200;
        rects.emplace_back("ListView", x, y, 120, 100, SDL_Color{100, 255, 200, 255});
        count++;
    }
    
    void addOptionSelect() {
        static int count = 0;
        int x = 450 + (count * 60) % 400;
        int y = TOOLBAR_HEIGHT + 450 + (count * 60) % 150;
        rects.emplace_back("OptionSelect", x, y, 100, 25, SDL_Color{200, 100, 255, 255});
        count++;
    }
    
    void addImage() {
        static int count = 0;
        int x = 500 + (count * 65) % 400;
        int y = TOOLBAR_HEIGHT + 500 + (count * 65) % 100;
        rects.emplace_back("Image", x, y, 80, 60, SDL_Color{255, 255, 100, 255});
        count++;
    }
    
    void addWindow() {
        static int count = 0;
        int x = 100 + (count * 50) % 200;
        int y = TOOLBAR_HEIGHT + 50 + (count * 50) % 100;
        rects.emplace_back("Window", x, y, 1024, 600, SDL_Color{128, 128, 128, 128});
        count++;
    }
    
    void handleClick(int x, int y, bool shiftPressed = false) {
        if (y < TOOLBAR_HEIGHT) {
            if (x >= 10 && x < 90) addButton();
            else if (x >= 95 && x < 165) addLabel();
            else if (x >= 170 && x < 250) addTextBox();
            else if (x >= 255 && x < 325) addCanvas();
            else if (x >= 330 && x < 420) addProgressBar();
            else if (x >= 425 && x < 485) addSlider();
            else if (x >= 490 && x < 570) addCheckBox();
            else if (x >= 575 && x < 645) addListView();
            else if (x >= 650 && x < 740) addOptionSelect();
            else if (x >= 745 && x < 805) addImage();
            else if (x >= 810 && x < 880) addWindow();
            else if (x >= 885 && x < 965) showLayout();
            else if (x >= 970 && x < 1050) clearAll();
            return;
        }
        
        // Shift+click removes element
        if (shiftPressed) {
            for (auto it = rects.rbegin(); it != rects.rend(); ++it) {
                if (it->contains(x, y)) {
                    rects.erase(std::next(it).base());
                    std::cout << "Removed " << it->type << " element\n";
                    return;
                }
            }
            return;
        }
        
        // Check for resize handle first (search from top to bottom)
        for (auto it = rects.rbegin(); it != rects.rend(); ++it) {
            if (it->isOnResizeHandle(x, y)) {
                draggedRect = &(*it);
                it->isResizing = true;
                return;
            }
        }
        
        // Check for dragging (search from top to bottom)
        for (auto it = rects.rbegin(); it != rects.rend(); ++it) {
            if (it->contains(x, y)) {
                draggedRect = &(*it);
                it->isDragging = true;
                it->dragOffsetX = x - it->x;
                it->dragOffsetY = y - it->y;
                break;
            }
        }
    }
    
    void handleMouseUp() {
        if (draggedRect) {
            draggedRect->isDragging = false;
            draggedRect->isResizing = false;
            draggedRect = nullptr;
        }
    }
    
    void handleMouseMove(int x, int y) {
        if (draggedRect) {
            if (draggedRect->isResizing) {
                int newWidth = x - draggedRect->x;
                int newHeight = y - draggedRect->y;
                draggedRect->width = std::max(30, newWidth);
                draggedRect->height = std::max(20, newHeight);
            } else if (draggedRect->isDragging) {
                draggedRect->x = x - draggedRect->dragOffsetX;
                draggedRect->y = y - draggedRect->dragOffsetY;
                draggedRect->x = std::max(0, std::min(draggedRect->x, WINDOW_WIDTH - draggedRect->width));
                draggedRect->y = std::max(TOOLBAR_HEIGHT, std::min(draggedRect->y, WINDOW_HEIGHT - draggedRect->height));
            }
        }
    }
    
    void showLayout() {
        std::cout << "\n=== DRAG & DROP LAYOUT ===\n";
        for (size_t i = 0; i < rects.size(); ++i) {
            const auto& rect = rects[i];
            std::cout << "Element " << (i + 1) << ": " << rect.type 
                      << " at (" << rect.x << ", " << rect.y << ") "
                      << "size " << rect.width << "x" << rect.height << "\n";
        }
        std::cout << "Total: " << rects.size() << " elements\n\n";
    }
    
    void clearAll() {
        rects.clear();
        draggedRect = nullptr;
        std::cout << "All elements cleared!\n";
    }
    
    void render() {
        SDL_SetRenderDrawColor(renderer, 240, 240, 240, 255);
        SDL_RenderClear(renderer);
        
        SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
        SDL_Rect toolbar = {0, 0, WINDOW_WIDTH, TOOLBAR_HEIGHT};
        SDL_RenderFillRect(renderer, &toolbar);
        
        const char* buttonLabels[] = {
            "Button", "Label", "TextBox", "Canvas", "ProgressBar", "Slider", 
            "CheckBox", "ListView", "OptionSelect", "Image", "Window", "Show Layout", "Clear All"
        };
        int buttonPositions[] = {
            10, 95, 170, 255, 330, 425, 490, 575, 650, 745, 810, 885, 970
        };
        int buttonWidths[] = {
            80, 70, 80, 70, 90, 60, 80, 70, 90, 60, 70, 80, 80
        };
        
        for (int i = 0; i < 13; ++i) {
            SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255);
            SDL_Rect btn = {buttonPositions[i], 5, buttonWidths[i], 25};
            SDL_RenderFillRect(renderer, &btn);
            
            SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
            SDL_RenderDrawRect(renderer, &btn);
            
            if (font) {
                SDL_Color textColor = {0, 0, 0, 255};
                SDL_Surface* textSurface = TTF_RenderText_Solid(font, buttonLabels[i], textColor);
                if (textSurface) {
                    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
                    if (textTexture) {
                        int textW = textSurface->w;
                        int textH = textSurface->h;
                        SDL_Rect textRect = {
                            buttonPositions[i] + (buttonWidths[i] - textW) / 2,
                            10 + (15 - textH) / 2,
                            textW, textH
                        };
                        SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);
                        SDL_DestroyTexture(textTexture);
                    }
                    SDL_FreeSurface(textSurface);
                }
            }
        }
        
        SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
        SDL_Rect border = {0, TOOLBAR_HEIGHT, WINDOW_WIDTH, WINDOW_HEIGHT - TOOLBAR_HEIGHT};
        SDL_RenderDrawRect(renderer, &border);
        
        // Draw all rectangles - Windows first (background), then others on top
        for (const auto& rect : rects) {
            if (rect.type == "Window") {
                SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
                SDL_SetRenderDrawColor(renderer, rect.color.r, rect.color.g, rect.color.b, rect.color.a);
                SDL_Rect sdlRect = {rect.x, rect.y, rect.width, rect.height};
                SDL_RenderFillRect(renderer, &sdlRect);
                
                SDL_SetRenderDrawColor(renderer, 64, 64, 64, 255);
                for (int i = 0; i < 3; ++i) {
                    SDL_Rect borderRect = {rect.x - i, rect.y - i, rect.width + 2*i, rect.height + 2*i};
                    SDL_RenderDrawRect(renderer, &borderRect);
                }
                SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
                
                if (font) {
                    SDL_Color textColor = {0, 0, 0, 255};
                    SDL_Surface* textSurface = TTF_RenderText_Solid(font, rect.type.c_str(), textColor);
                    if (textSurface) {
                        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
                        if (textTexture) {
                            int textW = textSurface->w;
                            int textH = textSurface->h;
                            SDL_Rect textRect = {rect.x + 10, rect.y + 10, textW, textH};
                            SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);
                            SDL_DestroyTexture(textTexture);
                        }
                        SDL_FreeSurface(textSurface);
                    }
                }
            }
        }
        
        // Draw all other elements on top
        for (const auto& rect : rects) {
            if (rect.type != "Window") {
                SDL_SetRenderDrawColor(renderer, rect.color.r, rect.color.g, rect.color.b, rect.color.a);
                SDL_Rect sdlRect = {rect.x, rect.y, rect.width, rect.height};
                SDL_RenderFillRect(renderer, &sdlRect);
                
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderDrawRect(renderer, &sdlRect);
                
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                SDL_Rect resizeHandle = {rect.x + rect.width - 15, rect.y + rect.height - 15, 15, 15};
                SDL_RenderFillRect(renderer, &resizeHandle);
                
                if (font) {
                    SDL_Color textColor = {0, 0, 0, 255};
                    SDL_Surface* textSurface = TTF_RenderText_Solid(font, rect.type.c_str(), textColor);
                    if (textSurface) {
                        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
                        if (textTexture) {
                            int textW = textSurface->w;
                            int textH = textSurface->h;
                            SDL_Rect textRect = {
                                rect.x + (rect.width - textW) / 2,
                                rect.y + (rect.height - textH) / 2,
                                textW, textH
                            };
                            SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);
                            SDL_DestroyTexture(textTexture);
                        }
                        SDL_FreeSurface(textSurface);
                    }
                }
            }
        }
        
        SDL_RenderPresent(renderer);
    }
    
    void run() {
        std::cout << "Comprehensive Drag & Drop Layout Designer\n";
        std::cout << "Available Elements: Button, Label, TextBox, Canvas, ProgressBar, Slider, CheckBox, ListView, OptionSelect, Image, Window\n";
        std::cout << "- Click toolbar buttons to add elements\n";
        std::cout << "- Drag rectangles to move them\n";
        std::cout << "- Drag RED CORNER to resize\n";
        std::cout << "- SHIFT+Click to remove elements\n";
        std::cout << "- Add Window (1024x600) for app boundaries\n\n";
        
        SDL_Event e;
        while (running) {
            while (SDL_PollEvent(&e)) {
                if (e.type == SDL_QUIT) {
                    running = false;
                } else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
                    bool shiftPressed = (SDL_GetModState() & KMOD_SHIFT) != 0;
                    handleClick(e.button.x, e.button.y, shiftPressed);
                } else if (e.type == SDL_MOUSEBUTTONUP && e.button.button == SDL_BUTTON_LEFT) {
                    handleMouseUp();
                } else if (e.type == SDL_MOUSEMOTION) {
                    handleMouseMove(e.motion.x, e.motion.y);
                }
            }
            
            render();
            SDL_Delay(16);
        }
    }
};

int main() {
    StandaloneDragDesigner designer;
    
    if (!designer.init()) {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        return -1;
    }
    
    designer.run();
    designer.cleanup();
    
    return 0;
}
