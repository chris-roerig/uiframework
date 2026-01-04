#include "uiframework/UIElements/TextBox.h"
#include "uiframework/Theme/ThemeBase.h"
#include "uiframework/Helpers.h"
#include "uiframework/UICore.h"
#include "uiframework/Constants.h"
#include "uiframework/ErrorHandling.h"
#include "uiframework/Utils/TextUtils.h"
#include "uiframework/Utils/BorderRenderer.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <algorithm>

namespace ui {

std::string TextBox::getCachedTruncatedText(const std::string& text, TTF_Font* font, int availableWidth) const {
    // Check if cache is valid
    if (displayCache.valid && displayCache.originalText == text && displayCache.availableWidth == availableWidth) {
        return displayCache.truncatedText;
    }
    
    // Cache miss - use TextUtils for truncation
    displayCache.originalText = text;
    displayCache.availableWidth = availableWidth;
    displayCache.truncatedText = TextUtils::truncateWithEllipsis(text, font, availableWidth);
    displayCache.valid = true;
    
    return displayCache.truncatedText;
}

void TextBox::invalidateStringCache() {
    displayCache.valid = false;
}

void TextBox::renderImpl(const RenderContext& ctx) {
    // Process real-time updates
    int textBufferIndex = currentTextBuffer.load();
    if (!textBuffers[textBufferIndex].empty()) {
        content = textBuffers[textBufferIndex];
        cursorPosition = content.length();
        invalidateStringCache();
    }
    
    int enabledBufferIndex = currentEnabledBuffer.load();
    if (enabledBuffers[enabledBufferIndex] != enabled) {
        enabled = enabledBuffers[enabledBufferIndex];
    }
    
    // Check if dimensions changed and invalidate cache if needed
    if (width != lastWidth || height != lastHeight) {
        invalidateStringCache();
        lastWidth = width;
        lastHeight = height;
    }
    
    const int padding = ui::Constants::DEFAULT_PADDING;
    int boxHeight = height;
    if (ctx.font) {
        boxHeight = std::max(height, TTF_FontLineSkip(ctx.font) + 2 * padding);
    }
    
    SDL_Rect rect = { x, y, width, boxHeight };
    ThemeableElementColors tc = ctx.textInputColors();
    
    // Determine colors based on enabled state and validation
    Color bgColor = enabled ? tc.textInputBackground : tc.textInputDisabled;
    Color textColor = enabled ? tc.textInputText : tc.textInputTextDisabled;
    
    // Override colors for validation errors
    if (showValidationError && !isValid && enabled) {
        // Use error colors - red tint for background and border
        bgColor = Color(255, 240, 240, 255);  // Light red background
        textColor = Color(180, 0, 0, 255);    // Dark red text
    }
    
    // Draw background
    drawFilledRect(ctx.renderer, rect, bgColor);
    
    // Draw border with validation error styling
    if (showValidationError && !isValid && enabled) {
        // Draw red error border
        Color errorBorder = Color(220, 0, 0, 255);
        SDL_SetRenderDrawColor(ctx.renderer, errorBorder.r, errorBorder.g, errorBorder.b, errorBorder.a);
        SDL_RenderDrawRect(ctx.renderer, &rect);
        
        // Draw inner red border for emphasis
        SDL_Rect innerRect = { rect.x + 1, rect.y + 1, rect.w - 2, rect.h - 2 };
        SDL_RenderDrawRect(ctx.renderer, &innerRect);
    } else {
        // Draw normal 3D border
        ui::BorderRenderer::draw3DBorder(ctx.renderer, rect, tc.textInputBorderLight, tc.textInputBorderDark);
    }
    
    // Draw focus indicator (only when enabled)
    if (hasFocus && enabled) {
        SDL_Rect focusRect = getFocusRect();
        SDL_SetRenderDrawColor(ctx.renderer, textColor.r, textColor.g, textColor.b, textColor.a);
        SDL_RenderDrawRect(ctx.renderer, &focusRect);
    }
    
    // Render text content or placeholder
    if (ctx.font) {
        bool shouldShowPlaceholder = content.empty() && !placeholderText.empty() && !hasFocus;
        std::string displayContent = shouldShowPlaceholder ? placeholderText : content;
        
        if (!displayContent.empty()) {
            int availableWidth = width - 2 * padding;
            if (availableWidth <= 0) return;
            
            std::string displayText = getCachedTruncatedText(displayContent, ctx.font, availableWidth);
            
            if (!displayText.empty()) {
                // Use placeholder styling if showing placeholder
                Color renderTextColor = shouldShowPlaceholder ? 
                    Color(textColor.r, textColor.g, textColor.b, 128) : // 50% opacity for placeholder
                    textColor;
                
                SDL_Color sdlTextColor = { renderTextColor.r, renderTextColor.g, renderTextColor.b, renderTextColor.a };
                SDL_Surface* surface = nullptr;
                
                if (textSelected && enabled && !shouldShowPlaceholder) {
                    // Render selected text with highlighted background (only when enabled)
                    SDL_Color bgColor = { textColor.r, textColor.g, textColor.b, textColor.a };
                    SDL_Color selTextColor = { bgColor.r, bgColor.g, bgColor.b, bgColor.a };
                    surface = TTF_RenderText_Shaded(ctx.font, displayText.c_str(), selTextColor, bgColor);
                } else {
                    surface = TTF_RenderText_Solid(ctx.font, displayText.c_str(), sdlTextColor);
                }
                
                if (surface) {
                    SDL_Texture* texture = SDL_CreateTextureFromSurface(ctx.renderer, surface);
                    if (texture) {
                        SDL_Rect contentRect = getContentRect();
                        SDL_Rect dst = { contentRect.x + padding, contentRect.y + padding, surface->w, surface->h };
                        SDL_RenderCopy(ctx.renderer, texture, nullptr, &dst);
                        SDL_DestroyTexture(texture);
                    } else {
                        SDL_FreeSurface(surface);
                        return;
                    }
                    SDL_FreeSurface(surface);
                }
            }
        }
        
        // Draw cursor if focused and not selected and not showing placeholder
        if (hasFocus && !textSelected && !shouldShowPlaceholder) {
            int cursorX = x + padding;
            if (cursorPosition > 0 && cursorPosition <= content.length()) {
                std::string beforeCursor = content.substr(0, cursorPosition);
                int beforeWidth = 0;
                if (TTF_SizeText(ctx.font, beforeCursor.c_str(), &beforeWidth, nullptr) == 0) {
                    cursorX += beforeWidth;
                }
            }
            SDL_Color textColor = { tc.textInputText.r, tc.textInputText.g, tc.textInputText.b, tc.textInputText.a };
            SDL_SetRenderDrawColor(ctx.renderer, textColor.r, textColor.g, textColor.b, textColor.a);
            SDL_Rect contentRect = getContentRect();
            SDL_RenderDrawLine(ctx.renderer, cursorX, contentRect.y + padding, cursorX, contentRect.y + contentRect.h - padding);
        }
    }
}

void TextBox::onMouseDown(int x, int y) {
    // Focus is automatically handled by InteractiveElement
}

void TextBox::onTextInput(const std::string& inputText) {
    if (!inputText.empty()) {
        std::string newContent;
        
        if (textSelected) {
            // Replace selected text
            newContent = inputText;
        } else {
            // Insert at cursor position
            newContent = content;
            newContent.insert(cursorPosition, inputText);
        }
        
        // Validate input if validation is enabled
        if (validationEnabled && !validateInput(newContent)) {
            // Input is invalid - show error but don't update content
            isValid = false;
            lastValidationError = getValidationError();
            showValidationError = true;
            return;
        }
        
        // Input is valid - update content
        if (textSelected) {
            content = inputText;
            cursorPosition = inputText.length();
            textSelected = false;
        } else {
            content.insert(cursorPosition, inputText);
            cursorPosition += inputText.length();
        }
        
        isValid = true;
        showValidationError = false;
        showPlaceholder = content.empty();  // Update placeholder state
        invalidateStringCache();
    }
}

void TextBox::onKeyDown(const SDL_Keycode& key) {
    switch (key) {
        case SDLK_BACKSPACE:
            if (textSelected) {
                content.clear();
                cursorPosition = 0;
                textSelected = false;
                showPlaceholder = true;  // Update placeholder state
                invalidateStringCache();
            } else if (cursorPosition > 0) {
                content.erase(cursorPosition - 1, 1);
                cursorPosition--;
                showPlaceholder = content.empty();  // Update placeholder state
                invalidateStringCache();
            }
            break;
        case SDLK_DELETE:
            if (textSelected) {
                content.clear();
                cursorPosition = 0;
                textSelected = false;
                showPlaceholder = true;  // Update placeholder state
                invalidateStringCache();
            } else if (cursorPosition < content.length()) {
                content.erase(cursorPosition, 1);
                showPlaceholder = content.empty();  // Update placeholder state
                invalidateStringCache();
            }
            break;
        case SDLK_LEFT:
            if (cursorPosition > 0) {
                cursorPosition--;
            }
            textSelected = false;
            break;
        case SDLK_RIGHT:
            if (cursorPosition < content.length()) {
                cursorPosition++;
            }
            textSelected = false;
            break;
        case SDLK_HOME:
            cursorPosition = 0;
            textSelected = false;
            break;
        case SDLK_END:
            cursorPosition = content.length();
            textSelected = false;
            break;
        case SDLK_a:
            if (SDL_GetModState() & KMOD_CTRL) {
                selectAll();
            }
            break;
        case SDLK_RETURN:
        case SDLK_KP_ENTER:
            activate();
            break;
    }
}

void TextBox::activate() {
    // Default activation clears selection
    textSelected = false;
}

SDL_Rect TextBox::getFocusRect() const {
    return SDL_Rect{ x - 2, y - 2, width + 4, height + 4 };
}

void TextBox::onFocusGained() {
    hasFocus = true;
    SDL_StartTextInput();
    if (autoHighlight) {
        selectAll();
    }
}

void TextBox::onFocusLost() {
    hasFocus = false;
    textSelected = false;
    SDL_StopTextInput();
    
    // Validate input when focus is lost
    if (validationEnabled) {
        validateCurrentInput();
    }
}

void TextBox::setText(const std::string& text) {
    // Validate input if validation is enabled
    if (validationEnabled && !validateInput(text)) {
        isValid = false;
        lastValidationError = getValidationError();
        showValidationError = true;
        return;
    }
    
    content = text;
    // Validate cursor position
    cursorPosition = std::min(cursorPosition, content.length());
    textSelected = false;
    isValid = true;
    showValidationError = false;
    showPlaceholder = content.empty();  // Update placeholder state
    invalidateStringCache();
}

void TextBox::selectAll() {
    textSelected = !content.empty();
}

void TextBox::clearSelection() {
    textSelected = false;
}

std::pair<int, int> TextBox::getPreferredSize(TTF_Font* font) const {
    if (!font) {
        return {width, height};
    }
    
    // Base size on content or placeholder
    std::string sampleText = content.empty() ? "Sample Text" : content;
    auto [textW, textH] = TextUtils::getTextSize(sampleText, font);
    return {textW + 20, textH + 8}; // Add padding for cursor and borders
}

std::pair<int, int> TextBox::getMinimumSize() const {
    return {60, 20}; // Minimum for cursor visibility and usability
}

void TextBox::autoSize(TTF_Font* font) {
    auto [prefW, prefH] = getPreferredSize(font);
    width = prefW;
    height = prefH;
}

// Real-time safe methods (lock-free, audio thread safe)
void TextBox::realtimeSetText(const std::string& newText) {
    int nextBuffer = 1 - currentTextBuffer.load();
    textBuffers[nextBuffer] = newText;
    currentTextBuffer.store(nextBuffer);
}

void TextBox::realtimeSetEnabled(bool enabled) {
    int nextBuffer = 1 - currentEnabledBuffer.load();
    enabledBuffers[nextBuffer] = enabled;
    currentEnabledBuffer.store(nextBuffer);
}

// Validation methods
bool TextBox::validateCurrentInput() {
    if (!validationEnabled) {
        isValid = true;
        showValidationError = false;
        return true;
    }
    
    isValid = validateInput(content);
    if (!isValid) {
        lastValidationError = getValidationError();
        showValidationError = true;
    } else {
        showValidationError = false;
    }
    
    return isValid;
}

} // namespace ui
