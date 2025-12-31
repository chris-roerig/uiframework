#include "uiframework/Utils/TextUtils.h"
#include "uiframework/Constants.h"
#include <algorithm>

namespace ui {

std::string TextUtils::truncateWithEllipsis(const std::string& text, TTF_Font* font, int maxWidth) {
    if (!font || maxWidth <= 0) {
        return "";
    }
    
    // Check if text already fits
    int textW = 0;
    if (TTF_SizeText(font, text.c_str(), &textW, nullptr) == 0 && textW <= maxWidth) {
        return text;
    }
    
    // Check if ellipsis fits
    int ellipsisW = 0;
    if (TTF_SizeText(font, ELLIPSIS, &ellipsisW, nullptr) != 0 || ellipsisW >= maxWidth) {
        return "";
    }
    
    // Binary search for optimal truncation point
    int left = 0, right = static_cast<int>(text.length());
    while (left < right) {
        int mid = (left + right + 1) / 2;
        std::string candidate = text.substr(0, mid) + ELLIPSIS;
        int candidateW = 0;
        if (TTF_SizeText(font, candidate.c_str(), &candidateW, nullptr) == 0 && candidateW <= maxWidth) {
            left = mid;
        } else {
            right = mid - 1;
        }
    }
    
    return left > 0 ? text.substr(0, left) + ELLIPSIS : "";
}

std::pair<int, int> TextUtils::getTextSize(const std::string& text, TTF_Font* font) {
    if (!font || text.empty()) {
        return {0, 0};
    }
    
    int width = 0, height = 0;
    if (TTF_SizeText(font, text.c_str(), &width, &height) != 0) {
        return {0, 0};
    }
    
    return {width, height};
}

bool TextUtils::textFitsWidth(const std::string& text, TTF_Font* font, int width) {
    auto [textWidth, textHeight] = getTextSize(text, font);
    return textWidth <= width;
}

std::vector<std::string> TextUtils::wrapText(const std::string& text, TTF_Font* font, int maxWidth) {
    std::vector<std::string> lines;
    
    if (!font || maxWidth <= 0 || text.empty()) {
        return lines;
    }
    
    // Simple word-based wrapping
    std::string currentLine;
    std::string word;
    
    for (char c : text) {
        if (c == ' ' || c == '\n') {
            // Check if adding this word would exceed width
            std::string testLine = currentLine.empty() ? word : currentLine + " " + word;
            
            if (textFitsWidth(testLine, font, maxWidth)) {
                currentLine = testLine;
            } else {
                // Word doesn't fit, start new line
                if (!currentLine.empty()) {
                    lines.push_back(currentLine);
                }
                currentLine = word;
            }
            
            word.clear();
            
            // Handle explicit line breaks
            if (c == '\n') {
                lines.push_back(currentLine);
                currentLine.clear();
            }
        } else {
            word += c;
        }
    }
    
    // Handle final word
    if (!word.empty()) {
        std::string testLine = currentLine.empty() ? word : currentLine + " " + word;
        if (textFitsWidth(testLine, font, maxWidth)) {
            currentLine = testLine;
        } else {
            if (!currentLine.empty()) {
                lines.push_back(currentLine);
            }
            currentLine = word;
        }
    }
    
    // Add final line
    if (!currentLine.empty()) {
        lines.push_back(currentLine);
    }
    
    return lines;
}

} // namespace ui
