#pragma once
#include "InteractiveElement.h"
#include <vector>
#include <string>
#include <functional>

namespace ui {

class ListView : public InteractiveElement {
private:
    int hoveredIndex = -1; // Index being hovered over
    
public:
    // The complete list of items
    std::vector<std::string> items;
    // The items that match a filter; if empty, use items
    std::vector<std::string> filteredItems;
    
    // Selection management
    bool multiSelect = false;
    int selectedIndex = 0;
    std::vector<int> selectedIndices;
    
    // Scrolling
    int scrollOffset = 0;
    int itemHeight;
    
    // Callbacks
    std::function<void(const std::vector<int>&)> onSelectionChange;
    std::function<void(int)> onItemActivated;
    
    // Constructor
    ListView(int x_, int y_, int w_, int h_, const std::vector<std::string>& items_, int itemHeight_ = 30);
    
    SDL_Rect getFocusRect() const override;
    void activate() override;

protected:
    void onMouseDown(int x, int y) override;
    void onKeyDown(const SDL_Keycode& key) override;

protected:
    void renderImpl(const RenderContext& ctx) override;
    
public:
    
    // Item management
    void setItems(const std::vector<std::string>& newItems);
    void addItem(const std::string& item);
    void removeItem(int index);
    void clearItems();
    
    // Selection management
    void setSelectedIndex(int index);
    void addToSelection(int index);
    void removeFromSelection(int index);
    void clearSelection();
    void selectAll();
    const std::vector<int>& getSelectedIndices() const { return selectedIndices; }
    int getSelectedIndex() const { return selectedIndex; }
    
    // Filtering
    void applyFilter(const std::string &filter);
    void clearFilter();
    
    // Scrolling
    void scrollToItem(int index);
    void scrollUp();
    void scrollDown();
    int getVisibleItemCount() const { return height / itemHeight; }
    
    // Utility methods
    bool isItemVisible(int index) const;
    int getItemAt(int mouseX, int mouseY) const;
    const std::vector<std::string>& getCurrentItems() const;
};

} // namespace ui
