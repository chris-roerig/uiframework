#pragma once
#include "Color.h"

namespace ui {

struct ThemeableElementColors {
    Color background;
    Color foreground;
    Color highlight;
    Color activeText;
    Color inactiveText;
    Color buttonText;
    Color buttonBackground;
    Color buttonForeground;
    Color buttonBorderLight;
    Color buttonBorderDark;
    Color checkboxEnabled;
    Color checkboxDisabled;
    Color checkboxChecked;
    Color checkboxUnchecked;
    Color checkboxBorderLight;
    Color checkboxBorderDark;
    Color textInputBackground;
    Color textInputText;
    Color textInputBorderLight;
    Color textInputBorderDark;
    Color selectOptionUnselected;
    Color selectOptionSelected;
    Color selectOptionBorderLight;
    Color selectOptionBorderDark;
    Color labelText;
    Color labelBackground;
    Color contextMenuBackground;
    Color contextMenuText;
    Color contextMenuBorder;
};

class Theme {
public:
    virtual ~Theme() = default;
    virtual ThemeableElementColors labelColors() const = 0;
    virtual ThemeableElementColors buttonColors() const = 0;
    virtual ThemeableElementColors checkboxColors() const = 0;
    virtual ThemeableElementColors optionSelectColors() const = 0;
    virtual ThemeableElementColors textInputColors() const = 0;
    virtual ThemeableElementColors contextMenuColors() const = 0;
    virtual Color highlightColor() const = 0;
    virtual Color backgroundColor() const = 0;
    virtual Color foregroundColor() const = 0;
};

class ThemeBase : public Theme {
protected:
    Color defaultBackground;
    Color defaultForeground;
    Color defaultHighlight;
    Color defaultActiveText;
    Color defaultInactiveText;
    Color defaultButtonText;
    Color defaultButtonBackground;
    Color defaultButtonForeground;
    Color defaultButtonBorderLight;
    Color defaultButtonBorderDark;
    Color defaultCheckboxBorderLight;
    Color defaultCheckboxBorderDark;
    Color defaultCheckboxEnabled;
    Color defaultCheckboxDisabled;
    Color defaultCheckboxChecked;
    Color defaultCheckboxUnchecked;
    Color defaultSelectOptionUnselected;
    Color defaultSelectOptionSelected;
    Color defaultSelectOptionBorderLight;
    Color defaultSelectOptionBorderDark;
    Color defaultTextInputBackground;
    Color defaultTextInputText;
    Color defaultTextInputBorderLight;
    Color defaultTextInputBorderDark;
    Color defaultLabelText;
    Color defaultLabelBackground;
    Color defaultContextMenuBackground;
    Color defaultContextMenuText;
    Color defaultContextMenuBorder;
public:
    ThemeBase()
      : defaultBackground(0,0,0),
        defaultForeground(255,255,255),
        defaultHighlight(255,255,0),
        defaultActiveText(255,255,255),
        defaultInactiveText(128,128,128),
        defaultButtonText(255,255,255),
        defaultButtonBackground(150,150,150),
        defaultButtonForeground(255,255,255),
        defaultCheckboxEnabled(255,255,255),
        defaultCheckboxDisabled(200,200,200),
        defaultCheckboxChecked(255,255,255),
        defaultCheckboxUnchecked(0,0,0),
        defaultSelectOptionUnselected(200,200,200),
        defaultSelectOptionSelected(100,149,237),
        defaultTextInputBackground(255,255,255),
        defaultTextInputText(255,255,255),
        defaultLabelText(255,255,255),
        defaultLabelBackground(0,0,0)
    {}

    ThemeableElementColors labelColors() const override {
        ThemeableElementColors c;
        c.labelText = defaultLabelText;
        c.labelBackground = defaultLabelBackground;
        return c;
    }
    ThemeableElementColors buttonColors() const override {
        ThemeableElementColors c;
        c.buttonText = defaultButtonText;
        c.buttonBackground = defaultButtonBackground;
        c.buttonForeground = defaultButtonForeground;
        c.buttonBorderLight = defaultButtonBorderLight;
        c.buttonBorderDark  = defaultButtonBorderDark;
        return c;
    }
    ThemeableElementColors checkboxColors() const override {
        ThemeableElementColors c;
        c.checkboxEnabled = defaultCheckboxEnabled;
        c.checkboxDisabled = defaultCheckboxDisabled;
        c.checkboxChecked = defaultCheckboxChecked;
        c.checkboxUnchecked = defaultCheckboxUnchecked;
        c.checkboxBorderLight = defaultCheckboxBorderLight;
        c.checkboxBorderDark  = defaultCheckboxBorderDark;
        return c;
    }
    ThemeableElementColors optionSelectColors() const override {
        ThemeableElementColors c;
        c.selectOptionSelected = defaultSelectOptionSelected;
        c.selectOptionUnselected = defaultSelectOptionUnselected;
        c.selectOptionBorderLight = defaultSelectOptionBorderLight;
        c.selectOptionBorderDark = defaultSelectOptionBorderDark;
        return c;
    }
    ThemeableElementColors textInputColors() const override {
        ThemeableElementColors c;
        c.textInputBackground = defaultTextInputBackground;
        c.textInputText = defaultTextInputText;
        c.textInputBorderLight = defaultTextInputBorderLight;
        c.textInputBorderDark  = defaultTextInputBorderDark;
        return c;
    }
    ThemeableElementColors contextMenuColors() const override {
        ThemeableElementColors c;
        c.contextMenuBackground = defaultContextMenuBackground;
        c.contextMenuText = defaultContextMenuText;
        return c;
    }
    Color highlightColor() const override { return defaultHighlight; }
    Color backgroundColor() const override { return defaultBackground; }
    Color foregroundColor() const override { return defaultForeground; }
};

} // namespace ui
