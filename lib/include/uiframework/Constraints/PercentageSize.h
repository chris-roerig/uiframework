#pragma once

namespace ui {

class PercentageSize {
public:
    static int calculateWidth(float percentage, int parentWidth);
    static int calculateHeight(float percentage, int parentHeight);
    static void calculateSize(float widthPercent, float heightPercent, 
                             int parentWidth, int parentHeight, 
                             int& outWidth, int& outHeight);
    static bool isValidPercentage(float percentage);
};

} // namespace ui
