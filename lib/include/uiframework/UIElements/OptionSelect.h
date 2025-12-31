#pragma once
#include "UIElement.h"
#include <vector>
#include <string>
#include <functional>

namespace ui {

class OptionSelect : public UIElement {
private:
    bool expanded = false;     // whether drop-down is open
    int hoveredIndex = -1;     // index being hovered over in dropdown
    int currentIndex = 0;      // currently selected option
    
    // String caching for performance
    struct StringCache {
        std::string originalText;
        int availableWidth = -1;
        std::string truncatedText;
        bool valid = false;
    };
    mutable StringCache displayCache;
    mutable std::vector<StringCache> dropdownCache;
    mutable int lastWidth = -1, lastHeight = -1;
    
    // Helper methods
    void renderCollapsed(SDL_Renderer* renderer, TTF_Font* font, std::shared_ptr<class Theme> theme);
    void renderExpanded(SDL_Renderer* renderer, TTF_Font* font, std::shared_ptr<class Theme> theme);
    std::string getCachedTruncatedText(const std::string& text, TTF_Font* font, int availableWidth, StringCache& cache) const;
    bool isValidIndex(int index) const;
    
public:
    std::vector<std::string> options;
    std::function<void(int)> onSelect;
    
    OptionSelect(int x_, int y_, int w_, int h_, int current, const std::vector<std::string>& opts, std::function<void(int)> callback = nullptr)
      : UIElement(x_, y_, w_, h_), currentIndex(current), options(opts), onSelect(callback) {
        if (!isValidIndex(currentIndex)) {
            currentIndex = options.empty() ? -1 : 0;
        }
    }
    
    void handleEvent(const SDL_Event &e) override;
    bool isInteractive() const override { return true; }
    SDL_Rect getFocusRect() const override;
    void activate() override;

protected:
    void renderImpl(const RenderContext& ctx) override;
    
public:
    // Focus management
    void onFocusLost() override;
    
    // Cache management
    void invalidateStringCache();
    
    // Safe option management
    void setOptions(const std::vector<std::string>& newOptions);
    void addOption(const std::string& option);
    void removeOption(int index);
    
    // OptionSelect-specific methods
    void setSelectedIndex(int index);
    int getSelectedIndex() const { return currentIndex; }
    const std::string& getSelectedOption() const;
    void clearOptions();
    bool isExpanded() const { return expanded; }
    void collapse() { expanded = false; hoveredIndex = -1; }
    void expand() { expanded = true; }
};

} // namespace ui
