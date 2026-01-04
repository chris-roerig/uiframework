#pragma once
#include "InteractiveElement.h"
#include <vector>
#include <string>
#include <functional>

namespace ui {

class CycleList : public InteractiveElement {
private:
    std::vector<std::string> items;
    int selectedIndex = 0;
    std::function<void(int, const std::string&)> onChange;
    
    // Real-time double buffering
    int selectedIndexBuffers[2];
    std::atomic<int> currentSelectedIndexBuffer{0};
    
public:
    CycleList(int x, int y, int w, int h, const std::vector<std::string>& items_);
    
protected:
    void renderImpl(const RenderContext& ctx) override;
public:
    void activate() override;

protected:
    void onMouseDown(int x, int y) override;
    void onKeyDown(const SDL_Keycode& key) override;

public:
    // CycleList-specific methods
    void setItems(const std::vector<std::string>& newItems);
    void setSelectedIndex(int index);
    int getSelectedIndex() const { return selectedIndex; }
    const std::string& getSelectedValue() const;
    void setOnChange(std::function<void(int, const std::string&)> callback) { onChange = callback; }
    
    // Real-time safe methods (lock-free, audio thread safe)
    void realtimeSetSelectedIndex(int index);
    
private:
    void selectNext();
    void selectPrevious();
};

} // namespace ui
