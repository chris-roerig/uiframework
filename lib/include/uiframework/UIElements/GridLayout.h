#pragma once
#include "UIElement.h"
#include "LayoutAlignment.h"
#include <vector>
#include <memory>

namespace ui {

struct GridCell {
    std::shared_ptr<UIElement> element;
    int row = 0;
    int col = 0;
    int rowSpan = 1;
    int colSpan = 1;
};

class GridLayout : public UIElement {
private:
    std::vector<GridCell> cells;
    int rows = 1;
    int cols = 1;
    int spacing = 5;
    VAlignment verticalAlignment = VAlignment::Top;
    HAlignment horizontalAlignment = HAlignment::Left;
    
public:
    GridLayout(int x_, int y_, int w_, int h_, int rows_, int cols_)
        : UIElement(x_, y_, w_, h_), rows(rows_), cols(cols_) {}
    
    // Child management
    void addChild(std::shared_ptr<UIElement> child, int row, int col, int rowSpan = 1, int colSpan = 1);
    void removeChild(std::shared_ptr<UIElement> child);
    void clearChildren();
    const std::vector<GridCell>& getCells() const { return cells; }
    
    // Grid configuration
    void setGridSize(int rows_, int cols_) { rows = rows_; cols = cols_; updateLayout(); }
    int getRows() const { return rows; }
    int getCols() const { return cols; }
    
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
