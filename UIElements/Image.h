#pragma once

#include "UIElement.h"
#include <SDL2/SDL.h>
#include <string>

namespace ui {

class Image : public UIElement {
public:
    // Constructs an Image element.
    // x, y: Position on the screen.
    // w, h: Desired dimensions (used if stretching is enabled).
    // path: File path to the image (supports jpg, png, gif, bmp).
    // renderer: SDL_Renderer pointer required to create the texture.
    // stretch_: When false (default), the image is rendered at its natural dimensions.
    Image(int x_, int y_, int w_, int h_, const std::string &path, SDL_Renderer* renderer, bool stretch_ = false);
    
    // Constructor for loading an image from binary data.
    // create binary headers with: xxd -i my_image.png > my_image.h
    // data: pointer to the embedded image data.
    // dataSize: size of the image data.
    Image(int x_, int y_, int w_, int h_, const unsigned char* data, size_t dataSize, SDL_Renderer* renderer, bool stretch_ = false);

    virtual ~Image();

    // Render the image.
    virtual void render(SDL_Renderer* renderer) override;

    // Images are non-interactive by default.
    virtual bool isInteractive() const override;

private:
    std::string filePath;
    bool stretch;
    SDL_Texture* texture;
    int naturalWidth;
    int naturalHeight;
};

} // namespace ui
