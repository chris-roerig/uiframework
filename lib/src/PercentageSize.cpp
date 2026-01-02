#include "uiframework/Constraints/PercentageSize.h"
#include <algorithm>

namespace ui {

int PercentageSize::calculateWidth(float percentage, int parentWidth) {
    if (!isValidPercentage(percentage) || parentWidth <= 0) return 0;
    return static_cast<int>(percentage * parentWidth);
}

int PercentageSize::calculateHeight(float percentage, int parentHeight) {
    if (!isValidPercentage(percentage) || parentHeight <= 0) return 0;
    return static_cast<int>(percentage * parentHeight);
}

void PercentageSize::calculateSize(float widthPercent, float heightPercent, 
                                  int parentWidth, int parentHeight, 
                                  int& outWidth, int& outHeight) {
    outWidth = calculateWidth(widthPercent, parentWidth);
    outHeight = calculateHeight(heightPercent, parentHeight);
}

bool PercentageSize::isValidPercentage(float percentage) {
    return percentage >= 0.0f && percentage <= 1.0f;
}

} // namespace ui
