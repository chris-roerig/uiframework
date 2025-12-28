#include "GridLayout.h"
#include "../UIElements/LayoutContainer.h"
#include <algorithm>

namespace ui {

GridLayout::GridLayout(int rows, int columns) : rows(rows), columns(columns) {
    std::lock_guard<std::mutex> lock(gridMutex);
    grid.resize(rows, std::vector<GridCell>(columns));
    rowHeights.resize(rows, 0);
    columnWidths.resize(columns, 0);
}

void GridLayout::addItem(std::shared_ptr<UIElement> element, int row, int column) {
    addItem(element, row, column, 1, 1);
}

void GridLayout::addItem(std::shared_ptr<UIElement> element, int row, int column, int rowSpan, int columnSpan) {
    if (!element || row < 0 || column < 0 || rowSpan < 1 || columnSpan < 1) return;
    
    std::lock_guard<std::mutex> lock(gridMutex);
    
    if (row + rowSpan > rows || column + columnSpan > columns) return;
    if (!canPlaceAt(row, column, rowSpan, columnSpan)) return;
    
    placeElement(element, row, column, rowSpan, columnSpan);
}

void GridLayout::addItem(std::shared_ptr<UIElement> element) {
    if (!element) return;
    
    std::lock_guard<std::mutex> lock(gridMutex);
    auto [row, col] = findNextPosition();
    if (row >= 0 && col >= 0) {
        placeElement(element, row, col, 1, 1);
    }
}

void GridLayout::addItem(std::shared_ptr<UIElement> element, SizePolicy hPolicy, SizePolicy vPolicy) {
    addItem(element); // Simplified - ignore policies for now
}

void GridLayout::addItem(std::shared_ptr<UIElement> element, int stretch) {
    addItem(element); // Simplified - ignore stretch for now
}

void GridLayout::updateLayout(int x, int y, int width, int height) {
    std::lock_guard<std::mutex> lock(gridMutex);
    
    auto [contentWidth, contentHeight] = calculateContentSize(width, height);
    
    // Account for borders in available space
    int totalBorderWidth = (columns + 1) * borderWidth;
    int totalBorderHeight = (rows + 1) * borderWidth;
    int availableWidth = contentWidth - totalBorderWidth;
    int availableHeight = contentHeight - totalBorderHeight;
    
    // Calculate cell dimensions
    int cellWidth = availableWidth / columns;
    int cellHeight = availableHeight / rows;
    
    // Position elements
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < columns; ++c) {
            auto& cell = grid[r][c];
            if (cell.element && cell.occupied) {
                // Calculate position including borders
                int elemX = x + padding.left + borderWidth + c * (cellWidth + borderWidth);
                int elemY = y + padding.top + borderWidth + r * (cellHeight + borderWidth);
                
                // Calculate size for spanning elements
                int elemWidth = cellWidth * cell.columnSpan + borderWidth * (cell.columnSpan - 1);
                int elemHeight = cellHeight * cell.rowSpan + borderWidth * (cell.rowSpan - 1);
                
                cell.element->setPosition(elemX, elemY);
                cell.element->setSize(elemWidth, elemHeight);
                
                // If this element is a LayoutContainer, trigger its layout update
                if (auto* layoutContainer = dynamic_cast<ui::LayoutContainer*>(cell.element.get())) {
                    layoutContainer->updateLayout();
                }
            }
        }
    }
}

std::pair<int, int> GridLayout::getMinimumSize() const {
    std::lock_guard<std::mutex> lock(gridMutex);
    
    int minWidth = columns * 50 + (columns - 1) * spacing + padding.left + padding.right;
    int minHeight = rows * 30 + (rows - 1) * spacing + padding.top + padding.bottom;
    
    return {std::max(1, minWidth), std::max(1, minHeight)};
}

std::pair<int, int> GridLayout::getPreferredSize() const {
    return getMinimumSize();
}

void GridLayout::removeItem(std::shared_ptr<UIElement> element) {
    if (!element) return;
    
    std::lock_guard<std::mutex> lock(gridMutex);
    
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < columns; ++c) {
            if (grid[r][c].element == element) {
                int rowSpan = grid[r][c].rowSpan;
                int colSpan = grid[r][c].columnSpan;
                
                // Clear all cells occupied by this element
                for (int sr = r; sr < r + rowSpan; ++sr) {
                    for (int sc = c; sc < c + colSpan; ++sc) {
                        grid[sr][sc] = GridCell{};
                    }
                }
                return;
            }
        }
    }
}

void GridLayout::removeItem(const std::string& elementId) {
    std::lock_guard<std::mutex> lock(gridMutex);
    
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < columns; ++c) {
            if (grid[r][c].element && grid[r][c].element->getId() == elementId) {
                int rowSpan = grid[r][c].rowSpan;
                int colSpan = grid[r][c].columnSpan;
                
                // Clear all cells occupied by this element
                for (int sr = r; sr < r + rowSpan; ++sr) {
                    for (int sc = c; sc < c + colSpan; ++sc) {
                        grid[sr][sc] = GridCell{};
                    }
                }
                return;
            }
        }
    }
}

void GridLayout::clear() {
    std::lock_guard<std::mutex> lock(gridMutex);
    
    for (auto& row : grid) {
        for (auto& cell : row) {
            cell = GridCell{};
        }
    }
}

bool GridLayout::canPlaceAt(int row, int col, int rowSpan, int colSpan) const {
    if (row + rowSpan > rows || col + colSpan > columns) return false;
    
    for (int r = row; r < row + rowSpan; ++r) {
        for (int c = col; c < col + colSpan; ++c) {
            if (grid[r][c].occupied) return false;
        }
    }
    return true;
}

void GridLayout::placeElement(std::shared_ptr<UIElement> element, int row, int col, int rowSpan, int colSpan) {
    // Set the main cell
    grid[row][col] = {element, rowSpan, colSpan, true};
    
    // Mark spanning cells as occupied
    for (int r = row; r < row + rowSpan; ++r) {
        for (int c = col; c < col + colSpan; ++c) {
            if (r != row || c != col) {
                grid[r][c].occupied = true;
            }
        }
    }
}

std::pair<int, int> GridLayout::findNextPosition() const {
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < columns; ++c) {
            if (!grid[r][c].occupied) {
                return {r, c};
            }
        }
    }
    return {-1, -1}; // Grid is full
}

} // namespace ui
