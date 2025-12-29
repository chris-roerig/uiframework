#pragma once
#include <SDL2/SDL.h>

namespace ui {

/**
 * @brief RAII wrapper for SDL_Texture to ensure automatic cleanup
 */
class SDLTextureRAII {
private:
    SDL_Texture* texture;

public:
    /**
     * @brief Create texture from surface with automatic cleanup
     * @param renderer SDL renderer
     * @param surface SDL surface to convert
     */
    SDLTextureRAII(SDL_Renderer* renderer, SDL_Surface* surface) 
        : texture(renderer && surface ? SDL_CreateTextureFromSurface(renderer, surface) : nullptr) {}
    
    /**
     * @brief Destructor automatically cleans up texture
     */
    ~SDLTextureRAII() { 
        if (texture) {
            SDL_DestroyTexture(texture);
        }
    }
    
    // Non-copyable
    SDLTextureRAII(const SDLTextureRAII&) = delete;
    SDLTextureRAII& operator=(const SDLTextureRAII&) = delete;
    
    // Movable
    SDLTextureRAII(SDLTextureRAII&& other) noexcept : texture(other.texture) {
        other.texture = nullptr;
    }
    
    SDLTextureRAII& operator=(SDLTextureRAII&& other) noexcept {
        if (this != &other) {
            if (texture) SDL_DestroyTexture(texture);
            texture = other.texture;
            other.texture = nullptr;
        }
        return *this;
    }
    
    /**
     * @brief Get the underlying SDL_Texture pointer
     * @return SDL_Texture* or nullptr if creation failed
     */
    SDL_Texture* get() const { return texture; }
    
    /**
     * @brief Check if texture was created successfully
     * @return true if texture is valid
     */
    operator bool() const { return texture != nullptr; }
};

} // namespace ui
