#pragma once
#include "Layout.h"
#include <vector>
#include <mutex>

namespace ui {

struct GridCell {
    std::shared_ptr<UIElement> element;
    int rowSpan = 1;
    int columnSpan = 1;
    bool occupied = false;
};

class GridLayout : public Layout {
private:
    mutable std::mutex gridMutex;
    std::vector<std::vector<GridCell>> grid;
    std::vector<int> rowHeights;
    std::vector<int> columnWidths;
    int rows;
    int columns;
    int borderWidth = 0;  // Border width in pixels
    
public:
    GridLayout(int rows, int columns);
    
    // Grid-specific methods
    void addItem(std::shared_ptr<UIElement> element, int row, int column);
    void addItem(std::shared_ptr<UIElement> element, int row, int column, int rowSpan, int columnSpan);
    
    // Base class overrides
    void addItem(std::shared_ptr<UIElement> element) override;
    void addItem(std::shared_ptr<UIElement> element, SizePolicy hPolicy, SizePolicy vPolicy) override;
    void addItem(std::shared_ptr<UIElement> element, int stretch) override;
    
    void updateLayout(int x, int y, int width, int height) override;
    std::pair<int, int> getMinimumSize() const override;
    std::pair<int, int> getPreferredSize() const override;
    
    void removeItem(std::shared_ptr<UIElement> element) override;
    void removeItem(const std::string& elementId) override;
    void clear() override;
    
    int getRowCount() const { return rows; }
    int getColumnCount() const { return columns; }
    
    // Border configuration
    void setBorderWidth(int width) { borderWidth = width; }
    int getBorderWidth() const { return borderWidth; }
    
private:
    bool canPlaceAt(int row, int col, int rowSpan, int colSpan) const;
    void placeElement(std::shared_ptr<UIElement> element, int row, int col, int rowSpan, int colSpan);
    std::pair<int, int> findNextPosition() const;
};

} // namespace ui
