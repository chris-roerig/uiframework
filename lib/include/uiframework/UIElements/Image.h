#pragma once

#include "UIElement.h"
#include "ScalingMode.h"
#include <SDL2/SDL.h>
#include <string>
#include <memory>
#include <mutex>

namespace ui {

class Image : public UIElement {
private:
    std::string filePath;
    std::vector<unsigned char> imageData; // For embedded images
    bool stretch;
    SDL_Texture* texture = nullptr;
    int naturalWidth = 0;
    int naturalHeight = 0;
    bool isDataImage = false; // true if loaded from data, false if from file
    mutable std::mutex textureMutex; // Protects texture operations
    
    // Rotation and scaling state
    double rotationAngle = 0.0; // Rotation angle in degrees
    ScalingMode scalingMode = ScalingMode::Stretch; // Default to stretch for backward compatibility
    SDL_Point rotationCenter = {-1, -1}; // -1,-1 means use center of image
    
    // Real-time queued loading
    std::string queuedImagePath;
    std::atomic<bool> hasQueuedPath{false};
    
    void loadFromFile(SDL_Renderer* renderer, const std::string& path);
    void loadFromData(SDL_Renderer* renderer, const unsigned char* data, size_t dataSize);
    void cleanup();
    
    // Helper method to calculate destination rectangle based on scaling mode
    SDL_Rect calculateDestRect(int imageWidth, int imageHeight) const;

public:
    // Constructor for loading from file path
    Image(int x_, int y_, int w_, int h_, const std::string &path, bool stretch_ = false);
    
    // Constructor for loading from binary data
    Image(int x_, int y_, int w_, int h_, const unsigned char* data, size_t dataSize, bool stretch_ = false);

    virtual ~Image() override;
    
    // Non-copyable but movable
    Image(const Image&) = delete;
    Image& operator=(const Image&) = delete;
    Image(Image&& other) noexcept;
    Image& operator=(Image&& other) noexcept;

protected:
    void renderImpl(const RenderContext& ctx) override;
    
public:
    // Image-specific methods
    void setStretch(bool shouldStretch) { stretch = shouldStretch; }
    bool isStretched() const { return stretch; }
    int getNaturalWidth() const { return naturalWidth; }
    int getNaturalHeight() const { return naturalHeight; }
    bool isLoaded() const { return texture != nullptr; }
    
    // Rotation and scaling methods
    void setRotation(double angle) { rotationAngle = angle; }
    double getRotation() const { return rotationAngle; }
    void setRotationCenter(int centerX, int centerY) { rotationCenter = {centerX, centerY}; }
    void setRotationCenter(const SDL_Point& center) { rotationCenter = center; }
    SDL_Point getRotationCenter() const { return rotationCenter; }
    void resetRotationCenter() { rotationCenter = {-1, -1}; } // Use image center
    
    void setScalingMode(ScalingMode mode) { scalingMode = mode; }
    ScalingMode getScalingMode() const { return scalingMode; }
    
    // Reload the image (useful if file changed)
    void reload(SDL_Renderer* renderer);
    
    // Set new image source
    void setImagePath(SDL_Renderer* renderer, const std::string& path);
    void setImageData(SDL_Renderer* renderer, const unsigned char* data, size_t dataSize);
    
    // Real-time safe methods (lock-free, audio thread safe)
    void realtimeSetImagePath(const std::string& path);
};

} // namespace ui
