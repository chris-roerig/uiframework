#include "Slider.h"
#include "../../lib/Theme/ThemeBase.h"
#include "../../src/Helpers.h"
#include "../../src/UICore.h"
#include <SDL2/SDL.h>
#include <algorithm>
#include <iostream>

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
    
    auto colors = theme->buttonColors();
    
    // Draw track
    SDL_Rect trackRect = { x, y + height/2 - 2, width, 4 };
    drawFilledRect(renderer, trackRect, colors.buttonBackground);
    SDL_SetRenderDrawColor(renderer, colors.buttonText.r, colors.buttonText.g, colors.buttonText.b, colors.buttonText.a);
    SDL_RenderDrawRect(renderer, &trackRect);
    
    // Draw thumb
    float ratio = (currentValue - minValue) / (maxValue - minValue);
    int thumbX = x + static_cast<int>(ratio * width) - 8;
    SDL_Rect thumbRect = { thumbX, y, 16, height };
    drawFilledRect(renderer, thumbRect, colors.buttonForeground);
    SDL_SetRenderDrawColor(renderer, colors.buttonText.r, colors.buttonText.g, colors.buttonText.b, colors.buttonText.a);
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
    
    auto colors = theme->buttonColors();
    
    // Draw track
    SDL_Rect trackRect = { x + width/2 - 2, y, 4, height };
    drawFilledRect(renderer, trackRect, colors.buttonBackground);
    SDL_SetRenderDrawColor(renderer, colors.buttonText.r, colors.buttonText.g, colors.buttonText.b, colors.buttonText.a);
    SDL_RenderDrawRect(renderer, &trackRect);
    
    // Draw thumb
    float ratio = (currentValue - minValue) / (maxValue - minValue);
    int thumbY = y + height - static_cast<int>(ratio * height) - 8; // Inverted for vertical
    SDL_Rect thumbRect = { x, thumbY, width, 16 };
    drawFilledRect(renderer, thumbRect, colors.buttonForeground);
    SDL_SetRenderDrawColor(renderer, colors.buttonText.r, colors.buttonText.g, colors.buttonText.b, colors.buttonText.a);
    SDL_RenderDrawRect(renderer, &thumbRect);
    
    // Draw focus indicator
    if (hasFocus) {
        SDL_Rect focusRect = getFocusRect();
        SDL_SetRenderDrawColor(renderer, colors.buttonText.r, colors.buttonText.g, colors.buttonText.b, colors.buttonText.a);
        SDL_RenderDrawRect(renderer, &focusRect);
    }
}

} // namespace ui
