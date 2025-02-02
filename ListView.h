#pragma once
#include "UICore.h"
#include <vector>
#include <string>
#include <functional>

namespace ui {

class ListView : public UIElement {
public:
    // The complete list of items.
    std::vector<std::string> items;
    // The items that match a filter; if empty, use items.
    std::vector<std::string> filteredItems;
    
    // If multiSelect is false, we use selectedIndex.
    // If multiSelect is true, selectedIndices stores indices (with respect to filteredItems).
    bool multiSelect = false;
    int selectedIndex = 0;
    std::vector<int> selectedIndices;
    
    // Vertical scroll offset (in number of items).
    int scrollOffset = 0;
    // Height for each item.
    int itemHeight;
    
    // Callback when selection changes; passes selected indices.
    std::function<void(const std::vector<int>&)> onSelectionChange;
    // Callback when an item is activated (e.g. by pressing Enter), with the index.
    std::function<void(int)> onItemActivated;
    
    // Constructor: x,y, width, height of the list area; itemHeight is the height of each item.
    ListView(int x_, int y_, int w_, int h_, int itemHeight_ = 30);
    
    virtual void render(SDL_Renderer* renderer) override;
    virtual void handleEvent(const SDL_Event &e) override;
    virtual SDL_Rect getFocusRect() const override;
    
    // Filtering: update filteredItems based on a substring (case-insensitive).
    void applyFilter(const std::string &filter);
    // Sorting: sorts the underlying items (and re-applies the filter).
    void sortItems(std::function<bool(const std::string&, const std::string&)> comp);
    virtual bool isInteractive() const override { return true; }   

    int getSelectedIndex() const;
    std::string getSelectedItem() const;
    void setSelectedIndex(int index); 
};

} // namespace ui
