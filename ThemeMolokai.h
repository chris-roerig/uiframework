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

        defaultCheckboxEnabled  = Color(255, 255, 255);
        defaultCheckboxDisabled = Color(180, 180, 180);
        defaultCheckboxChecked  = Color(248, 248, 242);
        defaultCheckboxUnchecked= Color(0, 0, 0);

        defaultSelectOptionUnselected = Color(220, 220, 220);
        defaultSelectOptionSelected   = Color(150, 150, 150);

        defaultTextInputBackground = Color(255, 255, 255);
        defaultTextInputText       = Color(248, 248, 242);

        defaultLabelText      = Color(248, 248, 242);
        defaultLabelBackground= Color(39, 40, 34);
    }
};

} // namespace ui
