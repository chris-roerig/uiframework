#pragma once
#include "ThemeBase.h"

namespace ui {

class ThemeMolokai : public ThemeBase {
public:
    ThemeMolokai() {
        // Molokai palette (adapted)
        defaultBackground = Color(39, 40, 34);             // Dark olive/gray background
        defaultForeground = Color(248, 248, 242);            // Near white
        defaultHighlight  = Color(189, 147, 249);            // Soft purple highlight
        defaultActiveText = Color(248, 248, 242);
        defaultInactiveText = Color(128, 128, 128);

        // Button Colors:
        defaultButtonText = Color(248, 248, 242);
        defaultButtonBackground = Color(80, 80, 80);
        defaultButtonForeground = Color(248, 248, 242);
        defaultButtonBorderLight = Color(180, 180, 180);
        defaultButtonBorderDark  = Color(90, 90, 90);

        // Checkbox Colors:
        defaultCheckboxEnabled = Color(248, 248, 242);
        defaultCheckboxDisabled = Color(180, 180, 180);
        defaultCheckboxChecked = Color(248, 248, 242);
        defaultCheckboxUnchecked = Color(39, 40, 34);
        defaultCheckboxBorderLight = Color(180, 180, 180);
        defaultCheckboxBorderDark  = Color(90, 90, 90);

        // OptionSelect Colors:
        defaultSelectOptionUnselected = Color(220, 220, 220);
        defaultSelectOptionSelected = Color(150, 150, 150);
        defaultSelectOptionBorderLight = Color(180, 180, 180);
        defaultSelectOptionBorderDark  = Color(90, 90, 90);

        // TextInput Colors:
        defaultTextInputBackground = Color(255, 255, 255);   // White input background
        defaultTextInputText = Color(39, 40, 34);             // Dark text on white
        defaultTextInputBorderLight = Color(180, 180, 180);
        defaultTextInputBorderDark  = Color(90, 90, 90);

        // Label Colors:
        defaultLabelText = Color(255, 255, 255);              // White labels
        defaultLabelBackground = Color(39, 40, 34);

        // ContextMenu Colors:
        // For Molokai, we choose a context menu background that contrasts well.
        defaultContextMenuBackground = Color(50, 50, 50);      // Dark gray
        defaultContextMenuText = Color(248, 248, 242);         // Near white text
        defaultContextMenuBorder = Color(90, 90, 90);          // Dark border
        defaultContextMenuHighlight = Color(189, 147, 249);    // Same as highlight
    }
};

} // namespace ui
