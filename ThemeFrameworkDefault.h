#pragma once
#include "ThemeBase.h"

namespace ui {

class ThemeFrameworkDefault : public ThemeBase {
public:
    ThemeFrameworkDefault() {
        defaultBackground = Color(30, 30, 30);
        defaultForeground = Color(220, 220, 220);
        defaultHighlight  = Color(255, 165, 0);
        defaultActiveText = Color(220, 220, 220);
        defaultInactiveText = Color(128, 128, 128);

        defaultButtonText = Color(220, 220, 220);
        defaultButtonBackground = Color(50, 50, 50);
        defaultButtonForeground = Color(220, 220, 220);
        defaultButtonBorderLight = Color(200, 200, 200);
        defaultButtonBorderDark  = Color(80, 80, 80);

        defaultCheckboxEnabled  = Color(255, 255, 255);
        defaultCheckboxDisabled = Color(100, 100, 100);
        defaultCheckboxChecked  = Color(220, 220, 220);
        defaultCheckboxUnchecked = Color(80, 80, 80);
        defaultCheckboxBorderLight = Color(210, 210, 210);
        defaultCheckboxBorderDark  = Color(90, 90, 90);

        defaultSelectOptionUnselected = Color(70, 70, 70);
        defaultSelectOptionSelected   = Color(255, 165, 0);
        defaultSelectOptionBorderLight = Color(200, 200, 200);
        defaultSelectOptionBorderDark  = Color(80, 80, 80);

        defaultTextInputBackground = Color(50, 50, 50);
        defaultTextInputText       = Color(220, 220, 220);
        defaultTextInputBorderLight = Color(200, 200, 200);
        defaultTextInputBorderDark  = Color(80, 80, 80);

        defaultLabelText      = Color(220, 220, 220);
        defaultLabelBackground = Color(30, 30, 30);

        defaultContextMenuBackground = Color(60, 60, 60);
        defaultContextMenuText = Color(220, 220, 220);
        defaultContextMenuBorder = Color(40, 40, 40);
  
    }
};

} // namespace ui
