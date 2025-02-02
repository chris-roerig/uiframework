#pragma once
#include "ThemeBase.h"

namespace ui {

class ThemeSolarizedLight : public ThemeBase {
public:
    ThemeSolarizedLight() {
        // Solarized Light palette (adapted)
        defaultBackground = Color(253, 246, 227);         // Light background (Base3)
        defaultForeground = Color(101, 123, 131);           // Dark text (Base00)
        defaultHighlight  = Color(203, 75, 22);             // Orange accent
        defaultActiveText = Color(101, 123, 131);
        defaultInactiveText = Color(150, 150, 150);

        // Button Colors:
        defaultButtonText = Color(101, 123, 131);
        defaultButtonBackground = Color(180, 180, 180);
        defaultButtonForeground = Color(101, 123, 131);
        defaultButtonBorderLight = Color(220, 220, 220);
        defaultButtonBorderDark  = Color(150, 150, 150);

        // Checkbox Colors:
        defaultCheckboxEnabled  = Color(101, 123, 131);
        defaultCheckboxDisabled = Color(150, 150, 150);
        defaultCheckboxChecked  = Color(101, 123, 131);
        defaultCheckboxUnchecked= Color(0, 0, 0);
        defaultCheckboxBorderLight = Color(220, 220, 220);
        defaultCheckboxBorderDark  = Color(150, 150, 150);

        // OptionSelect Colors:
        defaultSelectOptionUnselected = Color(150, 150, 150);
        defaultSelectOptionSelected   = Color(203, 75, 22);
        defaultSelectOptionBorderLight = Color(220, 220, 220);
        defaultSelectOptionBorderDark  = Color(150, 150, 150);

        // TextInput Colors:
        defaultTextInputBackground = Color(255, 255, 255);  // White input background
        defaultTextInputText       = Color(101, 123, 131);    // Dark text
        defaultTextInputBorderLight = Color(220, 220, 220);
        defaultTextInputBorderDark  = Color(150, 150, 150);

        // Label Colors:
        defaultLabelText      = Color(101, 123, 131);    // Dark text
        defaultLabelBackground= Color(253, 246, 227);        // Same as the main background

        // ContextMenu Colors:
        // The context menu bar background should be distinct from the main background.
        defaultContextMenuBackground = Color(240, 230, 200);  // A soft beige tone
        defaultContextMenuText = Color(55, 64, 74);           // Dark text for contrast
        defaultContextMenuBorder = Color(150, 150, 150);       // Border to separate the menu visually
        defaultContextMenuHighlight = Color(203, 75, 22);      // Use the same accent as highlight
    }
};

} // namespace ui
