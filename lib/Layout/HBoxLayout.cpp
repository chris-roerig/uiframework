#include "HBoxLayout.h"
#include <algorithm>
#include <numeric>

namespace ui {

HBoxLayout::HBoxLayout(int spacing) {
    this->spacing = spacing;
}

void HBoxLayout::updateLayout(int x, int y, int width, int height) {
    if (items.empty()) return;
    
    auto [contentWidth, contentHeight] = calculateContentSize(width, height);
    int currentX = x + padding.left;
    
    // Calculate widths for each item
    std::vector<int> widths = distributeSpace(contentWidth);
    
    for (size_t i = 0; i < items.size(); ++i) {
        const auto& item = items[i];
        if (!item.element) continue;
        
        int itemWidth = widths[i];
        int itemHeight = item.element->height;
        
        // Handle vertical sizing
        if (item.verticalPolicy == SizePolicy::Expanding) {
            itemHeight = contentHeight - item.margin.top - item.margin.bottom;
        } else if (item.verticalPolicy == SizePolicy::Minimum) {
            itemHeight = std::max(item.minHeight, item.element->height);
        }
        
        // Clamp to min/max constraints
        itemHeight = std::clamp(itemHeight, item.minHeight, item.maxHeight);
        
        // Calculate vertical position based on alignment
        int itemY = y + padding.top + item.margin.top;
        switch (item.verticalAlignment) {
            case Alignment::Center:
                itemY = y + padding.top + (contentHeight - itemHeight) / 2;
                break;
            case Alignment::End:
                itemY = y + height - padding.bottom - item.margin.bottom - itemHeight;
                break;
            case Alignment::Start:
            case Alignment::Stretch:
            default:
                break;
        }
        
        // Update element position and size
        item.element->setPosition(currentX + item.margin.left, itemY);
        item.element->setSize(itemWidth - item.margin.left - item.margin.right, itemHeight);
        
        currentX += itemWidth + spacing;
    }
}

std::pair<int, int> HBoxLayout::getMinimumSize() const {
    if (items.empty()) {
        return {padding.left + padding.right, padding.top + padding.bottom};
    }
    
    int minWidth = calculateTotalMinWidth();
    int minHeight = calculateMaxHeight();
    
    return {
        minWidth + padding.left + padding.right + calculateTotalSpacing(),
        minHeight + padding.top + padding.bottom
    };
}

std::pair<int, int> HBoxLayout::getPreferredSize() const {
    if (items.empty()) {
        return {padding.left + padding.right, padding.top + padding.bottom};
    }
    
    int prefWidth = calculateTotalPreferredWidth();
    int prefHeight = calculateMaxHeight();
    
    return {
        prefWidth + padding.left + padding.right + calculateTotalSpacing(),
        prefHeight + padding.top + padding.bottom
    };
}

int HBoxLayout::calculateTotalMinWidth() const {
    int total = 0;
    for (const auto& item : items) {
        if (item.element) {
            int itemMinWidth = std::max(item.minWidth, item.element->width);
            total += itemMinWidth + item.margin.left + item.margin.right;
        }
    }
    return total;
}

int HBoxLayout::calculateTotalPreferredWidth() const {
    int total = 0;
    for (const auto& item : items) {
        if (item.element) {
            int itemWidth = item.element->width + item.margin.left + item.margin.right;
            total += itemWidth;
        }
    }
    return total;
}

int HBoxLayout::calculateMaxHeight() const {
    int maxHeight = 0;
    for (const auto& item : items) {
        if (item.element) {
            int itemHeight = item.element->height + item.margin.top + item.margin.bottom;
            maxHeight = std::max(maxHeight, itemHeight);
        }
    }
    return maxHeight;
}

std::vector<int> HBoxLayout::distributeSpace(int availableWidth) const {
    std::vector<int> widths;
    widths.reserve(items.size());
    
    if (items.empty()) return widths;
    
    // Calculate total stretch factor
    int totalStretch = 0;
    int fixedWidth = 0;
    
    for (const auto& item : items) {
        if (!item.element) continue;
        
        if (item.horizontalPolicy == SizePolicy::Expanding && item.stretch > 0) {
            totalStretch += item.stretch;
        } else {
            int itemWidth = item.element->width + item.margin.left + item.margin.right;
            fixedWidth += itemWidth;
        }
    }
    
    int remainingWidth = availableWidth - fixedWidth - calculateTotalSpacing();
    
    // Distribute space
    for (const auto& item : items) {
        if (!item.element) {
            widths.push_back(0);
            continue;
        }
        
        int itemWidth;
        if (item.horizontalPolicy == SizePolicy::Expanding && item.stretch > 0 && totalStretch > 0) {
            itemWidth = (remainingWidth * item.stretch) / totalStretch + item.margin.left + item.margin.right;
        } else {
            itemWidth = item.element->width + item.margin.left + item.margin.right;
        }
        
        // Apply constraints
        itemWidth = std::clamp(itemWidth, 
                              item.minWidth + item.margin.left + item.margin.right,
                              item.maxWidth + item.margin.left + item.margin.right);
        
        widths.push_back(itemWidth);
    }
    
    return widths;
}

} // namespace ui
