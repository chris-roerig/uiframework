#include "uiframework/Constraints/GridSnap.h"

namespace ui {

int GridSnap::snapToGrid(int value, int gridSize) {
    if (gridSize <= 0) return value;
    return (value + gridSize / 2) / gridSize * gridSize;
}

void GridSnap::snapPosition(int& x, int& y, int gridSize) {
    if (gridSize <= 0) return;
    x = snapToGrid(x, gridSize);
    y = snapToGrid(y, gridSize);
}

bool GridSnap::isValidGridSize(int gridSize) {
    return gridSize > 0;
}

} // namespace ui
