#pragma once
#include "ThemeBase.h"

namespace ui {

class ThemeSolarizedLight : public ThemeBase {
public:
    ThemeSolarizedLight() {
        // Assign the Solarized Light colors directly.
        defaultBackground = Color(253, 246, 227);
        defaultForeground = Color(101, 123, 131);
        defaultHighlight  = Color(203, 75, 22);
        defaultActiveText = Color(101, 123, 131);
        defaultInactiveText = Color(150, 150, 150);

        defaultButtonText   = Color(101, 123, 131);
        defaultButtonBackground = Color(180, 180, 180);
        defaultButtonForeground = Color(101, 123, 131);

        defaultCheckboxEnabled  = Color(255, 255, 255);
        defaultCheckboxDisabled = Color(220, 220, 220);
        defaultCheckboxChecked  = Color(101, 123, 131);
        defaultCheckboxUnchecked= Color(0, 0, 0);

        defaultSelectOptionUnselected = Color(220, 220, 220);
        defaultSelectOptionSelected   = Color(80, 200, 120);

        defaultTextInputBackground = Color(255, 255, 255);
        defaultTextInputText       = Color(101, 123, 131);

        defaultLabelText      = Color(101, 123, 131);
        defaultLabelBackground= Color(253, 246, 227);
    }
};

} // namespace ui
