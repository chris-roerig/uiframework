#pragma once
#include "UIElement.h"
#include <vector>
#include <string>
#include <functional>

namespace ui {

class CycleList : public UIElement {
private:
    std::vector<std::string> items;
    int selectedIndex = 0;
    std::function<void(int, const std::string&)> onChange;
    
public:
    CycleList(int x, int y, int w, int h, const std::vector<std::string>& items_);
    
protected:
    void renderImpl(const RenderContext& ctx) override;
public:
    void handleEvent(const SDL_Event &e) override;
    bool isInteractive() const override { return true; }
    
    // CycleList-specific methods
    void setItems(const std::vector<std::string>& newItems);
    void setSelectedIndex(int index);
    int getSelectedIndex() const { return selectedIndex; }
    const std::string& getSelectedValue() const;
    void setOnChange(std::function<void(int, const std::string&)> callback) { onChange = callback; }
    
private:
    void selectNext();
    void selectPrevious();
};

} // namespace ui
