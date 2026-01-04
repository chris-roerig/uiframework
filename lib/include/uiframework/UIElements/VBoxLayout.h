#pragma once
#include "UIElement.h"
#include <vector>
#include <memory>

namespace ui {

enum class VAlignment {
    Top,
    Center,
    Bottom
};

enum class HAlignment {
    Left,
    Center,
    Right
};

class VBoxLayout : public UIElement {
private:
    std::vector<std::shared_ptr<UIElement>> children;
    int spacing = 5;
    VAlignment verticalAlignment = VAlignment::Top;
    HAlignment horizontalAlignment = HAlignment::Left;
    
public:
    VBoxLayout(int x_, int y_, int w_, int h_)
        : UIElement(x_, y_, w_, h_) {}
    
    // Child management
    void addChild(std::shared_ptr<UIElement> child);
    void removeChild(std::shared_ptr<UIElement> child);
    void clearChildren();
    const std::vector<std::shared_ptr<UIElement>>& getChildren() const { return children; }
    
    // Layout configuration
    void setSpacing(int spacing_) { spacing = spacing_; updateLayout(); }
    int getSpacing() const { return spacing; }
    
    void setVerticalAlignment(VAlignment alignment) { verticalAlignment = alignment; updateLayout(); }
    VAlignment getVerticalAlignment() const { return verticalAlignment; }
    
    void setHorizontalAlignment(HAlignment alignment) { horizontalAlignment = alignment; updateLayout(); }
    HAlignment getHorizontalAlignment() const { return horizontalAlignment; }
    
    // Layout calculation
    void updateLayout();
    
    // Sizing API implementation
    std::pair<int, int> getPreferredSize(TTF_Font* font) const override;
    std::pair<int, int> getMinimumSize() const override;
    void autoSize(TTF_Font* font) override;
    
protected:
    void renderImpl(const RenderContext& ctx) override;
};

} // namespace ui
