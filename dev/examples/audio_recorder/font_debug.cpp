#include "uiframework/Resources/FontManager.h"
#include "uiframework/Resources/EmbeddedFont.h"
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

int main() {
    // Initialize SDL and TTF
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL_Init failed: " << SDL_GetError() << std::endl;
        return -1;
    }
    
    if (TTF_Init() == -1) {
        std::cerr << "TTF_Init failed: " << TTF_GetError() << std::endl;
        return -1;
    }
    
    std::cout << "SDL and TTF initialized successfully" << std::endl;
    std::cout << "Embedded font size: " << ui::embedded::console_font_size << " bytes" << std::endl;
    
    // Test FontManager
    auto& fontManager = ui::FontManager::getInstance();
    
    // Try to get default font (empty path = embedded font)
    TTF_Font* font = fontManager.getFont("", 16);
    
    if (font) {
        std::cout << "✓ Successfully loaded embedded font!" << std::endl;
        
        // Test rendering some text
        SDL_Color color = {255, 255, 255, 255};
        SDL_Surface* surface = TTF_RenderText_Solid(font, "Test Text", color);
        
        if (surface) {
            std::cout << "✓ Successfully rendered text surface!" << std::endl;
            std::cout << "  Surface size: " << surface->w << "x" << surface->h << std::endl;
            SDL_FreeSurface(surface);
        } else {
            std::cout << "✗ Failed to render text: " << TTF_GetError() << std::endl;
        }
        
    } else {
        std::cout << "✗ Failed to load embedded font" << std::endl;
    }
    
    // Try system font fallback
    font = fontManager.getFont("/System/Library/Fonts/Helvetica.ttc", 16);
    if (font) {
        std::cout << "✓ System font fallback works" << std::endl;
    } else {
        std::cout << "✗ System font fallback failed" << std::endl;
    }
    
    TTF_Quit();
    SDL_Quit();
    
    return 0;
}
