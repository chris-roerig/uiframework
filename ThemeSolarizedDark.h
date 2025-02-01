#pragma once
#include "ThemeBase.h"

namespace ui {

class ThemeSolarizedDark : public ThemeBase {
public:
    ThemeSolarizedDark() {
        // Assign the Solarized Dark colors directly.
        defaultBackground = Color(0, 43, 54);
        defaultForeground = Color(131, 148, 150);
        defaultHighlight  = Color(181, 137, 0);
        defaultActiveText = Color(131, 148, 150);
        defaultInactiveText = Color(100, 100, 100);

        defaultButtonText   = Color(131, 148, 150);
        defaultButtonBackground = Color(150, 150, 150);
        defaultButtonForeground = Color(131, 148, 150);
        defaultButtonBorderLight = Color(210, 210, 210);
        defaultButtonBorderDark  = Color(120, 120, 120);

        defaultCheckboxEnabled  = Color(255, 255, 255);
        defaultCheckboxDisabled = Color(200, 200, 200);
        defaultCheckboxChecked  = Color(131, 148, 150);
        defaultCheckboxUnchecked= Color(0, 0, 0);
        defaultCheckboxBorderLight = Color(210, 210, 210);
        defaultCheckboxBorderDark  = Color(120, 120, 120);

        defaultSelectOptionUnselected = Color(200, 200, 200);
        defaultSelectOptionSelected   = Color(100, 149, 237);
        defaultSelectOptionBorderLight = Color(210, 210, 210);
        defaultSelectOptionBorderDark  = Color(120, 120, 120);

        defaultTextInputBackground = Color(255, 255, 255);
        defaultTextInputText       = Color(131, 148, 150);
        defaultTextInputBorderLight = Color(210, 210, 210);
        defaultTextInputBorderDark  = Color(120, 120, 120);

        defaultLabelText      = Color(131, 148, 150);
        defaultLabelBackground= Color(0, 43, 54);
    }
};

} // namespace ui
