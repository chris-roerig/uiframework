#pragma once
#include "ThemeBase.h"

namespace ui {

class ThemeSolarizedDark : public ThemeBase {
public:
    ThemeSolarizedDark() {
        // Solarized Dark palette (adapted)
        defaultBackground = Color(0, 43, 54);            // Base03: dark cyan-blue
        defaultForeground = Color(131, 148, 150);          // Base0
        defaultHighlight  = Color(181, 137, 0);            // Accent (yellowish)
        defaultActiveText = Color(131, 148, 150);
        defaultInactiveText = Color(88, 110, 117);         // Base1

        // Button Colors:
        defaultButtonText = Color(131, 148, 150);
        defaultButtonBackground = Color(38, 139, 210);       // Blue accent
        defaultButtonForeground = Color(131, 148, 150);
        defaultButtonBorderLight = Color(101, 123, 131);
        defaultButtonBorderDark  = Color(88, 110, 117);

        // Checkbox Colors:
        defaultCheckboxEnabled = Color(131, 148, 150);
        defaultCheckboxDisabled = Color(88, 110, 117);
        defaultCheckboxChecked = Color(131, 148, 150);
        defaultCheckboxUnchecked = Color(0, 43, 54);
        defaultCheckboxBorderLight = Color(101, 123, 131);
        defaultCheckboxBorderDark  = Color(88, 110, 117);

        // OptionSelect Colors:
        defaultSelectOptionUnselected = Color(88, 110, 117);
        defaultSelectOptionSelected = Color(181, 137, 0);
        defaultSelectOptionBorderLight = Color(101, 123, 131);
        defaultSelectOptionBorderDark  = Color(88, 110, 117);

        // TextInput Colors:
        defaultTextInputBackground = Color(0, 43, 54);
        defaultTextInputText = Color(131, 148, 150);
        defaultTextInputBorderLight = Color(101, 123, 131);
        defaultTextInputBorderDark  = Color(88, 110, 117);

        // Label Colors:
        defaultLabelText = Color(255, 255, 255);           // White labels
        defaultLabelBackground = Color(0, 43, 54);

        // ContextMenu Colors:
        // Use a distinct background that’s lighter than the main app background.
        defaultContextMenuBackground = Color(7, 54, 66);     // Slightly lighter than base03
        defaultContextMenuText = Color(131, 148, 150);         // Same as foreground
        defaultContextMenuBorder = Color(88, 110, 117);        // Darker for a subtle border
        defaultContextMenuHighlight = Color(181, 137, 0);      // Same as highlight
    }
};

} // namespace ui
