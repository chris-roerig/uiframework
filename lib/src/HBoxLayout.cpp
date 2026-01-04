#include "uiframework/UIElements/HBoxLayout.h"
#include "uiframework/Theme/ThemeBase.h"
#include <algorithm>

namespace ui {

void HBoxLayout::addChild(std::shared_ptr<UIElement> child) {
    if (child) {
        children.push_back(child);
        updateLayout();
    }
}

void HBoxLayout::removeChild(std::shared_ptr<UIElement> child) {
    auto it = std::find(children.begin(), children.end(), child);
    if (it != children.end()) {
        children.erase(it);
        updateLayout();
    }
}

void HBoxLayout::clearChildren() {
    children.clear();
    updateLayout();
}

void HBoxLayout::updateLayout() {
    if (children.empty()) return;
    
    SDL_Rect contentRect = getContentRect();
    int totalWidth = 0;
    
    // Calculate total width needed
    for (const auto& child : children) {
        totalWidth += child->getWidth();
    }
    totalWidth += (children.size() - 1) * spacing;
    
    // Calculate starting X position based on horizontal alignment
    int startX = contentRect.x;
    if (horizontalAlignment == HAlignment::Center) {
        startX = contentRect.x + (contentRect.w - totalWidth) / 2;
    } else if (horizontalAlignment == HAlignment::Right) {
        startX = contentRect.x + contentRect.w - totalWidth;
    }
    
    // Position children
    int currentX = startX;
    for (const auto& child : children) {
        int childY = contentRect.y;
        
        // Apply vertical alignment
        if (verticalAlignment == VAlignment::Center) {
            childY = contentRect.y + (contentRect.h - child->getHeight()) / 2;
        } else if (verticalAlignment == VAlignment::Bottom) {
            childY = contentRect.y + contentRect.h - child->getHeight();
        }
        
        child->setPosition(currentX, childY);
        currentX += child->getWidth() + spacing;
    }
}

void HBoxLayout::renderImpl(const RenderContext& ctx) {
    // Render all children
    for (const auto& child : children) {
        child->render(ctx);
    }
}

std::pair<int, int> HBoxLayout::getPreferredSize(TTF_Font* font) const {
    if (children.empty()) return {0, 0};
    
    int totalWidth = 0;
    int maxHeight = 0;
    
    for (const auto& child : children) {
        auto childSize = child->getPreferredSize(font);
        totalWidth += childSize.first;
        maxHeight = std::max(maxHeight, childSize.second);
    }
    
    totalWidth += (children.size() - 1) * spacing;
    return {totalWidth, maxHeight};
}

std::pair<int, int> HBoxLayout::getMinimumSize() const {
    if (children.empty()) return {0, 0};
    
    int totalWidth = 0;
    int maxHeight = 0;
    
    for (const auto& child : children) {
        auto childSize = child->getMinimumSize();
        totalWidth += childSize.first;
        maxHeight = std::max(maxHeight, childSize.second);
    }
    
    totalWidth += (children.size() - 1) * spacing;
    return {totalWidth, maxHeight};
}

void HBoxLayout::autoSize(TTF_Font* font) {
    auto preferredSize = getPreferredSize(font);
    setSize(preferredSize.first, preferredSize.second);
    updateLayout();
}

} // namespace ui
