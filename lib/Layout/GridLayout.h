#pragma once
#include "Layout.h"
#include <vector>

namespace ui {

// Grid position structure
struct GridPosition {
    int row = 0;
    int column = 0;
    int rowSpan = 1;
    int columnSpan = 1;
    
    GridPosition() = default;
    GridPosition(int r, int c) : row(r), column(c) {}
    GridPosition(int r, int c, int rs, int cs) : row(r), column(c), rowSpan(rs), columnSpan(cs) {}
};

// Grid Layout Item
struct GridLayoutItem : public LayoutItem {
    GridPosition position;
    
    GridLayoutItem(std::shared_ptr<UIElement> elem, const GridPosition& pos) 
        : LayoutItem(elem), position(pos) {}
};

// Grid Layout - arranges elements in a grid
class GridLayout : public Layout {
private:
    std::vector<GridLayoutItem> gridItems;
    std::vector<int> rowHeights;
    std::vector<int> columnWidths;
    std::vector<int> rowStretch;
    std::vector<int> columnStretch;
    int rows = 0;
    int columns = 0;
    
public:
    GridLayout() = default;
    GridLayout(int rows, int columns);
    
    // Override base class methods
    void addItem(std::shared_ptr<UIElement> element) override;
    void addItem(std::shared_ptr<UIElement> element, SizePolicy hPolicy, SizePolicy vPolicy) override;
    void addItem(std::shared_ptr<UIElement> element, int stretch) override;
    
    // Grid-specific methods
    void addItem(std::shared_ptr<UIElement> element, int row, int column);
    void addItem(std::shared_ptr<UIElement> element, int row, int column, int rowSpan, int columnSpan);
    void addItem(std::shared_ptr<UIElement> element, const GridPosition& position);
    
    // Layout implementation
    void updateLayout(int x, int y, int width, int height) override;
    std::pair<int, int> getMinimumSize() const override;
    std::pair<int, int> getPreferredSize() const override;
    
    // Grid configuration
    void setRowStretch(int row, int stretch);
    void setColumnStretch(int column, int stretch);
    void setRowMinimumHeight(int row, int height);
    void setColumnMinimumWidth(int column, int width);
    
    // Grid information
    int getRowCount() const { return rows; }
    int getColumnCount() const { return columns; }
    
    // Remove methods
    void removeItem(std::shared_ptr<UIElement> element) override;
    void removeItem(const std::string& elementId) override;
    void clear() override;
    
private:
    void ensureGridSize(int row, int column);
    void calculateRowHeights(int availableHeight);
    void calculateColumnWidths(int availableWidth);
    std::pair<int, int> calculateCellSize(const GridLayoutItem& item) const;
    void updateGridDimensions();
    int findNextAvailablePosition();
};

} // namespace ui
