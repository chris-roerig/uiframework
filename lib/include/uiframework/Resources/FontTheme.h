#pragma once
#include <string>

namespace ui {

/**
 * Theme interface for font selection - Phase 0 implementation
 * Provides theme-based font family and size selection
 */
class FontTheme {
public:
    virtual ~FontTheme() = default;
    
    // Font family selection
    virtual std::string getPrimaryFontFamily() const = 0;
    virtual std::string getMonospaceFontFamily() const = 0;
    virtual std::string getUIFontFamily() const = 0;
    
    // Font size selection
    virtual int getBaseFontSize() const = 0;
    virtual int getSmallFontSize() const { return getBaseFontSize() - 2; }
    virtual int getLargeFontSize() const { return getBaseFontSize() + 4; }
};

/**
 * Default framework theme - Phase 0 implementation
 * Uses embedded fonts for consistent cross-platform appearance
 */
class DefaultFontTheme : public FontTheme {
public:
    std::string getPrimaryFontFamily() const override { return "Roboto"; }
    std::string getMonospaceFontFamily() const override { return "Console"; }
    std::string getUIFontFamily() const override { return "Roboto"; }
    int getBaseFontSize() const override { return 12; }
};

/**
 * Console theme for development/debugging - Phase 0 implementation
 * Uses monospace fonts for all text
 */
class ConsoleFontTheme : public FontTheme {
public:
    std::string getPrimaryFontFamily() const override { return "Console"; }
    std::string getMonospaceFontFamily() const override { return "Console"; }
    std::string getUIFontFamily() const override { return "Console"; }
    int getBaseFontSize() const override { return 10; }
};

} // namespace ui
