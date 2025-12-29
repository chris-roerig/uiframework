#pragma once
#include "UIElement.h"
#include "../Layout/Layout.h"
#include <memory>

namespace ui {

// Container element that manages layout of child elements
class LayoutContainer : public UIElement {
private:
    std::unique_ptr<Layout> layout;
    bool autoResize = false;  // Disabled by default to prevent validation issues
    
public:
    LayoutContainer(int x_, int y_, int w_, int h_, std::unique_ptr<Layout> layout_);
    virtual ~LayoutContainer() override = default;
    
    // UIElement interface
    void render(SDL_Renderer* renderer, TTF_Font* font, std::shared_ptr<class Theme> theme) override;
    void handleEvent(const SDL_Event &e) override;
    bool isInteractive() const override { return false; } // Container itself is not interactive
    
    // Layout management
    void setLayout(std::unique_ptr<Layout> newLayout);
    Layout* getLayout() const { return layout.get(); }
    
    // Grid-specific methods (only work if layout is GridLayout)
    void addElement(std::shared_ptr<UIElement> element, int row, int column);
    void addElement(std::shared_ptr<UIElement> element, int row, int column, int rowSpan, int columnSpan);
    void setBorderWidth(int width);  // Set grid border width
    void updateLayout();
    void setAutoResize(bool enable) { autoResize = enable; }
    bool getAutoResize() const { return autoResize; }
    
    // Convenience methods for adding elements
    void addElement(std::shared_ptr<UIElement> element);
    void addElement(std::shared_ptr<UIElement> element, ui::SizePolicy hPolicy, ui::SizePolicy vPolicy);
    void addElement(std::shared_ptr<UIElement> element, int stretch);
    
    // Remove elements
    void removeElement(std::shared_ptr<UIElement> element);
    void removeElement(const std::string& elementId);
    void clearElements();
    
    // Size management
    void resizeToFit();
    std::pair<int, int> getPreferredSize() const;
    std::pair<int, int> getMinimumSize() const;
    
    // Override setSize to trigger layout update
    void setSize(int newWidth, int newHeight);
    void setPosition(int newX, int newY);
};

} // namespace ui
