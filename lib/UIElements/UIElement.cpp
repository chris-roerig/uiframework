#include "UIElement.h"
#include "../ErrorHandling.h"
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
    
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!ErrorHandling::validateTexture(texture)) {
        SDL_FreeSurface(surface);
        return nullptr;
    }
    
    // Create new cache entry
    auto entry = std::make_unique<TextCacheEntry>();
    entry->texture = texture;
    entry->width = surface->w;
    entry->height = surface->h;
    entry->text = text;
    entry->color = color;
    
    SDL_FreeSurface(surface);
    
    // Store in cache
    TextCacheEntry* result = entry.get();
    textCache[key] = std::move(entry);
    
    return result;
}

void UIElement::invalidateTextCache() const {
    textCache.clear();
}

} // namespace ui
