#pragma once
#include "Layout.h"

namespace ui {

// Vertical Box Layout - arranges elements vertically
class VBoxLayout : public Layout {
public:
    VBoxLayout() = default;
    explicit VBoxLayout(int spacing);
    
    // Layout implementation
    void updateLayout(int x, int y, int width, int height) override;
    std::pair<int, int> getMinimumSize() const override;
    std::pair<int, int> getPreferredSize() const override;
    
    // VBox-specific methods
    void setAlignment(Alignment align) { alignment = align; }
    Alignment getAlignment() const { return alignment; }
    
private:
    Alignment alignment = Alignment::Start;
    
    // Helper methods
    int calculateTotalMinHeight() const;
    int calculateTotalPreferredHeight() const;
    int calculateMaxWidth() const;
    std::vector<int> distributeSpace(int availableHeight) const;
};

} // namespace ui
