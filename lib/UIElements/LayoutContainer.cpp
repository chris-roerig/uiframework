#include "LayoutContainer.h"
#include "../../lib/Theme/ThemeBase.h"
#include "../../src/Helpers.h"
#include <SDL2/SDL.h>

namespace ui {

LayoutContainer::LayoutContainer(int x_, int y_, int w_, int h_, std::unique_ptr<Layout> layout_)
    : UIElement(x_, y_, w_, h_), layout(std::move(layout_)) {
    if (layout) {
        updateLayout();
    }
}

void LayoutContainer::render(SDL_Renderer* renderer, TTF_Font* font, std::shared_ptr<Theme> theme) {
    if (!renderer || !theme || !layout) {
        return;
    }
    
    // Draw container background (optional)
    if (hasFocus) {
        auto colors = theme->buttonColors();
        SDL_Rect focusRect = getFocusRect();
        SDL_SetRenderDrawColor(renderer, colors.buttonText.r, colors.buttonText.g, 
                             colors.buttonText.b, colors.buttonText.a);
        SDL_RenderDrawRect(renderer, &focusRect);
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
        if (prefWidth != width || prefHeight != height) {
            setSize(prefWidth, prefHeight);
            // Update layout again with new size
            layout->updateLayout(x, y, width, height);
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

} // namespace ui
