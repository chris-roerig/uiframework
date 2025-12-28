#include "VBoxLayout.h"
#include <algorithm>
#include <numeric>

namespace ui {

VBoxLayout::VBoxLayout(int spacing) {
    this->spacing = spacing;
}

void VBoxLayout::updateLayout(int x, int y, int width, int height) {
    if (items.empty()) return;
    
    auto [contentWidth, contentHeight] = calculateContentSize(width, height);
    int currentY = y + padding.top;
    
    // Calculate heights for each item
    std::vector<int> heights = distributeSpace(contentHeight);
    
    for (size_t i = 0; i < items.size(); ++i) {
        const auto& item = items[i];
        if (!item.element) continue;
        
        int itemHeight = heights[i];
        int itemWidth = item.element->width;
        
        // Handle horizontal sizing
        if (item.horizontalPolicy == SizePolicy::Expanding) {
            itemWidth = contentWidth - item.margin.left - item.margin.right;
        } else if (item.horizontalPolicy == SizePolicy::Minimum) {
            itemWidth = std::max(item.minWidth, item.element->width);
        }
        
        // Clamp to min/max constraints
        itemWidth = std::clamp(itemWidth, item.minWidth, item.maxWidth);
        
        // Calculate horizontal position based on alignment
        int itemX = x + padding.left + item.margin.left;
        switch (item.horizontalAlignment) {
            case Alignment::Center:
                itemX = x + padding.left + (contentWidth - itemWidth) / 2;
                break;
            case Alignment::End:
                itemX = x + width - padding.right - item.margin.right - itemWidth;
                break;
            case Alignment::Start:
            case Alignment::Stretch:
            default:
                break;
        }
        
        // Update element position and size
        item.element->setPosition(itemX, currentY + item.margin.top);
        item.element->setSize(itemWidth, itemHeight - item.margin.top - item.margin.bottom);
        
        currentY += itemHeight + spacing;
    }
}

std::pair<int, int> VBoxLayout::getMinimumSize() const {
    if (items.empty()) {
        return {padding.left + padding.right, padding.top + padding.bottom};
    }
    
    int minWidth = calculateMaxWidth();
    int minHeight = calculateTotalMinHeight();
    
    return {
        minWidth + padding.left + padding.right,
        minHeight + padding.top + padding.bottom + calculateTotalSpacing()
    };
}

std::pair<int, int> VBoxLayout::getPreferredSize() const {
    if (items.empty()) {
        return {padding.left + padding.right, padding.top + padding.bottom};
    }
    
    int prefWidth = calculateMaxWidth();
    int prefHeight = calculateTotalPreferredHeight();
    
    return {
        prefWidth + padding.left + padding.right,
        prefHeight + padding.top + padding.bottom + calculateTotalSpacing()
    };
}

int VBoxLayout::calculateTotalMinHeight() const {
    int total = 0;
    for (const auto& item : items) {
        if (item.element) {
            int itemMinHeight = std::max(item.minHeight, item.element->height);
            total += itemMinHeight + item.margin.top + item.margin.bottom;
        }
    }
    return total;
}

int VBoxLayout::calculateTotalPreferredHeight() const {
    int total = 0;
    for (const auto& item : items) {
        if (item.element) {
            int itemHeight = item.element->height + item.margin.top + item.margin.bottom;
            total += itemHeight;
        }
    }
    return total;
}

int VBoxLayout::calculateMaxWidth() const {
    int maxWidth = 0;
    for (const auto& item : items) {
        if (item.element) {
            int itemWidth = item.element->width + item.margin.left + item.margin.right;
            maxWidth = std::max(maxWidth, itemWidth);
        }
    }
    return maxWidth;
}

std::vector<int> VBoxLayout::distributeSpace(int availableHeight) const {
    std::vector<int> heights;
    heights.reserve(items.size());
    
    if (items.empty()) return heights;
    
    // Calculate total stretch factor
    int totalStretch = 0;
    int fixedHeight = 0;
    
    for (const auto& item : items) {
        if (!item.element) continue;
        
        if (item.verticalPolicy == SizePolicy::Expanding && item.stretch > 0) {
            totalStretch += item.stretch;
        } else {
            int itemHeight = item.element->height + item.margin.top + item.margin.bottom;
            fixedHeight += itemHeight;
        }
    }
    
    int remainingHeight = availableHeight - fixedHeight - calculateTotalSpacing();
    
    // Distribute space
    for (const auto& item : items) {
        if (!item.element) {
            heights.push_back(0);
            continue;
        }
        
        int itemHeight;
        if (item.verticalPolicy == SizePolicy::Expanding && item.stretch > 0 && totalStretch > 0) {
            itemHeight = (remainingHeight * item.stretch) / totalStretch + item.margin.top + item.margin.bottom;
        } else {
            itemHeight = item.element->height + item.margin.top + item.margin.bottom;
        }
        
        // Apply constraints
        itemHeight = std::clamp(itemHeight, 
                               item.minHeight + item.margin.top + item.margin.bottom,
                               item.maxHeight + item.margin.top + item.margin.bottom);
        
        heights.push_back(itemHeight);
    }
    
    return heights;
}

} // namespace ui
