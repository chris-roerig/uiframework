#pragma once
#include "../UIElements/UIElement.h"
#include <vector>
#include <memory>
#include <functional>

namespace ui {

// Layout alignment options
enum class Alignment {
    Start,    // Left/Top
    Center,   // Center
    End,      // Right/Bottom
    Stretch   // Fill available space
};

// Layout sizing options
enum class SizePolicy {
    Fixed,      // Fixed size
    Expanding,  // Take available space
    Minimum,    // Minimum required size
    Maximum     // Maximum allowed size
};

// Margin and padding structure
struct Spacing {
    int left = 0;
    int top = 0;
    int right = 0;
    int bottom = 0;
    
    Spacing() = default;
    explicit Spacing(int all) : left(all), top(all), right(all), bottom(all) {}
    Spacing(int horizontal, int vertical) : left(horizontal), top(vertical), right(horizontal), bottom(vertical) {}
    Spacing(int l, int t, int r, int b) : left(l), top(t), right(r), bottom(b) {}
};

// Layout item wrapper
struct LayoutItem {
    std::shared_ptr<UIElement> element;
    SizePolicy horizontalPolicy = SizePolicy::Fixed;
    SizePolicy verticalPolicy = SizePolicy::Fixed;
    Alignment horizontalAlignment = Alignment::Start;
    Alignment verticalAlignment = Alignment::Start;
    Spacing margin;
    int stretch = 0; // Stretch factor for expanding items
    int minWidth = 0;
    int minHeight = 0;
    int maxWidth = INT_MAX;
    int maxHeight = INT_MAX;
    
    explicit LayoutItem(std::shared_ptr<UIElement> elem) : element(elem) {}
};

// Base Layout class
class Layout {
protected:
    std::vector<LayoutItem> items;
    Spacing padding;
    int spacing = 0; // Space between items
    
public:
    virtual ~Layout() = default;
    
    // Add elements to layout
    virtual void addItem(std::shared_ptr<UIElement> element);
    virtual void addItem(std::shared_ptr<UIElement> element, SizePolicy hPolicy, SizePolicy vPolicy);
    virtual void addItem(std::shared_ptr<UIElement> element, int stretch);
    
    // Remove elements
    virtual void removeItem(std::shared_ptr<UIElement> element);
    virtual void removeItem(const std::string& elementId);
    virtual void clear();
    
    // Layout configuration
    void setSpacing(int space) { spacing = space; }
    int getSpacing() const { return spacing; }
    
    void setPadding(const Spacing& pad) { padding = pad; }
    const Spacing& getPadding() const { return padding; }
    
    // Layout calculation - must be implemented by subclasses
    virtual void updateLayout(int x, int y, int width, int height) = 0;
    
    // Get layout requirements
    virtual std::pair<int, int> getMinimumSize() const = 0;
    virtual std::pair<int, int> getPreferredSize() const = 0;
    
    // Get all elements in this layout
    const std::vector<LayoutItem>& getItems() const { return items; }
    
    // Utility methods
    bool isEmpty() const { return items.empty(); }
    size_t getItemCount() const { return items.size(); }
    
protected:
    // Helper methods for subclasses
    int calculateTotalSpacing() const;
    std::pair<int, int> calculateContentSize(int availableWidth, int availableHeight) const;
};

} // namespace ui
