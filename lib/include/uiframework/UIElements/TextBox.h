#pragma once
#include "InteractiveElement.h"
#include <vector>
#include <string>
#include <functional>

namespace ui {

class TextBox : public InteractiveElement {
private:
    bool textSelected = false;   // true if text is selected
    size_t cursorPosition = 0;   // cursor position in text
    
    // Real-time double buffering
    std::string textBuffers[2];
    bool enabledBuffers[2];
    std::atomic<int> currentTextBuffer{0};
    std::atomic<int> currentEnabledBuffer{0};
    
    // String caching for performance
    struct StringCache {
        std::string originalText;
        int availableWidth = -1;
        std::string truncatedText;
        bool valid = false;
    };
    mutable StringCache displayCache;
    mutable int lastWidth = -1, lastHeight = -1;
    
    // Validation state
    bool validationEnabled = true;
    bool isValid = true;
    std::string lastValidationError;
    bool showValidationError = false;
    
    // Placeholder text state
    std::string placeholderText;
    bool showPlaceholder = true;
    
    // Helper method for cached truncation
    std::string getCachedTruncatedText(const std::string& text, TTF_Font* font, int availableWidth) const;
    
public:
    std::string content;
    bool autoHighlight;  // auto-highlight on focus; default true
    
    TextBox(int x_, int y_, int w_, int h_, const std::string& defaultText = "", bool autoHighlight_ = true)
      : InteractiveElement(x_, y_, w_, h_), content(defaultText), autoHighlight(autoHighlight_) {
        cursorPosition = content.length();
        
        // Initialize validation state
        validationEnabled = true;
        isValid = true;
        showValidationError = false;
        
        // Initialize placeholder state
        showPlaceholder = content.empty();
    }
    
    SDL_Rect getFocusRect() const override;
    void activate() override;

protected:
    // InteractiveElement event handlers
    void onMouseDown(int x, int y) override;
    void onTextInput(const std::string& text) override;
    void onKeyDown(const SDL_Keycode& key) override;

public:

protected:
    void renderImpl(const RenderContext& ctx) override;
    
public:
    
    // Sizing API implementation
    std::pair<int, int> getPreferredSize(TTF_Font* font) const override;
    std::pair<int, int> getMinimumSize() const override;
    void autoSize(TTF_Font* font) override;
    
    // Focus management
    void onFocusGained() override;
    void onFocusLost() override;
    
    // Cache management
    void invalidateStringCache();
    
    // TextBox-specific methods
    void setText(const std::string& text);
    const std::string& getText() const { return content; }
    void clear() { setText(""); }
    void selectAll();
    void clearSelection();
    bool hasSelection() const { return textSelected; }
    
    // Real-time safe methods (lock-free, audio thread safe)
    void realtimeSetText(const std::string& newText);
    void realtimeSetEnabled(bool enabled);
    
    // Validation methods
    void setValidationEnabled(bool enabled) { validationEnabled = enabled; }
    bool isValidationEnabled() const { return validationEnabled; }
    bool isInputValid() const { return isValid; }
    const std::string& getLastValidationError() const { return lastValidationError; }
    void setShowValidationError(bool show) { showValidationError = show; }
    bool isShowingValidationError() const { return showValidationError; }
    
    // Validate current content
    bool validateCurrentInput();
    
    // Placeholder text methods
    void setPlaceholder(const std::string& placeholder) { 
        placeholderText = placeholder; 
        showPlaceholder = content.empty();
    }
    const std::string& getPlaceholder() const { return placeholderText; }
    bool isShowingPlaceholder() const { return showPlaceholder && content.empty(); }
};

} // namespace ui
