#include "Slider.h"
#include "../../lib/Theme/ThemeBase.h"
#include "../../src/Helpers.h"
#include "../../src/UICore.h"
#include <SDL2/SDL.h>
#include <algorithm>
#include <iostream>
#include <cmath>

namespace ui {

// Base Slider implementation
Slider::Slider(int x_, int y_, int w_, int h_, float min, float max, float initial)
    : UIElement(x_, y_, w_, h_), minValue(min), maxValue(max), currentValue(std::clamp(initial, min, max)) {
}

void Slider::handleEvent(const SDL_Event &e) {
    if (!visible) return;
    
    if (e.type == SDL_MOUSEBUTTONDOWN) {
        if (e.button.button == SDL_BUTTON_LEFT) {
            int mouseX = e.button.x;
            int mouseY = e.button.y;
            if (containsPoint(mouseX, mouseY)) {
                if (coreRef) {
                    coreRef->setFocus(elementId);
                }
                isDragging = true;
                updateValueFromMouse(mouseX, mouseY);
            }
        }
    } else if (e.type == SDL_MOUSEBUTTONUP) {
        if (e.button.button == SDL_BUTTON_LEFT) {
            isDragging = false;
        }
    } else if (e.type == SDL_MOUSEMOTION && isDragging) {
        updateValueFromMouse(e.motion.x, e.motion.y);
    }
}

void Slider::updateValueFromMouse(int mouseX, int mouseY) {
    float newValue = getValueFromPosition(mouseX, mouseY);
    setValue(newValue);
}

void Slider::activate() {
    // Slider activation could increment/decrement by small amount
}

void Slider::setValue(float value) {
    float clampedValue = std::clamp(value, minValue, maxValue);
    if (clampedValue != currentValue) {
        currentValue = clampedValue;
        if (onChange && coreRef) {
            float valueToPass = currentValue;
            coreRef->queueCallback([this, valueToPass]() {
                if (onChange) {
                    try {
                        onChange(valueToPass);
                    } catch (const std::exception& e) {
                        std::cerr << "Error in slider callback: " << e.what() << std::endl;
                    }
                }
            });
        }
    }
}

void Slider::setRange(float min, float max) {
    minValue = min;
    maxValue = max;
    setValue(currentValue); // Re-clamp current value
}

// HSlider implementation
HSlider::HSlider(int x, int y, int w, int h, float min, float max, float initial)
    : Slider(x, y, w, h, min, max, initial) {
}

float HSlider::getValueFromPosition(int mouseX, int mouseY) {
    int relativeX = mouseX - x;
    float ratio = static_cast<float>(relativeX) / width;
    ratio = std::clamp(ratio, 0.0f, 1.0f);
    return minValue + ratio * (maxValue - minValue);
}

void HSlider::render(SDL_Renderer* renderer, TTF_Font* font, std::shared_ptr<Theme> theme) {
    if (!renderer || !theme) return;
    
    auto colors = theme->sliderColors();
    
    // Draw track
    SDL_Rect trackRect = { x, y + height/2 - 2, width, 4 };
    drawFilledRect(renderer, trackRect, colors.sliderTrack);
    SDL_SetRenderDrawColor(renderer, colors.sliderBorder.r, colors.sliderBorder.g, colors.sliderBorder.b, colors.sliderBorder.a);
    SDL_RenderDrawRect(renderer, &trackRect);
    
    // Draw thumb
    float ratio = (currentValue - minValue) / (maxValue - minValue);
    int thumbX = x + static_cast<int>(ratio * width) - 8;
    SDL_Rect thumbRect = { thumbX, y, 16, height };
    drawFilledRect(renderer, thumbRect, colors.sliderThumb);
    SDL_SetRenderDrawColor(renderer, colors.sliderBorder.r, colors.sliderBorder.g, colors.sliderBorder.b, colors.sliderBorder.a);
    SDL_RenderDrawRect(renderer, &thumbRect);
    
    // Draw focus indicator
    if (hasFocus) {
        SDL_Rect focusRect = getFocusRect();
        SDL_SetRenderDrawColor(renderer, colors.buttonText.r, colors.buttonText.g, colors.buttonText.b, colors.buttonText.a);
        SDL_RenderDrawRect(renderer, &focusRect);
    }
}

// VSlider implementation
VSlider::VSlider(int x, int y, int w, int h, float min, float max, float initial)
    : Slider(x, y, w, h, min, max, initial) {
}

float VSlider::getValueFromPosition(int mouseX, int mouseY) {
    int relativeY = mouseY - y;
    float ratio = 1.0f - (static_cast<float>(relativeY) / height); // Inverted for vertical
    ratio = std::clamp(ratio, 0.0f, 1.0f);
    return minValue + ratio * (maxValue - minValue);
}

void VSlider::render(SDL_Renderer* renderer, TTF_Font* font, std::shared_ptr<Theme> theme) {
    if (!renderer || !theme) return;
    
    auto colors = theme->sliderColors();
    
    // Draw track
    SDL_Rect trackRect = { x + width/2 - 2, y, 4, height };
    drawFilledRect(renderer, trackRect, colors.sliderTrack);
    SDL_SetRenderDrawColor(renderer, colors.sliderBorder.r, colors.sliderBorder.g, colors.sliderBorder.b, colors.sliderBorder.a);
    SDL_RenderDrawRect(renderer, &trackRect);
    
    // Draw thumb
    float ratio = (currentValue - minValue) / (maxValue - minValue);
    int thumbY = y + height - static_cast<int>(ratio * height) - 8; // Inverted for vertical
    SDL_Rect thumbRect = { x, thumbY, width, 16 };
    drawFilledRect(renderer, thumbRect, colors.sliderThumb);
    SDL_SetRenderDrawColor(renderer, colors.sliderBorder.r, colors.sliderBorder.g, colors.sliderBorder.b, colors.sliderBorder.a);
    SDL_RenderDrawRect(renderer, &thumbRect);
    
    // Draw focus indicator
    if (hasFocus) {
        SDL_Rect focusRect = getFocusRect();
        SDL_SetRenderDrawColor(renderer, colors.buttonText.r, colors.buttonText.g, colors.buttonText.b, colors.buttonText.a);
        SDL_RenderDrawRect(renderer, &focusRect);
    }
}

// KnobSlider implementation
KnobSlider::KnobSlider(int x, int y, int w, int h, float min, float max, float initial)
    : Slider(x, y, w, h, min, max, initial) {
}

float KnobSlider::getValueFromPosition(int mouseX, int mouseY) {
    int centerX = x + width / 2;
    int centerY = y + height / 2;
    int relativeX = mouseX - centerX;
    int relativeY = mouseY - centerY;
    
    // Calculate angle from center (-π to π)
    float angle = atan2(relativeY, relativeX);
    
    // Convert to 0-2π range, starting from top (rotate by π/2)
    angle += M_PI / 2;
    if (angle < 0) angle += 2 * M_PI;
    
    // Map angle to value (270 degrees of rotation, leaving 90 degrees gap at bottom)
    float usableAngle = 3 * M_PI / 2; // 270 degrees
    float startAngle = M_PI / 8; // 22.5 degrees from bottom
    
    if (angle < startAngle || angle > (2 * M_PI - startAngle)) {
        // In the gap area, clamp to nearest edge
        if (angle < M_PI) {
            angle = startAngle;
        } else {
            angle = 2 * M_PI - startAngle;
        }
    }
    
    // Normalize to 0-1 range
    float normalizedAngle = (angle - startAngle) / usableAngle;
    normalizedAngle = std::clamp(normalizedAngle, 0.0f, 1.0f);
    
    return minValue + normalizedAngle * (maxValue - minValue);
}

void KnobSlider::render(SDL_Renderer* renderer, TTF_Font* font, std::shared_ptr<Theme> theme) {
    if (!renderer || !theme) return;
    
    auto colors = theme->sliderColors();
    int centerX = x + width / 2;
    int centerY = y + height / 2;
    int radius = std::min(width, height) / 2 - 6; // More padding for better proportions
    
    // Draw filled circle (knob body) with anti-aliasing effect
    SDL_SetRenderDrawColor(renderer, colors.sliderThumb.r, colors.sliderThumb.g, colors.sliderThumb.b, colors.sliderThumb.a);
    
    // Multiple passes for smoother appearance
    for (int r = 0; r <= radius; r++) {
        for (int dy = -r; dy <= r; dy++) {
            int dx = (int)sqrt(r * r - dy * dy);
            if (r == radius) {
                // Outer edge - draw border
                SDL_SetRenderDrawColor(renderer, colors.sliderBorder.r, colors.sliderBorder.g, colors.sliderBorder.b, colors.sliderBorder.a);
                SDL_RenderDrawPoint(renderer, centerX - dx, centerY + dy);
                SDL_RenderDrawPoint(renderer, centerX + dx, centerY + dy);
            } else {
                // Inner fill
                SDL_SetRenderDrawColor(renderer, colors.sliderThumb.r, colors.sliderThumb.g, colors.sliderThumb.b, colors.sliderThumb.a);
                SDL_RenderDrawLine(renderer, centerX - dx, centerY + dy, centerX + dx, centerY + dy);
            }
        }
    }
    
    // Calculate indicator position
    float ratio = (currentValue - minValue) / (maxValue - minValue);
    float usableAngle = 3 * M_PI / 2; // 270 degrees
    float startAngle = M_PI / 8; // 22.5 degrees from bottom
    float valueAngle = startAngle + ratio * usableAngle - M_PI / 2; // Adjust for top start
    
    // Draw thick indicator line (much more prominent)
    int indicatorLength = radius - 8;
    int indicatorX = centerX + cos(valueAngle) * indicatorLength;
    int indicatorY = centerY + sin(valueAngle) * indicatorLength;
    
    SDL_SetRenderDrawColor(renderer, colors.sliderBorder.r, colors.sliderBorder.g, colors.sliderBorder.b, colors.sliderBorder.a);
    
    // Draw thick line by drawing multiple parallel lines
    for (int offset = -2; offset <= 2; offset++) {
        int offsetX = offset * sin(valueAngle);
        int offsetY = -offset * cos(valueAngle);
        SDL_RenderDrawLine(renderer, 
                          centerX + offsetX, centerY + offsetY, 
                          indicatorX + offsetX, indicatorY + offsetY);
    }
    
    // Draw larger center dot for better visual feedback
    SDL_SetRenderDrawColor(renderer, colors.sliderBorder.r, colors.sliderBorder.g, colors.sliderBorder.b, colors.sliderBorder.a);
    for (int dx = -2; dx <= 2; dx++) {
        for (int dy = -2; dy <= 2; dy++) {
            if (dx*dx + dy*dy <= 4) { // Circle of radius 2
                SDL_RenderDrawPoint(renderer, centerX + dx, centerY + dy);
            }
        }
    }
    SDL_Rect centerDot = { centerX - 2, centerY - 2, 4, 4 };
    SDL_RenderFillRect(renderer, &centerDot);
    
    // Draw focus indicator
    if (hasFocus) {
        SDL_Rect focusRect = getFocusRect();
        SDL_SetRenderDrawColor(renderer, colors.buttonText.r, colors.buttonText.g, colors.buttonText.b, colors.buttonText.a);
        SDL_RenderDrawRect(renderer, &focusRect);
    }
}

} // namespace ui
