#include "uiframework/UIElements/LayoutContainer.h"
#include "uiframework/Theme/ThemeBase.h"
#include "uiframework/Layout/GridLayout.h"
#include "uiframework/Helpers.h"
#include <SDL2/SDL.h>
#include <cmath>

namespace ui {

LayoutContainer::LayoutContainer(int x_, int y_, int w_, int h_, std::unique_ptr<Layout> layout_)
    : UIElement(x_, y_, w_, h_), layout(std::move(layout_)) {
    if (layout) {
        updateLayout();
    }
}

void LayoutContainer::renderImpl(const RenderContext& ctx) {
    if (!layout) {
        return;
    }
    
    // Draw grid borders if this is a GridLayout
    if (auto* gridLayout = dynamic_cast<ui::GridLayout*>(layout.get())) {
        int borderWidth = gridLayout->getBorderWidth();
        if (borderWidth > 0) {
            auto colors = ctx.theme->gridColors();
            SDL_SetRenderDrawColor(ctx.ctx.renderer, colors.gridBorder.r, colors.gridBorder.g, 
                                 colors.gridBorder.b, colors.gridBorder.a);
            
            int rows = gridLayout->getRowCount();
            int cols = gridLayout->getColumnCount();
            
            // Calculate cell dimensions
            int cellWidth = (width - (cols + 1) * borderWidth) / cols;
            int cellHeight = (height - (rows + 1) * borderWidth) / rows;
            
            // Draw vertical lines
            for (int c = 0; c <= cols; ++c) {
                int lineX = x + c * (cellWidth + borderWidth);
                SDL_Rect vLine = {lineX, y, borderWidth, height};
                SDL_RenderFillRect(ctx.renderer, &vLine);
            }
            
            // Draw horizontal lines
            for (int r = 0; r <= rows; ++r) {
                int lineY = y + r * (cellHeight + borderWidth);
                SDL_Rect hLine = {x, lineY, width, borderWidth};
                SDL_RenderFillRect(ctx.renderer, &hLine);
            }
        }
    }
    
    // Draw container focus border
    if (hasFocus) {
        auto colors = theme->focusColors();
        SDL_Rect focusRect = getFocusRect();
        SDL_SetRenderDrawColor(ctx.renderer, colors.focusBorder.r, colors.focusBorder.g, 
                             colors.focusBorder.b, colors.focusBorder.a);
        SDL_RenderDrawRect(ctx.renderer, &focusRect);
    }
    
    // Child elements are rendered by the main UI system
    // The layout container just manages their positions
}

void LayoutContainer::handleEvent(const SDL_Event &e) {
    // Container itself doesn't handle events
    // Child elements handle their own events
}

void LayoutContainer::setLayout(std::unique_ptr<Layout> newLayout) {
    layout = std::move(newLayout);
    if (layout) {
        updateLayout();
    }
}

void LayoutContainer::updateLayout() {
    if (!layout) return;
    
    layout->updateLayout(x, y, width, height);
    
    if (autoResize) {
        auto [prefWidth, prefHeight] = layout->getPreferredSize();
        
        // Add bounds checking and prevent zero dimensions
        prefWidth = std::max(1, prefWidth);
        prefHeight = std::max(1, prefHeight);
        
        // Only resize if there's a significant difference to prevent thrashing
        if (std::abs(prefWidth - width) > 1 || std::abs(prefHeight - height) > 1) {
            // Temporarily disable auto-resize to prevent infinite recursion
            bool wasAutoResize = autoResize;
            autoResize = false;
            
            setSize(prefWidth, prefHeight);
            layout->updateLayout(x, y, width, height);
            
            autoResize = wasAutoResize;
        }
    }
}

void LayoutContainer::addElement(std::shared_ptr<UIElement> element) {
    if (layout && element) {
        layout->addItem(element);
        updateLayout();
    }
}

void LayoutContainer::addElement(std::shared_ptr<UIElement> element, ui::SizePolicy hPolicy, ui::SizePolicy vPolicy) {
    if (layout && element) {
        layout->addItem(element, hPolicy, vPolicy);
        updateLayout();
    }
}

void LayoutContainer::addElement(std::shared_ptr<UIElement> element, int stretch) {
    if (layout && element) {
        layout->addItem(element, stretch);
        updateLayout();
    }
}

void LayoutContainer::removeElement(std::shared_ptr<UIElement> element) {
    if (layout) {
        layout->removeItem(element);
        updateLayout();
    }
}

void LayoutContainer::removeElement(const std::string& elementId) {
    if (layout) {
        layout->removeItem(elementId);
        updateLayout();
    }
}

void LayoutContainer::clearElements() {
    if (layout) {
        layout->clear();
        updateLayout();
    }
}

void LayoutContainer::resizeToFit() {
    if (layout) {
        auto [prefWidth, prefHeight] = layout->getPreferredSize();
        setSize(prefWidth, prefHeight);
    }
}

std::pair<int, int> LayoutContainer::getPreferredSize() const {
    if (layout) {
        return layout->getPreferredSize();
    }
    return {width, height};
}

std::pair<int, int> LayoutContainer::getMinimumSize() const {
    if (layout) {
        return layout->getMinimumSize();
    }
    return {width, height};
}

void LayoutContainer::setSize(int newWidth, int newHeight) {
    UIElement::setSize(newWidth, newHeight);
    updateLayout();
}

void LayoutContainer::setPosition(int newX, int newY) {
    UIElement::setPosition(newX, newY);
    updateLayout();
}

void LayoutContainer::addElement(std::shared_ptr<UIElement> element, int row, int column) {
    if (auto* gridLayout = dynamic_cast<ui::GridLayout*>(layout.get())) {
        gridLayout->addItem(element, row, column);
        updateLayout();
    }
}

void LayoutContainer::addElement(std::shared_ptr<UIElement> element, int row, int column, int rowSpan, int columnSpan) {
    if (auto* gridLayout = dynamic_cast<ui::GridLayout*>(layout.get())) {
        gridLayout->addItem(element, row, column, rowSpan, columnSpan);
        updateLayout();
    }
}

void LayoutContainer::setBorderWidth(int width) {
    if (auto* gridLayout = dynamic_cast<ui::GridLayout*>(layout.get())) {
        gridLayout->setBorderWidth(width);
        updateLayout();
    }
}

} // namespace ui
