#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <vector>
#include <functional>
#include <memory>
#include <unordered_map>
#include <list>
#include "../Constants.h"
#include "../Rendering/RenderContext.h"
#include "../Resources/FontManager.h"
#include "../Constraints/ConstraintManager.h"
#include "../Constraints/GridSnap.h"

namespace ui {

// Forward declaration
class UICore;

struct TextCacheEntry {
    SDL_Texture* texture = nullptr;
    int width = 0;
    int height = 0;
    std::string text;
    SDL_Color color;
    
    ~TextCacheEntry() {
        if (texture) {
            SDL_DestroyTexture(texture);
        }
    }
};

// Base class for all UI elements with proper resource management
class UIElement {
protected:
    std::string elementId;
    uint64_t numericId = 0; // Performance-optimized numeric ID
    UICore* coreRef = nullptr; // Non-owning reference to core
    
    // Phase 1: Text cache with size limits
    static constexpr size_t MAX_TEXT_CACHE_ENTRIES = 10;
    mutable std::unordered_map<std::string, std::unique_ptr<TextCacheEntry>> textCache;
    mutable std::list<std::string> textCacheLRU; // LRU tracking for text cache
    
    // Helper method for cached text rendering
    TextCacheEntry* getCachedText(const std::string& key, const std::string& text, 
                                  SDL_Color color, SDL_Renderer* renderer, TTF_Font* font) const;
    void invalidateTextCache() const;
    
    // Phase 1: Text cache management
    void evictOldestTextCache() const;
    
    // Phase 2: Font properties
    std::string fontFamily = "";  // Empty means use default
    int fontSize = 0;             // 0 means use default
    FontStyle fontStyle = FontStyle::Regular;
    FontType fontType = FontType::Primary;  // For theme-based selection
    bool useThemeFont = true;     // Whether to use theme-based font selection
    
public:
    int x, y, width, height;
    bool hasFocus = false;
    bool visible = true;
    bool enabled = true;
    
    // Tooltip support
    std::string tooltip;
    mutable bool showTooltip = false;
    mutable Uint32 hoverStartTime = 0;
    
    // Spacing support
    struct Spacing {
        int top, right, bottom, left;
        Spacing(int all = 0) : top(all), right(all), bottom(all), left(all) {}
        Spacing(int t, int r, int b, int l) : top(t), right(r), bottom(b), left(l) {}
    };
    
    Spacing padding{0};
    Spacing margin{0};
    
    UIElement(int x_, int y_, int w_, int h_)
      : x(x_), y(y_), width(w_), height(h_) {}
    
    virtual ~UIElement() = default;
    
    // Non-copyable due to unique_ptr in textCache
    UIElement(const UIElement&) = delete;
    UIElement& operator=(const UIElement&) = delete;
    
    // Movable
    UIElement(UIElement&&) = default;
    UIElement& operator=(UIElement&&) = default;
    
    // Core interface methods
    virtual void render(SDL_Renderer* renderer, TTF_Font* font, std::shared_ptr<class Theme> theme) final {
        RenderContext ctx = RenderContext::create(renderer, font, theme);
        if (ctx.isValid()) {
            renderImpl(ctx);
        }
    }
    
    virtual void handleEvent(const SDL_Event &e) {}
    virtual bool isInteractive() const { return false; }
    virtual SDL_Rect getFocusRect() const { 
        return SDL_Rect{ x - Constants::FOCUS_BORDER_WIDTH, y - Constants::FOCUS_BORDER_WIDTH, 
                        width + 2 * Constants::FOCUS_BORDER_WIDTH, height + 2 * Constants::FOCUS_BORDER_WIDTH }; 
    }
    virtual void activate() { /* default does nothing */ }

protected:
    virtual void renderImpl(const RenderContext& ctx) = 0;
    
public:
    
    // Sizing API - consistent across all elements
    virtual std::pair<int, int> getPreferredSize(TTF_Font* font) const { return {width, height}; }
    virtual std::pair<int, int> getMinimumSize() const { return {0, 0}; }
    virtual void autoSize(TTF_Font* font) { /* default does nothing */ }
    virtual bool hasFixedSize() const { return false; }
    
    // Focus event callbacks
    virtual void onFocusGained() { /* default does nothing */ }
    virtual void onFocusLost() { /* default does nothing */ }
    
    // Element management
    const std::string& getId() const { return elementId; }
    uint64_t getNumericId() const { return numericId; }
    void setId(const std::string& id) { elementId = id; }
    void setNumericId(uint64_t id) { numericId = id; }
    void setCoreReference(UICore* core) { coreRef = core; }
    
    // Utility methods
    bool containsPoint(int px, int py) const {
        return px >= x && px < x + width && py >= y && py < y + height;
    }
    
    void setPosition(int newX, int newY) {
        x = newX;
        y = newY;
    }
    
    void setSize(int newWidth, int newHeight) {
        width = newWidth;
        height = newHeight;
    }
    
    void setVisible(bool isVisible) {
        visible = isVisible;
    }
    
    bool isVisible() const {
        return visible;
    }
    
    void setEnabled(bool isEnabled) {
        enabled = isEnabled;
    }
    
    bool isEnabled() const {
        return enabled;
    }
    
    // Tooltip support
    void setTooltip(const std::string& text) { tooltip = text; }
    const std::string& getTooltip() const { return tooltip; }
    bool hasTooltip() const { return !tooltip.empty(); }
    
    // Tooltip state management (used by UICore)
    void setTooltipVisible(bool visible) const { showTooltip = visible; }
    bool isTooltipVisible() const { return showTooltip; }
    void setHoverStartTime(Uint32 time) const { hoverStartTime = time; }
    Uint32 getHoverStartTime() const { return hoverStartTime; }
    
    // Spacing API
    void setPadding(int all) { padding = Spacing(all); }
    void setPadding(int top, int right, int bottom, int left) { padding = Spacing(top, right, bottom, left); }
    void setMargin(int all) { margin = Spacing(all); }
    void setMargin(int top, int right, int bottom, int left) { margin = Spacing(top, right, bottom, left); }
    
    const Spacing& getPadding() const { return padding; }
    const Spacing& getMargin() const { return margin; }
    
    // Get content rectangle (element bounds minus padding)
    SDL_Rect getContentRect() const {
        return SDL_Rect{
            x + padding.left,
            y + padding.top,
            width - padding.left - padding.right,
            height - padding.top - padding.bottom
        };
    }
    
    // Phase 2: Font API
    void setFont(const std::string& familyName, int size, FontStyle style = FontStyle::Regular);
    void setThemeFont(FontType type, int size = 0);  // 0 means use theme default
    void setFontSize(int size);
    void setFontStyle(FontStyle style);
    
    // Get the actual font to use for rendering
    TTF_Font* getEffectiveFont() const;
    
    // Font property getters
    const std::string& getFontFamily() const { return fontFamily; }
    int getFontSize() const { return fontSize; }
    FontStyle getFontStyle() const { return fontStyle; }
    FontType getFontType() const { return fontType; }
    bool isUsingThemeFont() const { return useThemeFont; }
    
    // Phase 3: Text alignment support
    enum class TextAlignment {
        TopLeft, TopCenter, TopRight,
        MiddleLeft, MiddleCenter, MiddleRight,
        BottomLeft, BottomCenter, BottomRight,
        Baseline  // Proper baseline alignment
    };
    
    // Phase 3: Text positioning with baseline alignment
    SDL_Point calculateTextPosition(const std::string& text, const SDL_Rect& bounds, 
                                   TextAlignment alignment = TextAlignment::TopLeft) const;
    
    // Phase 3: Get font metrics for current effective font
    FontMetrics getEffectiveFontMetrics() const;

private:
    TextAlignment textAlignment = TextAlignment::TopLeft;
    
public:
    void setTextAlignment(TextAlignment alignment) { textAlignment = alignment; }
    TextAlignment getTextAlignment() const { return textAlignment; }
    
    // Constraint-based positioning
    void setAnchor(std::shared_ptr<UIElement> target, AnchorType type, int offset = 0);
    void clearConstraints();
    bool hasConstraints() const;
    void updateConstraintPosition();
    void setConstraintManager(ConstraintManager* manager);
    
    // Grid snapping
    void snapToGrid();
    void setGridSnapping(bool enabled);
    bool isGridSnappingEnabled() const;
    
    // Position getters for constraint calculations
    int getX() const { return x; }
    int getY() const { return y; }
    int getWidth() const { return width; }
    int getHeight() const { return height; }

private:
    ConstraintManager* constraintManager = nullptr;
    bool gridSnappingEnabled = false;
};

} // namespace ui
