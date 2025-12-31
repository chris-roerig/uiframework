#pragma once

#include "InteractiveElement.h"
#include <functional>

namespace ui {

class Slider : public InteractiveElement {
protected:
    float minValue = 0.0f;
    float maxValue = 100.0f;
    float currentValue = 0.0f;
    std::function<void(float)> onChange;
    
    virtual float getValueFromPosition(int mouseX, int mouseY) = 0;
    virtual void updateValueFromMouse(int mouseX, int mouseY);
    
    // InteractiveElement event handlers
    void onMouseDown(int x, int y) override;
    void onMouseUp(int x, int y) override;
    void onMouseDrag(int x, int y) override;
    
public:
    Slider(int x, int y, int w, int h, float min = 0.0f, float max = 100.0f, float initial = 0.0f);
    
    void setValue(float value);
    float getValue() const { return currentValue; }
    void setRange(float min, float max);
    void setOnChange(std::function<void(float)> callback) { onChange = callback; }
    
    virtual ~Slider() override = default;
};

class HSlider : public Slider {
protected:
    float getValueFromPosition(int mouseX, int mouseY) override;
    
public:
    HSlider(int x, int y, int w, int h, float min = 0.0f, float max = 100.0f, float initial = 0.0f);

protected:
    void renderImpl(const RenderContext& ctx) override;
};

class VSlider : public Slider {
protected:
    float getValueFromPosition(int mouseX, int mouseY) override;
    
public:
    VSlider(int x, int y, int w, int h, float min = 0.0f, float max = 100.0f, float initial = 0.0f);

protected:
    void renderImpl(const RenderContext& ctx) override;
};

class KnobSlider : public Slider {
protected:
    float getValueFromPosition(int mouseX, int mouseY) override;
    
public:
    KnobSlider(int x, int y, int w, int h, float min = 0.0f, float max = 100.0f, float initial = 0.0f);

protected:
    void renderImpl(const RenderContext& ctx) override;
};

} // namespace ui
