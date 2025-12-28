#pragma once
#include "Layout.h"

namespace ui {

// Horizontal Box Layout - arranges elements horizontally
class HBoxLayout : public Layout {
public:
    HBoxLayout() = default;
    explicit HBoxLayout(int spacing);
    
    // Layout implementation
    void updateLayout(int x, int y, int width, int height) override;
    std::pair<int, int> getMinimumSize() const override;
    std::pair<int, int> getPreferredSize() const override;
    
    // HBox-specific methods
    void setAlignment(Alignment align) { alignment = align; }
    Alignment getAlignment() const { return alignment; }
    
private:
    Alignment alignment = Alignment::Start;
    
    // Helper methods
    int calculateTotalMinWidth() const;
    int calculateTotalPreferredWidth() const;
    int calculateMaxHeight() const;
    std::vector<int> distributeSpace(int availableWidth) const;
};

} // namespace ui
