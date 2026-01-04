#include "uiframework/UIElements/VBoxLayout.h"
#include "uiframework/Theme/ThemeBase.h"
#include <algorithm>

namespace ui {

void VBoxLayout::addChild(std::shared_ptr<UIElement> child) {
    if (child) {
        children.push_back(child);
        updateLayout();
    }
}

void VBoxLayout::removeChild(std::shared_ptr<UIElement> child) {
    auto it = std::find(children.begin(), children.end(), child);
    if (it != children.end()) {
        children.erase(it);
        updateLayout();
    }
}

void VBoxLayout::clearChildren() {
    children.clear();
    updateLayout();
}

void VBoxLayout::updateLayout() {
    if (children.empty()) return;
    
    SDL_Rect contentRect = getContentRect();
    int totalHeight = 0;
    
    // Calculate total height needed
    for (const auto& child : children) {
        totalHeight += child->getHeight();
    }
    totalHeight += (children.size() - 1) * spacing;
    
    // Calculate starting Y position based on vertical alignment
    int startY = contentRect.y;
    if (verticalAlignment == VAlignment::Center) {
        startY = contentRect.y + (contentRect.h - totalHeight) / 2;
    } else if (verticalAlignment == VAlignment::Bottom) {
        startY = contentRect.y + contentRect.h - totalHeight;
    }
    
    // Position children
    int currentY = startY;
    for (const auto& child : children) {
        int childX = contentRect.x;
        
        // Apply horizontal alignment
        if (horizontalAlignment == HAlignment::Center) {
            childX = contentRect.x + (contentRect.w - child->getWidth()) / 2;
        } else if (horizontalAlignment == HAlignment::Right) {
            childX = contentRect.x + contentRect.w - child->getWidth();
        }
        
        child->setPosition(childX, currentY);
        currentY += child->getHeight() + spacing;
    }
}

void VBoxLayout::renderImpl(const RenderContext& ctx) {
    // Render all children
    for (const auto& child : children) {
        child->render(ctx);
    }
}

std::pair<int, int> VBoxLayout::getPreferredSize(TTF_Font* font) const {
    if (children.empty()) return {0, 0};
    
    int maxWidth = 0;
    int totalHeight = 0;
    
    for (const auto& child : children) {
        auto childSize = child->getPreferredSize(font);
        maxWidth = std::max(maxWidth, childSize.first);
        totalHeight += childSize.second;
    }
    
    totalHeight += (children.size() - 1) * spacing;
    return {maxWidth, totalHeight};
}

std::pair<int, int> VBoxLayout::getMinimumSize() const {
    if (children.empty()) return {0, 0};
    
    int maxWidth = 0;
    int totalHeight = 0;
    
    for (const auto& child : children) {
        auto childSize = child->getMinimumSize();
        maxWidth = std::max(maxWidth, childSize.first);
        totalHeight += childSize.second;
    }
    
    totalHeight += (children.size() - 1) * spacing;
    return {maxWidth, totalHeight};
}

void VBoxLayout::autoSize(TTF_Font* font) {
    auto preferredSize = getPreferredSize(font);
    setSize(preferredSize.first, preferredSize.second);
    updateLayout();
}

} // namespace ui
