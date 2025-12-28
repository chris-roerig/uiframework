#pragma once

#include "UIElement.h"
#include <functional>

namespace ui {

class Slider : public UIElement {
protected:
    float minValue = 0.0f;
    float maxValue = 100.0f;
    float currentValue = 0.0f;
    std::function<void(float)> onChange;
    bool isDragging = false;
    
    virtual float getValueFromPosition(int mouseX, int mouseY) = 0;
    virtual void updateValueFromMouse(int mouseX, int mouseY);
    
public:
    Slider(int x, int y, int w, int h, float min = 0.0f, float max = 100.0f, float initial = 0.0f);
    
    void handleEvent(const SDL_Event &e) override;
    bool isInteractive() const override { return true; }
    void activate() override;
    
    void setValue(float value);
    float getValue() const { return currentValue; }
    void setRange(float min, float max);
    void setOnChange(std::function<void(float)> callback) { onChange = callback; }
    
    virtual ~Slider() = default;
};

class HSlider : public Slider {
protected:
    float getValueFromPosition(int mouseX, int mouseY) override;
    
public:
    HSlider(int x, int y, int w, int h, float min = 0.0f, float max = 100.0f, float initial = 0.0f);
    void render(SDL_Renderer* renderer, TTF_Font* font, std::shared_ptr<Theme> theme) override;
};

class VSlider : public Slider {
protected:
    float getValueFromPosition(int mouseX, int mouseY) override;
    
public:
    VSlider(int x, int y, int w, int h, float min = 0.0f, float max = 100.0f, float initial = 0.0f);
    void render(SDL_Renderer* renderer, TTF_Font* font, std::shared_ptr<Theme> theme) override;
};

} // namespace ui
