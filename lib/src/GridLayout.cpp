#include "uiframework/UIElements/GridLayout.h"
#include "uiframework/Theme/ThemeBase.h"
#include <algorithm>

namespace ui {

void GridLayout::addChild(std::shared_ptr<UIElement> child, int row, int col, int rowSpan, int colSpan) {
    if (child && row >= 0 && col >= 0 && row < rows && col < cols) {
        GridCell cell;
        cell.element = child;
        cell.row = row;
        cell.col = col;
        cell.rowSpan = std::min(rowSpan, rows - row);
        cell.colSpan = std::min(colSpan, cols - col);
        cells.push_back(cell);
        updateLayout();
    }
}

void GridLayout::removeChild(std::shared_ptr<UIElement> child) {
    auto it = std::find_if(cells.begin(), cells.end(), 
        [child](const GridCell& cell) { return cell.element == child; });
    if (it != cells.end()) {
        cells.erase(it);
        updateLayout();
    }
}

void GridLayout::clearChildren() {
    cells.clear();
    updateLayout();
}

void GridLayout::updateLayout() {
    if (cells.empty() || rows <= 0 || cols <= 0) return;
    
    SDL_Rect contentRect = getContentRect();
    
    // Calculate cell dimensions
    int cellWidth = (contentRect.w - (cols - 1) * spacing) / cols;
    int cellHeight = (contentRect.h - (rows - 1) * spacing) / rows;
    
    // Position each cell
    for (const auto& cell : cells) {
        if (!cell.element) continue;
        
        // Calculate cell position
        int cellX = contentRect.x + cell.col * (cellWidth + spacing);
        int cellY = contentRect.y + cell.row * (cellHeight + spacing);
        
        // Calculate cell size with spans
        int cellW = cellWidth * cell.colSpan + spacing * (cell.colSpan - 1);
        int cellH = cellHeight * cell.rowSpan + spacing * (cell.rowSpan - 1);
        
        // Apply alignment within cell
        int elementX = cellX;
        int elementY = cellY;
        
        if (horizontalAlignment == HAlignment::Center) {
            elementX = cellX + (cellW - cell.element->getWidth()) / 2;
        } else if (horizontalAlignment == HAlignment::Right) {
            elementX = cellX + cellW - cell.element->getWidth();
        }
        
        if (verticalAlignment == VAlignment::Center) {
            elementY = cellY + (cellH - cell.element->getHeight()) / 2;
        } else if (verticalAlignment == VAlignment::Bottom) {
            elementY = cellY + cellH - cell.element->getHeight();
        }
        
        cell.element->setPosition(elementX, elementY);
    }
}

void GridLayout::renderImpl(const RenderContext& ctx) {
    // Render all children
    for (const auto& cell : cells) {
        if (cell.element) {
            cell.element->render(ctx);
        }
    }
}

std::pair<int, int> GridLayout::getPreferredSize(TTF_Font* font) const {
    if (cells.empty() || rows <= 0 || cols <= 0) return {0, 0};
    
    // Calculate maximum cell dimensions
    int maxCellWidth = 0;
    int maxCellHeight = 0;
    
    for (const auto& cell : cells) {
        if (cell.element) {
            auto cellSize = cell.element->getPreferredSize(font);
            maxCellWidth = std::max(maxCellWidth, cellSize.first / cell.colSpan);
            maxCellHeight = std::max(maxCellHeight, cellSize.second / cell.rowSpan);
        }
    }
    
    int totalWidth = maxCellWidth * cols + spacing * (cols - 1);
    int totalHeight = maxCellHeight * rows + spacing * (rows - 1);
    
    return {totalWidth, totalHeight};
}

std::pair<int, int> GridLayout::getMinimumSize() const {
    if (cells.empty() || rows <= 0 || cols <= 0) return {0, 0};
    
    // Calculate maximum cell dimensions
    int maxCellWidth = 0;
    int maxCellHeight = 0;
    
    for (const auto& cell : cells) {
        if (cell.element) {
            auto cellSize = cell.element->getMinimumSize();
            maxCellWidth = std::max(maxCellWidth, cellSize.first / cell.colSpan);
            maxCellHeight = std::max(maxCellHeight, cellSize.second / cell.rowSpan);
        }
    }
    
    int totalWidth = maxCellWidth * cols + spacing * (cols - 1);
    int totalHeight = maxCellHeight * rows + spacing * (rows - 1);
    
    return {totalWidth, totalHeight};
}

void GridLayout::autoSize(TTF_Font* font) {
    auto preferredSize = getPreferredSize(font);
    setSize(preferredSize.first, preferredSize.second);
    updateLayout();
}

} // namespace ui
