#include "uiframework/Layout/Layout.h"
#include <algorithm>
#include <climits>

namespace ui {

void Layout::addItem(std::shared_ptr<UIElement> element) {
    if (element) {
        items.emplace_back(element);
    }
}

void Layout::addItem(std::shared_ptr<UIElement> element, SizePolicy hPolicy, SizePolicy vPolicy) {
    if (element) {
        LayoutItem item(element);
        item.horizontalPolicy = hPolicy;
        item.verticalPolicy = vPolicy;
        items.push_back(item);
    }
}

void Layout::addItem(std::shared_ptr<UIElement> element, int stretch) {
    if (element) {
        LayoutItem item(element);
        item.stretch = stretch;
        item.horizontalPolicy = SizePolicy::Expanding;
        items.push_back(item);
    }
}

void Layout::removeItem(std::shared_ptr<UIElement> element) {
    if (!element) return;
    
    items.erase(
        std::remove_if(items.begin(), items.end(),
            [&element](const LayoutItem& item) {
                return item.element == element;
            }),
        items.end()
    );
}

void Layout::removeItem(const std::string& elementId) {
    items.erase(
        std::remove_if(items.begin(), items.end(),
            [&elementId](const LayoutItem& item) {
                return item.element && item.element->getId() == elementId;
            }),
        items.end()
    );
}

void Layout::clear() {
    items.clear();
}

int Layout::calculateTotalSpacing() const {
    return std::max(0, static_cast<int>(items.size()) - 1) * spacing;
}

std::pair<int, int> Layout::calculateContentSize(int availableWidth, int availableHeight) const {
    int contentWidth = availableWidth - padding.left - padding.right;
    int contentHeight = availableHeight - padding.top - padding.bottom;
    return {std::max(0, contentWidth), std::max(0, contentHeight)};
}

} // namespace ui
