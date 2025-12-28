#pragma once
#include "ThemeBase.h"

namespace ui {

class ThemeFrameworkDefault : public ThemeBase {
public:
    ThemeFrameworkDefault() {
        // Main App Colors
        defaultBackground = Color(31, 32, 40);         // #1F2028
        defaultForeground = Color(248, 248, 242);        // #F8F8F2
        defaultHighlight  = Color(255, 121, 198);         // #FF79C6
        defaultActiveText = Color(248, 248, 242);
        defaultInactiveText = Color(108, 113, 124);       // Subdued tone

        // Button Colors
        defaultButtonText = Color(248, 248, 242);
        defaultButtonBackground = Color(68, 71, 90);      // #44475A
        defaultButtonForeground = Color(248, 248, 242);
        defaultButtonBorderLight = Color(100, 100, 110);
        defaultButtonBorderDark  = Color(50, 50, 60);

        // Checkbox Colors
        defaultCheckboxEnabled  = Color(248, 248, 242);
        defaultCheckboxDisabled = Color(108, 113, 124);
        defaultCheckboxChecked  = Color(248, 248, 242);
        defaultCheckboxUnchecked = Color(81, 92, 104);
        defaultCheckboxBorderLight = Color(100, 100, 110);
        defaultCheckboxBorderDark  = Color(50, 50, 60);

        // OptionSelect Colors
        defaultSelectOptionUnselected = Color(68, 71, 90);
        defaultSelectOptionSelected   = Color(255, 121, 198);
        defaultSelectOptionBorderLight = Color(100, 100, 110);
        defaultSelectOptionBorderDark  = Color(50, 50, 60);

        // TextInput Colors
        defaultTextInputBackground = Color(68, 71, 90);
        defaultTextInputText       = Color(248, 248, 242);
        defaultTextInputBorderLight = Color(100, 100, 110);
        defaultTextInputBorderDark  = Color(50, 50, 60);

        // Label Colors
        defaultLabelText      = Color(255, 255, 255);       // White text for labels
        defaultLabelBackground= Color(31, 32, 40);

        // ContextMenu Colors
        // We want the context menu bar to have a distinct color from the main app background.
        defaultContextMenuBackground = Color(68, 71, 90);    // Same as accent used for buttons
        defaultContextMenuText = Color(255, 255, 255);         // White text
        defaultContextMenuBorder = Color(50, 50, 60);          // Dark border for subtle 3D effect
        defaultContextMenuHighlight = Color(115, 163, 211);

        // Overlay: a semi-transparent black.
        defaultModalOverlay = Color(0, 0, 0, 140);
        // Modal background: a slightly lighter color than the main background.
        defaultModalBackground = Color(240, 240, 240);
        defaultModalText = Color(55, 64, 74);
        defaultModalBorder = Color(180, 180, 180);
        defaultModalButtonBackground = defaultButtonBackground;  // or your chosen value
        defaultModalButtonText = defaultButtonText;
        defaultModalButtonHighlight = defaultHighlight;
        defaultModalButtonBorder = defaultButtonBorderDark;  // or another value you prefer

        defaultListViewScrollbarBackground = Color(200, 200, 200); // Light gray for the track.
        defaultListViewScrollbarThumb = Color(150, 150, 150);        // Darker gray for the thumb.

        defaultProgressBarBackground = Color(68, 71, 90);
        defaultProgressBarBorder = Color(100, 100, 110);
        defaultProgressBarForeground = Color(255, 121, 198);
        defaultProgressBarText = Color(0,0,0,255);
    }
};

} // namespace ui
