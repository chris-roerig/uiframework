#include "uiframework/UIElements/UIElement.h"
#include "uiframework/UICore.h"
#include "uiframework/Resources/SDLTextureRAII.h"
#include "uiframework/ErrorHandling.h"
#include <SDL2/SDL_ttf.h>

namespace ui {

TextCacheEntry* UIElement::getCachedText(const std::string& key, const std::string& text, 
                                         SDL_Color color, SDL_Renderer* renderer, TTF_Font* font) const {
    auto it = textCache.find(key);
    
    // Check if we have a valid cached entry
    if (it != textCache.end() && it->second) {
        auto& entry = it->second;
        if (entry->text == text && 
            entry->color.r == color.r && entry->color.g == color.g && 
            entry->color.b == color.b && entry->color.a == color.a) {
            
            // Phase 1: Update LRU on cache hit
            auto lruIt = std::find(textCacheLRU.begin(), textCacheLRU.end(), key);
            if (lruIt != textCacheLRU.end()) {
                textCacheLRU.erase(lruIt);
            }
            textCacheLRU.push_front(key);
            
            return entry.get();
        }
    }
    
    // Create new texture
    if (!renderer || !font || text.empty()) {
        return nullptr;
    }
    
    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
    if (!ErrorHandling::validateSurface(surface)) {
        return nullptr;
    }
    
    // Get dimensions before creating texture
    int width = surface->w;
    int height = surface->h;
    
    SDLTextureRAII textureRAII(renderer, surface);
    SDL_FreeSurface(surface);
    
    if (!textureRAII) {
        return nullptr;
    }
    
    // Phase 1: Check cache size before adding
    if (textCache.size() >= MAX_TEXT_CACHE_ENTRIES) {
        evictOldestTextCache();
    }
    
    // Create new cache entry
    auto entry = std::make_unique<TextCacheEntry>();
    entry->texture = textureRAII.release(); // Transfer ownership
    entry->width = width;
    entry->height = height;
    entry->text = text;
    entry->color = color;
    
    // Store in cache and return pointer
    TextCacheEntry* result = entry.get();
    textCache[key] = std::move(entry);
    
    // Phase 1: Add to LRU list
    textCacheLRU.push_front(key);
    
    return result;
}

void UIElement::invalidateTextCache() const {
    textCache.clear();
    textCacheLRU.clear();
}

// Phase 1: Text cache eviction
void UIElement::evictOldestTextCache() const {
    if (textCacheLRU.empty()) {
        return;
    }
    
    // Remove the least recently used text cache entry
    std::string oldestKey = textCacheLRU.back();
    textCacheLRU.pop_back();
    
    auto it = textCache.find(oldestKey);
    if (it != textCache.end()) {
        textCache.erase(it);
    }
}

// Phase 2: Font API implementation
void UIElement::setFont(const std::string& familyName, int size, FontStyle style) {
    fontFamily = familyName;
    fontSize = size;
    fontStyle = style;
    useThemeFont = false;
    invalidateTextCache(); // Clear cache when font changes
}

void UIElement::setThemeFont(FontType type, int size) {
    fontType = type;
    fontSize = size;
    useThemeFont = true;
    invalidateTextCache(); // Clear cache when font changes
}

void UIElement::setFontSize(int size) {
    fontSize = size;
    invalidateTextCache(); // Clear cache when font changes
}

void UIElement::setFontStyle(FontStyle style) {
    fontStyle = style;
    invalidateTextCache(); // Clear cache when font changes
}

TTF_Font* UIElement::getEffectiveFont() const {
    auto& fontManager = FontManager::getInstance();
    
    if (useThemeFont) {
        // Use theme-based font selection
        int effectiveSize = (fontSize > 0) ? fontSize : 12; // Default size if not specified
        return fontManager.getThemeFont(fontType, effectiveSize, fontStyle);
    } else if (!fontFamily.empty()) {
        // Use specific font family
        int effectiveSize = (fontSize > 0) ? fontSize : 12; // Default size if not specified
        return fontManager.getFont(fontFamily, effectiveSize, fontStyle);
    } else {
        // Fallback to legacy method for backward compatibility
        return fontManager.getFont("", (fontSize > 0) ? fontSize : 12);
    }
}

// Phase 3: Text positioning with baseline alignment
SDL_Point UIElement::calculateTextPosition(const std::string& text, const SDL_Rect& bounds, 
                                          TextAlignment alignment) const {
    if (text.empty()) {
        return {bounds.x, bounds.y};
    }
    
    auto& fontManager = FontManager::getInstance();
    
    // Get text dimensions and font metrics
    std::pair<int, int> textSize;
    FontMetrics metrics;
    
    if (useThemeFont) {
        int effectiveSize = (fontSize > 0) ? fontSize : 12;
        textSize = fontManager.getTextSize(text, "", effectiveSize, fontStyle); // Use theme font
        metrics = fontManager.getFontMetrics("", effectiveSize, fontStyle);
    } else if (!fontFamily.empty()) {
        int effectiveSize = (fontSize > 0) ? fontSize : 12;
        textSize = fontManager.getTextSize(text, fontFamily, effectiveSize, fontStyle);
        metrics = fontManager.getFontMetrics(fontFamily, effectiveSize, fontStyle);
    } else {
        // Fallback - use effective font directly
        TTF_Font* font = getEffectiveFont();
        if (font) {
            int w = 0, h = 0;
            TTF_SizeText(font, text.c_str(), &w, &h);
            textSize = {w, h};
            metrics.ascent = TTF_FontAscent(font);
            metrics.descent = TTF_FontDescent(font);
            metrics.height = TTF_FontHeight(font);
        } else {
            return {bounds.x, bounds.y};
        }
    }
    
    int textWidth = textSize.first;
    int textHeight = textSize.second;
    
    SDL_Point position = {bounds.x, bounds.y};
    
    // Calculate horizontal position
    switch (alignment) {
        case TextAlignment::TopCenter:
        case TextAlignment::MiddleCenter:
        case TextAlignment::BottomCenter:
        case TextAlignment::Baseline:
            position.x = bounds.x + (bounds.w - textWidth) / 2;
            break;
        case TextAlignment::TopRight:
        case TextAlignment::MiddleRight:
        case TextAlignment::BottomRight:
            position.x = bounds.x + bounds.w - textWidth;
            break;
        default: // Left alignment
            position.x = bounds.x;
            break;
    }
    
    // Calculate vertical position
    switch (alignment) {
        case TextAlignment::TopLeft:
        case TextAlignment::TopCenter:
        case TextAlignment::TopRight:
            position.y = bounds.y;
            break;
        case TextAlignment::MiddleLeft:
        case TextAlignment::MiddleCenter:
        case TextAlignment::MiddleRight:
            position.y = bounds.y + (bounds.h - textHeight) / 2;
            break;
        case TextAlignment::BottomLeft:
        case TextAlignment::BottomCenter:
        case TextAlignment::BottomRight:
            position.y = bounds.y + bounds.h - textHeight;
            break;
        case TextAlignment::Baseline:
            // Position text so baseline is at vertical center of bounds
            position.y = bounds.y + bounds.h / 2 - metrics.ascent;
            break;
    }
    
    return position;
}

// Phase 3: Get font metrics for current effective font
FontMetrics UIElement::getEffectiveFontMetrics() const {
    auto& fontManager = FontManager::getInstance();
    
    if (useThemeFont) {
        int effectiveSize = (fontSize > 0) ? fontSize : 12;
        return fontManager.getFontMetrics("", effectiveSize, fontStyle); // Use theme font
    } else if (!fontFamily.empty()) {
        int effectiveSize = (fontSize > 0) ? fontSize : 12;
        return fontManager.getFontMetrics(fontFamily, effectiveSize, fontStyle);
    } else {
        // Fallback - use effective font directly
        TTF_Font* font = getEffectiveFont();
        if (font) {
            return FontMetrics(
                TTF_FontAscent(font),
                TTF_FontDescent(font),
                TTF_FontLineSkip(font),
                TTF_FontHeight(font)
            );
        }
        return FontMetrics();
    }
}

// Constraint-based positioning implementation
void UIElement::setAnchor(std::shared_ptr<UIElement> target, AnchorType type, int offset) {
    if (!constraintManager) return; // Need constraint manager reference
    
    Constraint constraint(target, type, offset);
    constraintManager->addConstraint(this, constraint);
    
    // Calculate and apply new position
    updateConstraintPosition();
}

void UIElement::clearConstraints() {
    if (!constraintManager) return;
    constraintManager->removeConstraints(this);
}

bool UIElement::hasConstraints() const {
    if (!constraintManager) return false;
    return constraintManager->hasConstraints(const_cast<UIElement*>(this));
}

void UIElement::updateConstraintPosition() {
    if (!constraintManager) return;
    auto newPos = constraintManager->calculatePosition(this);
    if (newPos.first != -1 && newPos.second != -1) {
        setPosition(newPos.first, newPos.second);
    }
}

void UIElement::setConstraintManager(ConstraintManager* manager) {
    constraintManager = manager;
}

void UIElement::snapToGrid() {
    if (!gridSnappingEnabled || !constraintManager) return;
    
    int gridSize = constraintManager->getGridSize();
    
    if (GridSnap::isValidGridSize(gridSize)) {
        int newX = x, newY = y;
        GridSnap::snapPosition(newX, newY, gridSize);
        setPosition(newX, newY);
    }
}

void UIElement::setGridSnapping(bool enabled) {
    gridSnappingEnabled = enabled;
}

bool UIElement::isGridSnappingEnabled() const {
    return gridSnappingEnabled;
}

void UIElement::setRelativeSize(float widthPercent, float heightPercent) {
    if (PercentageSize::isValidPercentage(widthPercent) && 
        PercentageSize::isValidPercentage(heightPercent)) {
        hasRelativeSizing = true;
        relativeWidth = widthPercent;
        relativeHeight = heightPercent;
        updateRelativeSize();
    }
}

void UIElement::clearRelativeSize() {
    hasRelativeSizing = false;
    relativeWidth = 0.0f;
    relativeHeight = 0.0f;
}

bool UIElement::hasRelativeSize() const {
    return hasRelativeSizing;
}

void UIElement::updateRelativeSize() {
    if (!hasRelativeSizing || !coreRef) return;
    
    // Get parent dimensions from UI core
    int parentWidth = coreRef->getWidth();
    int parentHeight = coreRef->getHeight();
    
    int newWidth, newHeight;
    PercentageSize::calculateSize(relativeWidth, relativeHeight, 
                                 parentWidth, parentHeight, 
                                 newWidth, newHeight);
    setSize(newWidth, newHeight);
}

} // namespace ui
