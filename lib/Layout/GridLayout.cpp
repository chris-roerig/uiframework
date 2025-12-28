#include "GridLayout.h"
#include <algorithm>
#include <numeric>

namespace ui {

GridLayout::GridLayout(int rows, int columns) : rows(rows), columns(columns) {
    rowHeights.resize(rows, 0);
    columnWidths.resize(columns, 0);
    rowStretch.resize(rows, 0);
    columnStretch.resize(columns, 0);
}

void GridLayout::addItem(std::shared_ptr<UIElement> element) {
    if (!element) return;
    
    int pos = findNextAvailablePosition();
    int row = pos / std::max(1, columns);
    int col = pos % std::max(1, columns);
    
    addItem(element, row, col);
}

void GridLayout::addItem(std::shared_ptr<UIElement> element, SizePolicy hPolicy, SizePolicy vPolicy) {
    addItem(element);
    if (!gridItems.empty()) {
        auto& item = gridItems.back();
        item.horizontalPolicy = hPolicy;
        item.verticalPolicy = vPolicy;
    }
}

void GridLayout::addItem(std::shared_ptr<UIElement> element, int stretch) {
    addItem(element);
    if (!gridItems.empty()) {
        auto& item = gridItems.back();
        item.stretch = stretch;
        item.horizontalPolicy = SizePolicy::Expanding;
        item.verticalPolicy = SizePolicy::Expanding;
    }
}

void GridLayout::addItem(std::shared_ptr<UIElement> element, int row, int column) {
    addItem(element, GridPosition(row, column));
}

void GridLayout::addItem(std::shared_ptr<UIElement> element, int row, int column, int rowSpan, int columnSpan) {
    addItem(element, GridPosition(row, column, rowSpan, columnSpan));
}

void GridLayout::addItem(std::shared_ptr<UIElement> element, const GridPosition& position) {
    if (!element) return;
    
    ensureGridSize(position.row + position.rowSpan - 1, position.column + position.columnSpan - 1);
    gridItems.emplace_back(element, position);
    updateGridDimensions();
}

void GridLayout::updateLayout(int x, int y, int width, int height) {
    if (gridItems.empty()) return;
    
    auto [contentWidth, contentHeight] = calculateContentSize(width, height);
    
    calculateColumnWidths(contentWidth);
    calculateRowHeights(contentHeight);
    
    // Position elements
    for (const auto& item : gridItems) {
        if (!item.element) continue;
        
        const auto& pos = item.position;
        
        // Calculate position
        int itemX = x + padding.left;
        int itemY = y + padding.top;
        
        for (int c = 0; c < pos.column; ++c) {
            itemX += columnWidths[c] + spacing;
        }
        for (int r = 0; r < pos.row; ++r) {
            itemY += rowHeights[r] + spacing;
        }
        
        // Calculate size
        int itemWidth = 0;
        int itemHeight = 0;
        
        for (int c = pos.column; c < pos.column + pos.columnSpan; ++c) {
            itemWidth += columnWidths[c];
            if (c > pos.column) itemWidth += spacing;
        }
        
        for (int r = pos.row; r < pos.row + pos.rowSpan; ++r) {
            itemHeight += rowHeights[r];
            if (r > pos.row) itemHeight += spacing;
        }
        
        // Apply margins
        itemX += item.margin.left;
        itemY += item.margin.top;
        itemWidth -= item.margin.left + item.margin.right;
        itemHeight -= item.margin.top + item.margin.bottom;
        
        // Apply size constraints
        itemWidth = std::clamp(itemWidth, item.minWidth, item.maxWidth);
        itemHeight = std::clamp(itemHeight, item.minHeight, item.maxHeight);
        
        // Handle alignment within cell
        if (item.horizontalAlignment == Alignment::Center) {
            int cellWidth = itemWidth + item.margin.left + item.margin.right;
            itemX += (cellWidth - itemWidth) / 2;
        } else if (item.horizontalAlignment == Alignment::End) {
            int cellWidth = itemWidth + item.margin.left + item.margin.right;
            itemX += cellWidth - itemWidth - item.margin.right;
        }
        
        if (item.verticalAlignment == Alignment::Center) {
            int cellHeight = itemHeight + item.margin.top + item.margin.bottom;
            itemY += (cellHeight - itemHeight) / 2;
        } else if (item.verticalAlignment == Alignment::End) {
            int cellHeight = itemHeight + item.margin.top + item.margin.bottom;
            itemY += cellHeight - itemHeight - item.margin.bottom;
        }
        
        item.element->setPosition(itemX, itemY);
        item.element->setSize(itemWidth, itemHeight);
    }
}

std::pair<int, int> GridLayout::getMinimumSize() const {
    if (gridItems.empty()) {
        return {padding.left + padding.right, padding.top + padding.bottom};
    }
    
    int totalWidth = std::accumulate(columnWidths.begin(), columnWidths.end(), 0);
    int totalHeight = std::accumulate(rowHeights.begin(), rowHeights.end(), 0);
    
    totalWidth += (columns - 1) * spacing + padding.left + padding.right;
    totalHeight += (rows - 1) * spacing + padding.top + padding.bottom;
    
    return {totalWidth, totalHeight};
}

std::pair<int, int> GridLayout::getPreferredSize() const {
    return getMinimumSize(); // For now, same as minimum
}

void GridLayout::setRowStretch(int row, int stretch) {
    if (row >= 0 && row < static_cast<int>(rowStretch.size())) {
        rowStretch[row] = stretch;
    }
}

void GridLayout::setColumnStretch(int column, int stretch) {
    if (column >= 0 && column < static_cast<int>(columnStretch.size())) {
        columnStretch[column] = stretch;
    }
}

void GridLayout::setRowMinimumHeight(int row, int height) {
    if (row >= 0 && row < static_cast<int>(rowHeights.size())) {
        rowHeights[row] = std::max(rowHeights[row], height);
    }
}

void GridLayout::setColumnMinimumWidth(int column, int width) {
    if (column >= 0 && column < static_cast<int>(columnWidths.size())) {
        columnWidths[column] = std::max(columnWidths[column], width);
    }
}

void GridLayout::removeItem(std::shared_ptr<UIElement> element) {
    if (!element) return;
    
    gridItems.erase(
        std::remove_if(gridItems.begin(), gridItems.end(),
            [&element](const GridLayoutItem& item) {
                return item.element == element;
            }),
        gridItems.end()
    );
    updateGridDimensions();
}

void GridLayout::removeItem(const std::string& elementId) {
    gridItems.erase(
        std::remove_if(gridItems.begin(), gridItems.end(),
            [&elementId](const GridLayoutItem& item) {
                return item.element && item.element->getId() == elementId;
            }),
        gridItems.end()
    );
    updateGridDimensions();
}

void GridLayout::clear() {
    gridItems.clear();
    rows = columns = 0;
    rowHeights.clear();
    columnWidths.clear();
    rowStretch.clear();
    columnStretch.clear();
}

void GridLayout::ensureGridSize(int row, int column) {
    if (row >= rows) {
        rows = row + 1;
        rowHeights.resize(rows, 0);
        rowStretch.resize(rows, 0);
    }
    if (column >= columns) {
        columns = column + 1;
        columnWidths.resize(columns, 0);
        columnStretch.resize(columns, 0);
    }
}

void GridLayout::calculateRowHeights(int availableHeight) {
    if (rows == 0) return;
    
    // Reset heights
    std::fill(rowHeights.begin(), rowHeights.end(), 0);
    
    // Calculate minimum heights based on content
    for (const auto& item : gridItems) {
        if (!item.element) continue;
        
        int itemHeight = item.element->height + item.margin.top + item.margin.bottom;
        int heightPerRow = itemHeight / item.position.rowSpan;
        
        for (int r = item.position.row; r < item.position.row + item.position.rowSpan; ++r) {
            rowHeights[r] = std::max(rowHeights[r], heightPerRow);
        }
    }
    
    // Distribute remaining space based on stretch factors
    int totalMinHeight = std::accumulate(rowHeights.begin(), rowHeights.end(), 0);
    int totalSpacing = (rows - 1) * spacing;
    int remainingHeight = availableHeight - totalMinHeight - totalSpacing;
    
    if (remainingHeight > 0) {
        int totalStretch = std::accumulate(rowStretch.begin(), rowStretch.end(), 0);
        if (totalStretch > 0) {
            for (int r = 0; r < rows; ++r) {
                if (rowStretch[r] > 0) {
                    rowHeights[r] += (remainingHeight * rowStretch[r]) / totalStretch;
                }
            }
        }
    }
}

void GridLayout::calculateColumnWidths(int availableWidth) {
    if (columns == 0) return;
    
    // Reset widths
    std::fill(columnWidths.begin(), columnWidths.end(), 0);
    
    // Calculate minimum widths based on content
    for (const auto& item : gridItems) {
        if (!item.element) continue;
        
        int itemWidth = item.element->width + item.margin.left + item.margin.right;
        int widthPerColumn = itemWidth / item.position.columnSpan;
        
        for (int c = item.position.column; c < item.position.column + item.position.columnSpan; ++c) {
            columnWidths[c] = std::max(columnWidths[c], widthPerColumn);
        }
    }
    
    // Distribute remaining space based on stretch factors
    int totalMinWidth = std::accumulate(columnWidths.begin(), columnWidths.end(), 0);
    int totalSpacing = (columns - 1) * spacing;
    int remainingWidth = availableWidth - totalMinWidth - totalSpacing;
    
    if (remainingWidth > 0) {
        int totalStretch = std::accumulate(columnStretch.begin(), columnStretch.end(), 0);
        if (totalStretch > 0) {
            for (int c = 0; c < columns; ++c) {
                if (columnStretch[c] > 0) {
                    columnWidths[c] += (remainingWidth * columnStretch[c]) / totalStretch;
                }
            }
        }
    }
}

void GridLayout::updateGridDimensions() {
    rows = columns = 0;
    for (const auto& item : gridItems) {
        rows = std::max(rows, item.position.row + item.position.rowSpan);
        columns = std::max(columns, item.position.column + item.position.columnSpan);
    }
    
    rowHeights.resize(rows, 0);
    columnWidths.resize(columns, 0);
    rowStretch.resize(rows, 0);
    columnStretch.resize(columns, 0);
}

int GridLayout::findNextAvailablePosition() {
    // Simple implementation - just return the next position
    return static_cast<int>(gridItems.size());
}

} // namespace ui
