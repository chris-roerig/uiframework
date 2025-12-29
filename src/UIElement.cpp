#include "uiframework/UIElements/UIElement.h"
#include "uiframework/Resources/SDLTextureRAII.h"
#include "uiframework/ErrorHandling.h"
#include <SDL2/SDL_ttf.h>

namespace ui {

TextCacheEntry* UIElement::getCachedText(const std::string& key, const std::string& text, 
                                         SDL_Color color, SDL_Renderer* renderer, TTF_Font* font) const {
    auto it = textCache.find(key);
    
    // Check if we have a valid cached entry
    if (it != textCache.end() && it->second) {
        auto& entry = it->second;
        if (entry->text == text && 
            entry->color.r == color.r && entry->color.g == color.g && 
            entry->color.b == color.b && entry->color.a == color.a) {
            return entry.get();
        }
    }
    
    // Create new texture
    if (!renderer || !font || text.empty()) {
        return nullptr;
    }
    
    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
    if (!ErrorHandling::validateSurface(surface)) {
        return nullptr;
    }
    
    SDLTextureRAII textureRAII(renderer, surface);
    SDL_FreeSurface(surface);
    
    if (!textureRAII) {
        return nullptr;
    }
    
    // Create new cache entry
    auto entry = std::make_unique<TextCacheEntry>();
    entry->texture = textureRAII.get();
    entry->width = surface->w;
    entry->height = surface->h;
    entry->text = text;
    entry->color = color;
    
    // Store in cache and return pointer
    TextCacheEntry* result = entry.get();
    textCache[key] = std::move(entry);
    
    // Release ownership from RAII wrapper since TextCacheEntry will manage it
    const_cast<SDLTextureRAII&>(textureRAII) = SDLTextureRAII(nullptr, nullptr);
    
    return result;
}

void UIElement::invalidateTextCache() const {
    textCache.clear();
}

} // namespace ui
