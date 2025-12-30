#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <fstream>
#include <sstream>
#include <ctime>
#include "uiframework/Resources/EmbeddedFont.h"

struct DesignElement {
    std::string type;
    std::string name;
    int x, y, width, height;
    SDL_Color color;
    bool isDragging = false;
    bool isResizing = false;
    int dragOffsetX = 0, dragOffsetY = 0;
    
    DesignElement(const std::string& t, const std::string& n, int px, int py, int w, int h, SDL_Color c)
        : type(t), name(n), x(px), y(py), width(w), height(h), color(c) {}
        
    bool contains(int mx, int my) const {
        return mx >= x && mx < x + width && my >= y && my < y + height;
    }
    
    bool isOnResizeHandle(int mx, int my) const {
        return mx >= x + width - 10 && mx < x + width &&
               my >= y + height - 10 && my < y + height;
    }
};

class KeyboardLayoutDesigner {
private:
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    TTF_Font* font = nullptr;
    std::vector<DesignElement> elements;
    DesignElement* draggedElement = nullptr;
    bool running = true;
    int windowWidth = 1024;
    int windowHeight = 600;
    
    // UI Framework element sizes (accurate to actual rendering)
    std::map<std::string, std::pair<int, int>> elementSizes = {
        {"Button", {100, 30}},
        {"Label", {80, 25}},
        {"TextBox", {120, 25}},
        {"Canvas", {100, 80}},
        {"ProgressBar", {150, 25}},
        {"Slider", {120, 20}},
        {"CheckBox", {20, 20}},
        {"ListView", {120, 100}},
        {"OptionSelect", {100, 25}},
        {"Image", {80, 60}},
        {"HSlider", {120, 20}},
        {"VSlider", {20, 120}},
        {"KnobSlider", {60, 60}},
        {"Sprite", {80, 60}},
        {"AnimatedSprite", {80, 60}},
        {"VirtualKeyboard", {300, 200}},
        {"CycleList", {150, 30}},
        {"ContextMenu", {150, 100}},
        {"VBoxLayout", {100, 150}},
        {"HBoxLayout", {150, 100}},
        {"GridLayout", {150, 150}},
        {"Modal", {200, 100}}
    };
    
    std::map<std::string, SDL_Color> elementColors = {
        {"Button", {100, 150, 255, 255}},
        {"Label", {150, 255, 150, 255}},
        {"TextBox", {255, 200, 100, 255}},
        {"Canvas", {200, 150, 200, 255}},
        {"ProgressBar", {255, 150, 50, 255}},
        {"Slider", {50, 200, 200, 255}},
        {"CheckBox", {255, 100, 100, 255}},
        {"ListView", {100, 255, 200, 255}},
        {"OptionSelect", {200, 100, 255, 255}},
        {"Image", {255, 255, 100, 255}},
        {"HSlider", {50, 200, 200, 255}},
        {"VSlider", {50, 200, 200, 255}},
        {"KnobSlider", {50, 200, 200, 255}},
        {"Sprite", {255, 180, 100, 255}},
        {"AnimatedSprite", {255, 120, 100, 255}},
        {"VirtualKeyboard", {180, 180, 180, 255}},
        {"CycleList", {100, 200, 255, 255}},
        {"ContextMenu", {220, 220, 220, 255}},
        {"VBoxLayout", {100, 100, 255, 255}},
        {"HBoxLayout", {100, 255, 100, 255}},
        {"GridLayout", {255, 100, 255, 255}},
        {"Modal", {200, 200, 200, 255}}
    };
    
    std::map<std::string, int> elementCounters;
    
public:
    bool init() {
        if (SDL_Init(SDL_INIT_VIDEO) < 0) return false;
        if (TTF_Init() == -1) return false;
        
        window = SDL_CreateWindow("Keyboard Layout Designer", 
                                 SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                 windowWidth, windowHeight, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
        if (!window) return false;
        
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if (!renderer) return false;
        
        SDL_RWops* fontRW = SDL_RWFromConstMem(ui::embedded::console_font_data, ui::embedded::console_font_size);
        if (fontRW) {
            font = TTF_OpenFontRW(fontRW, 1, 12);
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
    
    void loadLayout(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cout << "Error: Could not load " << filename << "\n";
            return;
        }
        
        clearAll();
        
        file >> windowWidth >> windowHeight;
        SDL_SetWindowSize(window, windowWidth, windowHeight);
        
        std::string type, name;
        int x, y, width, height;
        while (file >> type >> name >> x >> y >> width >> height) {
            auto color = elementColors[type];
            elements.emplace_back(type, name, x, y, width, height, color);
            
            // Update counter for this type
            std::string baseType = type;
            if (name.length() > type.length()) {
                std::string numStr = name.substr(type.length());
                if (!numStr.empty()) {
                    try {
                        int num = std::stoi(numStr);
                        elementCounters[baseType] = std::max(elementCounters[baseType], num);
                    } catch (const std::invalid_argument&) {
                        // Name doesn't follow TypeN pattern, skip counter update
                    }
                }
            }
        }
        file.close();
        std::cout << "Layout loaded from " << filename << " (" << elements.size() << " elements)\n";
    }
    
    void saveLayout(const std::string& filename) {
        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cout << "Error: Could not save to " << filename << "\n";
            return;
        }
        
        file << windowWidth << " " << windowHeight << "\n";
        for (const auto& elem : elements) {
            file << elem.type << " " << elem.name << " " 
                 << elem.x << " " << elem.y << " " 
                 << elem.width << " " << elem.height << "\n";
        }
        file.close();
        std::cout << "Layout saved to " << filename << "\n";
    }
    
    void showHelp() {
        std::cout << "\n=== KEYBOARD LAYOUT DESIGNER HELP ===\n";
        std::cout << "UI Elements:\n";
        std::cout << "  Q=Button       W=Label        E=TextBox      R=Canvas\n";
        std::cout << "  T=ProgressBar  Y=Slider       U=CheckBox     I=ListView\n";
        std::cout << "  O=OptionSelect P=Image        A=HSlider      S=VSlider\n";
        std::cout << "  D=KnobSlider   F=Sprite       G=AnimatedSprite\n";
        std::cout << "  H=VirtualKeyboard  J=ContextMenu  K=VBoxLayout\n";
        std::cout << "  L=HBoxLayout   Z=GridLayout   X=Modal   C=CycleList\n\n";
        std::cout << "Actions:\n";
        std::cout << "  SPACE = Export layout to console\n";
        std::cout << "  C = Clear all elements\n";
        std::cout << "  Cmd+S = Save layout to file\n";
        std::cout << "  Shift+/ = Show this help\n";
        std::cout << "  Shift+Click = Remove element\n\n";
        std::cout << "Mouse:\n";
        std::cout << "  Drag element = Move (snaps to 10x10 grid)\n";
        std::cout << "  Drag red corner = Resize (snaps to grid)\n";
        std::cout << "=====================================\n\n";
    }
    
    int snapToGrid(int value) {
        return ((value + 5) / 10) * 10;  // Round to nearest 10
    }
    
    void addElement(const std::string& type) {
        static int offsetCounter = 0;
        auto size = elementSizes[type];
        auto color = elementColors[type];
        
        elementCounters[type]++;
        std::string name = type + std::to_string(elementCounters[type]);
        
        int x = snapToGrid(50 + (offsetCounter * 20) % (windowWidth - size.first - 100));
        int y = snapToGrid(50 + (offsetCounter * 20) % (windowHeight - size.second - 100));
        
        elements.emplace_back(type, name, x, y, size.first, size.second, color);
        offsetCounter++;
        
        std::cout << "Added " << name << " (" << size.first << "x" << size.second << ") at (" << x << ", " << y << ")\n";
    }
    
    void handleKeyPress(SDL_Keycode key, bool cmdPressed = false, bool shiftPressed = false) {
        if (cmdPressed && key == SDLK_s) {
            std::string filename = "layout_" + std::to_string(time(nullptr)) + ".txt";
            saveLayout(filename);
            return;
        }
        
        if (shiftPressed && key == SDLK_SLASH) {
            showHelp();
            return;
        }
        
        switch (key) {
            case SDLK_q: addElement("Button"); break;
            case SDLK_w: addElement("Label"); break;
            case SDLK_e: addElement("TextBox"); break;
            case SDLK_r: addElement("Canvas"); break;
            case SDLK_t: addElement("ProgressBar"); break;
            case SDLK_y: addElement("Slider"); break;
            case SDLK_u: addElement("CheckBox"); break;
            case SDLK_i: addElement("ListView"); break;
            case SDLK_o: addElement("OptionSelect"); break;
            case SDLK_p: addElement("Image"); break;
            case SDLK_a: addElement("HSlider"); break;
            case SDLK_s: addElement("VSlider"); break;
            case SDLK_d: addElement("KnobSlider"); break;
            case SDLK_f: addElement("Sprite"); break;
            case SDLK_g: addElement("AnimatedSprite"); break;
            case SDLK_h: addElement("VirtualKeyboard"); break;
            case SDLK_j: addElement("ContextMenu"); break;
            case SDLK_k: addElement("VBoxLayout"); break;
            case SDLK_c: addElement("CycleList"); break;
            case SDLK_l: addElement("HBoxLayout"); break;
            case SDLK_z: addElement("GridLayout"); break;
            case SDLK_x: addElement("Modal"); break;
            case SDLK_SPACE: printLayout(); break;
            case SDLK_c: clearAll(); break;
        }
    }
    
    void handleClick(int x, int y, bool shiftPressed = false) {
        if (shiftPressed) {
            for (auto it = elements.rbegin(); it != elements.rend(); ++it) {
                if (it->contains(x, y)) {
                    std::cout << "Removed " << it->name << "\n";
                    elements.erase(std::next(it).base());
                    return;
                }
            }
            return;
        }
        
        for (auto it = elements.rbegin(); it != elements.rend(); ++it) {
            if (it->isOnResizeHandle(x, y)) {
                draggedElement = &(*it);
                it->isResizing = true;
                return;
            }
        }
        
        for (auto it = elements.rbegin(); it != elements.rend(); ++it) {
            if (it->contains(x, y)) {
                draggedElement = &(*it);
                it->isDragging = true;
                it->dragOffsetX = x - it->x;
                it->dragOffsetY = y - it->y;
                break;
            }
        }
    }
    
    void handleMouseUp() {
        if (draggedElement) {
            draggedElement->isDragging = false;
            draggedElement->isResizing = false;
            draggedElement = nullptr;
        }
    }
    
    void handleMouseMove(int x, int y) {
        if (draggedElement) {
            if (draggedElement->isResizing) {
                int newWidth = snapToGrid(x - draggedElement->x);
                int newHeight = snapToGrid(y - draggedElement->y);
                draggedElement->width = std::max(20, newWidth);
                draggedElement->height = std::max(20, newHeight);
            } else if (draggedElement->isDragging) {
                int newX = snapToGrid(x - draggedElement->dragOffsetX);
                int newY = snapToGrid(y - draggedElement->dragOffsetY);
                draggedElement->x = std::max(0, std::min(newX, windowWidth - draggedElement->width));
                draggedElement->y = std::max(0, std::min(newY, windowHeight - draggedElement->height));
            }
        }
    }
    
    void printLayout() {
        std::cout << "\n=== LAYOUT EXPORT ===\n";
        std::cout << "Window Size: " << windowWidth << "x" << windowHeight << "\n\n";
        
        for (size_t i = 0; i < elements.size(); ++i) {
            const auto& elem = elements[i];
            std::cout << "Element " << (i + 1) << ": " << elem.name 
                      << " at (" << elem.x << ", " << elem.y << ") "
                      << "size " << elem.width << "x" << elem.height << "\n";
        }
        std::cout << "Total: " << elements.size() << " elements\n";
        std::cout << "====================\n\n";
    }
    
    void clearAll() {
        elements.clear();
        elementCounters.clear();
        draggedElement = nullptr;
        std::cout << "All elements cleared!\n";
    }
    
    void render() {
        SDL_GetWindowSize(window, &windowWidth, &windowHeight);
        
        SDL_SetRenderDrawColor(renderer, 245, 245, 245, 255);
        SDL_RenderClear(renderer);
        
        // Draw grid dots every 10x10 pixels
        SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
        for (int x = 0; x < windowWidth; x += 10) {
            for (int y = 0; y < windowHeight; y += 10) {
                SDL_RenderDrawPoint(renderer, x, y);
            }
        }
        
        // Draw elements
        for (const auto& elem : elements) {
            SDL_SetRenderDrawColor(renderer, elem.color.r, elem.color.g, elem.color.b, elem.color.a);
            SDL_Rect rect = {elem.x, elem.y, elem.width, elem.height};
            SDL_RenderFillRect(renderer, &rect);
            
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderDrawRect(renderer, &rect);
            
            // Resize handle
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            SDL_Rect handle = {elem.x + elem.width - 10, elem.y + elem.height - 10, 10, 10};
            SDL_RenderFillRect(renderer, &handle);
            
            // Draw text label with numbered name
            if (font) {
                SDL_Color textColor = {0, 0, 0, 255};
                SDL_Surface* textSurface = TTF_RenderText_Solid(font, elem.name.c_str(), textColor);
                if (textSurface) {
                    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
                    if (textTexture) {
                        int textW = textSurface->w;
                        int textH = textSurface->h;
                        SDL_Rect textRect = {
                            elem.x + (elem.width - textW) / 2,
                            elem.y + (elem.height - textH) / 2,
                            textW, textH
                        };
                        SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);
                        SDL_DestroyTexture(textTexture);
                    }
                    SDL_FreeSurface(textSurface);
                }
            }
        }
        
        SDL_RenderPresent(renderer);
    }
    
    void run() {
        std::cout << "Keyboard Layout Designer Started\n";
        std::cout << "Press Shift+/ for help\n\n";
        showHelp();
        
        SDL_Event e;
        while (running) {
            while (SDL_PollEvent(&e)) {
                if (e.type == SDL_QUIT) {
                    running = false;
                } else if (e.type == SDL_KEYDOWN) {
                    bool cmdPressed = (SDL_GetModState() & KMOD_GUI) != 0;
                    bool shiftPressed = (SDL_GetModState() & KMOD_SHIFT) != 0;
                    handleKeyPress(e.key.keysym.sym, cmdPressed, shiftPressed);
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

int main(int argc, char* argv[]) {
    KeyboardLayoutDesigner designer;
    
    if (!designer.init()) {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        return -1;
    }
    
    // Load layout file if provided as argument
    if (argc > 1) {
        designer.loadLayout(argv[1]);
    }
    
    designer.run();
    designer.cleanup();
    
    return 0;
}
