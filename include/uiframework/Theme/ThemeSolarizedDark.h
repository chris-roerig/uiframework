#pragma once
#include "ThemeBase.h"

namespace ui {

class ThemeSolarizedDark : public ThemeBase {
public:
    ThemeSolarizedDark() {
        // Solarized Dark Palette
        // Base colors: base03(0,43,54), base02(7,54,66), base01(88,110,117), base00(101,123,131)
        // Content colors: base0(131,148,150), base1(147,161,161), base2(238,232,213), base3(253,246,227)
        // Accent colors: yellow(181,137,0), orange(203,75,22), red(220,50,47), magenta(211,54,130)
        //               violet(108,113,196), blue(38,139,210), cyan(42,161,152), green(133,153,0)

        // Main App Colors
        defaultBackground = Color(0, 43, 54);           // base03
        defaultForeground = Color(131, 148, 150);       // base0
        defaultHighlight  = Color(181, 137, 0);         // yellow
        defaultActiveText = Color(131, 148, 150);       // base0
        defaultInactiveText = Color(88, 110, 117);      // base01

        // Button Colors
        defaultButtonText = Color(131, 148, 150);       // base0
        defaultButtonBackground = Color(38, 139, 210);  // blue
        defaultButtonForeground = Color(131, 148, 150); // base0
        defaultButtonBorderLight = Color(101, 123, 131); // base00
        defaultButtonBorderDark  = Color(88, 110, 117);  // base01

        // ContextMenu Colors
        defaultContextMenuBackground = Color(7, 54, 66);  // base02
        defaultContextMenuText = Color(131, 148, 150);    // base0
        defaultContextMenuBorder = Color(88, 110, 117);   // base01
        defaultContextMenuHighlight = Color(181, 137, 0); // yellow
        defaultContextMenuActiveItem = Color(38, 139, 210); // blue

        // Modal Colors (use light theme for contrast)
        defaultModalOverlay = Color(0, 0, 0, 140);
        defaultModalBackground = Color(253, 246, 227);    // base3
        defaultModalText = Color(0, 43, 54);              // base03
        defaultModalBorder = Color(147, 161, 161);        // base1
        defaultModalButtonBackground = Color(38, 139, 210); // blue
        defaultModalButtonText = Color(253, 246, 227);    // base3
        defaultModalButtonHighlight = Color(181, 137, 0); // yellow
        defaultModalButtonBorder = Color(88, 110, 117);   // base01

        // TextInput Colors
        defaultTextInputBackground = Color(7, 54, 66);    // base02
        defaultTextInputText = Color(131, 148, 150);      // base0
        defaultTextInputBorderLight = Color(101, 123, 131); // base00
        defaultTextInputBorderDark = Color(88, 110, 117);   // base01

        // ListView Colors
        defaultListViewBackground = Color(7, 54, 66);     // base02
        defaultListViewText = Color(131, 148, 150);       // base0
        defaultListViewBorder = Color(88, 110, 117);      // base01
        defaultGridBorder = Color(147, 161, 161);         // base1 - lighter for contrast
        defaultListViewSelectedItem = Color(38, 139, 210); // blue
        defaultListViewScrollbarBackground = Color(88, 110, 117); // base01
        defaultListViewScrollbarThumb = Color(101, 123, 131);     // base00

        // ProgressBar Colors
        defaultProgressBarBackground = Color(7, 54, 66);  // base02
        defaultProgressBarBorder = Color(88, 110, 117);   // base01
        defaultProgressBarForeground = Color(133, 153, 0); // green
        defaultProgressBarText = Color(131, 148, 150);    // base0

        // Label Colors
        defaultLabelText = Color(131, 148, 150);          // base0
        defaultLabelBackground = Color(0, 43, 54);        // base03

        // CheckBox Colors
        defaultCheckboxEnabled = Color(131, 148, 150);    // base0
        defaultCheckboxDisabled = Color(88, 110, 117);    // base01
        defaultCheckboxChecked = Color(133, 153, 0);      // green
        defaultCheckboxUnchecked = Color(7, 54, 66);      // base02
        defaultCheckboxBorderLight = Color(101, 123, 131); // base00
        defaultCheckboxBorderDark = Color(88, 110, 117);   // base01

        // OptionSelect Colors
        defaultSelectOptionSelected = Color(38, 139, 210);   // blue
        defaultSelectOptionUnselected = Color(7, 54, 66);    // base02
        defaultSelectOptionBorderLight = Color(101, 123, 131); // base00
        defaultSelectOptionBorderDark = Color(88, 110, 117);   // base01
        defaultSelectOptionTextSelected = Color(253, 246, 227); // base3 (light text on blue)
        defaultSelectOptionTextUnselected = Color(131, 148, 150); // base0

        // Focus Colors
        defaultFocusBorder = Color(181, 137, 0);          // yellow
        defaultFocusBackground = Color(181, 137, 0, 30);  // yellow with alpha

        // Slider Colors
        defaultSliderTrack = Color(7, 54, 66);            // base02
        defaultSliderThumb = Color(38, 139, 210);         // blue
        defaultSliderBorder = Color(88, 110, 117);        // base01

        // Canvas Colors
        defaultCanvasBackground = Color(0, 43, 54);       // base03
        defaultCanvasGrid = Color(7, 54, 66);             // base02
        defaultCanvasBorder = Color(88, 110, 117);        // base01
    }
};

} // namespace ui
