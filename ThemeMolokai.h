#pragma once
#include "ThemeBase.h"

namespace ui {

class ThemeMolokai : public ThemeBase {
public:
    ThemeMolokai() {
        // Assign the Molokai colors directly.
        defaultBackground = Color(39, 40, 34);
        defaultForeground = Color(248, 248, 242);
        defaultHighlight  = Color(189, 147, 249);
        defaultActiveText = Color(248, 248, 242);
        defaultInactiveText = Color(128, 128, 128);

        defaultButtonText   = Color(248, 248, 242);
        defaultButtonBackground = Color(80, 80, 80);
        defaultButtonForeground = Color(248, 248, 242);
        defaultButtonBorderLight = Color(180, 180, 180);
        defaultButtonBorderDark  = Color(90, 90, 90);

        defaultCheckboxEnabled  = Color(255, 255, 255);
        defaultCheckboxDisabled = Color(180, 180, 180);
        defaultCheckboxChecked  = Color(248, 248, 242);
        defaultCheckboxUnchecked= Color(0, 0, 0);
        defaultCheckboxBorderLight = Color(180, 180, 180);
        defaultCheckboxBorderDark  = Color(90, 90, 90);

        defaultSelectOptionUnselected = Color(220, 220, 220);
        defaultSelectOptionSelected   = Color(150, 150, 150);
        defaultSelectOptionBorderLight = Color(180, 180, 180);
        defaultSelectOptionBorderDark  = Color(90, 90, 90);

        defaultTextInputBackground = Color(255, 255, 255);
        defaultTextInputText       = Color(248, 248, 242);
        defaultTextInputBorderLight = Color(180, 180, 180);
        defaultTextInputBorderDark  = Color(90, 90, 90);

        defaultLabelText      = Color(248, 248, 242);
        defaultLabelBackground= Color(39, 40, 34);
    }
};

} // namespace ui
