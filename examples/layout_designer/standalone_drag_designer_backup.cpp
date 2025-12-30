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
        // Bottom-right corner resize handle (15x15 pixels)
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
    
    static constexpr int WINDOW_WIDTH = 1300;
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
        
        // Load embedded font
        SDL_RWops* fontRW = SDL_RWFromConstMem(ui::embedded::console_font_data, ui::embedded::console_font_size);
        if (fontRW) {
            font = TTF_OpenFontRW(fontRW, 1, 10); // Reduced from 14 to 10
            if (font) {
                std::cout << "Loaded embedded font successfully" << std::endl;
            } else {
                std::cout << "Failed to load embedded font: " << TTF_GetError() << std::endl;
            }
        } else {
            std::cout << "Failed to create font RWops" << std::endl;
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
        rects.emplace_back("Window", x, y, 1024, 600, SDL_Color{128, 128, 128, 128}); // Semi-transparent gray
        count++;
    }
    
    void handleClick(int x, int y) {
        // Check toolbar buttons - expanded for all elements
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
        
        // Check rectangles for resizing first (resize handles have priority)
        for (auto& rect : rects) {
            if (rect.isOnResizeHandle(x, y)) {
                draggedRect = &rect;
                rect.isResizing = true;
                return;
            }
        }
        
        // Check rectangles for dragging
        for (auto& rect : rects) {
            if (rect.contains(x, y)) {
                draggedRect = &rect;
                rect.isDragging = true;
                rect.dragOffsetX = x - rect.x;
                rect.dragOffsetY = y - rect.y;
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
                // Resize the rectangle
                int newWidth = x - draggedRect->x;
                int newHeight = y - draggedRect->y;
                
                // Minimum size constraints
                draggedRect->width = std::max(30, newWidth);
                draggedRect->height = std::max(20, newHeight);
            } else if (draggedRect->isDragging) {
                // Move the rectangle
                draggedRect->x = x - draggedRect->dragOffsetX;
                draggedRect->y = y - draggedRect->dragOffsetY;
                
                // Keep in bounds
                draggedRect->x = std::max(0, std::min(draggedRect->x, WINDOW_WIDTH - draggedRect->width));
                draggedRect->y = std::max(TOOLBAR_HEIGHT, std::min(draggedRect->y, WINDOW_HEIGHT - draggedRect->height));
            }
        }
    }
    
    void showLayout() {
        std::cout << "\n=== DRAG & DROP LAYOUT ===\n";
        
        // Find window containers for relative positioning
        std::vector<DragRect*> windows;
        for (auto& rect : rects) {
            if (rect.type == "Window") {
                windows.push_back(&rect);
            }
        }
        
        for (size_t i = 0; i < rects.size(); ++i) {
            const auto& rect = rects[i];
            std::cout << "Element " << (i + 1) << ": " << rect.type 
                      << " at (" << rect.x << ", " << rect.y << ") "
                      << "size " << rect.width << "x" << rect.height;
            
            // Check if element is inside any window
            if (rect.type != "Window") {
                for (const auto* window : windows) {
                    if (rect.x >= window->x && rect.y >= window->y &&
                        rect.x + rect.width <= window->x + window->width &&
                        rect.y + rect.height <= window->y + window->height) {
                        int relX = rect.x - window->x;
                        int relY = rect.y - window->y;
                        std::cout << " [Window-relative: (" << relX << ", " << relY << ")]";
                        break;
                    }
                }
            }
            std::cout << "\n";
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
        
        // Draw toolbar background
        SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
        SDL_Rect toolbar = {0, 0, WINDOW_WIDTH, TOOLBAR_HEIGHT};
        SDL_RenderFillRect(renderer, &toolbar);
        
        // Draw toolbar buttons with text labels
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
            // Draw button background
            SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255);
            SDL_Rect btn = {buttonPositions[i], 5, buttonWidths[i], 25};
            SDL_RenderFillRect(renderer, &btn);
            
            // Draw button border
            SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
            SDL_RenderDrawRect(renderer, &btn);
            
            // Draw button text
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
        
        // Draw design area border
        SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
        SDL_Rect border = {0, TOOLBAR_HEIGHT, WINDOW_WIDTH, WINDOW_HEIGHT - TOOLBAR_HEIGHT};
        SDL_RenderDrawRect(renderer, &border);
        
        // Draw all rectangles - Windows first, then other elements on top
        // First pass: Draw windows
        for (const auto& rect : rects) {
            if (rect.type == "Window") {
                SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
                SDL_SetRenderDrawColor(renderer, rect.color.r, rect.color.g, rect.color.b, rect.color.a);
                SDL_Rect sdlRect = {rect.x, rect.y, rect.width, rect.height};
                SDL_RenderFillRect(renderer, &sdlRect);
                
                // Draw thicker border for window
                SDL_SetRenderDrawColor(renderer, 64, 64, 64, 255);
                for (int i = 0; i < 3; ++i) {
                    SDL_Rect borderRect = {rect.x - i, rect.y - i, rect.width + 2*i, rect.height + 2*i};
                    SDL_RenderDrawRect(renderer, &borderRect);
                }
                SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
                
                // Draw text for window
                if (font) {
                    SDL_Color textColor = {0, 0, 0, 255};
                    SDL_Surface* textSurface = TTF_RenderText_Solid(font, rect.type.c_str(), textColor);
                    if (textSurface) {
                        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
                        if (textTexture) {
                            int textW = textSurface->w;
                            int textH = textSurface->h;
                            SDL_Rect textRect = {
                                rect.x + 10, rect.y + 10, textW, textH
                            };
                            SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);
                            SDL_DestroyTexture(textTexture);
                        }
                        SDL_FreeSurface(textSurface);
                    }
                }
            }
        }
        
        // Second pass: Draw all other elements on top
        for (const auto& rect : rects) {
            if (rect.type != "Window") {
                // Normal element rendering
                SDL_SetRenderDrawColor(renderer, rect.color.r, rect.color.g, rect.color.b, rect.color.a);
                SDL_Rect sdlRect = {rect.x, rect.y, rect.width, rect.height};
                SDL_RenderFillRect(renderer, &sdlRect);
                
                // Draw border
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderDrawRect(renderer, &sdlRect);
                
                // Draw resize handle
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                SDL_Rect resizeHandle = {rect.x + rect.width - 15, rect.y + rect.height - 15, 15, 15};
                SDL_RenderFillRect(renderer, &resizeHandle);
            
            // Draw text using embedded font
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
            } else {
                // Fallback: simple text blocks
                const char* text = rect.type.c_str();
                int textX = rect.x + 5;
                int textY = rect.y + 5;
                
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 200);
                SDL_Rect textBg = {textX - 2, textY - 2, (int)strlen(text) * 8 + 4, 14};
                SDL_RenderFillRect(renderer, &textBg);
                
                for (int i = 0; text[i] && i < 10; ++i) {
                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                    SDL_Rect charRect = {textX + i * 8, textY, 6, 10};
                    SDL_RenderFillRect(renderer, &charRect);
                }
            }
        }
        
        SDL_RenderPresent(renderer);
    }
    
    void run() {
        std::cout << "Comprehensive Drag & Drop Layout Designer\n";
        std::cout << "Available Elements: Button, Label, TextBox, Canvas, ProgressBar, Slider, CheckBox, ListView, OptionSelect, Image, Window\n";
        std::cout << "- Click toolbar buttons to add elements\n";
        std::cout << "- Add Window (1024x600) to help align elements within app boundaries\n";
        std::cout << "- Drag rectangles to move them\n";
        std::cout << "- Drag RED CORNER (bottom-right) to resize\n";
        std::cout << "- Click 'Show Layout' to export positions (shows window-relative coords)\n";
        std::cout << "- Click 'Clear All' to reset\n\n";
        
        SDL_Event e;
        while (running) {
            while (SDL_PollEvent(&e)) {
                if (e.type == SDL_QUIT) {
                    running = false;
                } else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
                    handleClick(e.button.x, e.button.y);
                } else if (e.type == SDL_MOUSEBUTTONUP && e.button.button == SDL_BUTTON_LEFT) {
                    handleMouseUp();
                } else if (e.type == SDL_MOUSEMOTION) {
                    handleMouseMove(e.motion.x, e.motion.y);
                }
            }
            
            render();
            SDL_Delay(16); // ~60 FPS
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
