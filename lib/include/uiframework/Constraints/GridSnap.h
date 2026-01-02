#pragma once

namespace ui {

class GridSnap {
public:
    static int snapToGrid(int value, int gridSize);
    static void snapPosition(int& x, int& y, int gridSize);
    static bool isValidGridSize(int gridSize);
};

} // namespace ui
