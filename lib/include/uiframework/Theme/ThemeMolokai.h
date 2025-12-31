#pragma once
#include "ThemeBase.h"

namespace ui {

class ThemeMolokai : public ThemeBase {
public:
    ThemeMolokai() {
        // Main App Colors
        defaultBackground = Color(39, 40, 34);
        defaultForeground = Color(248, 248, 242);
        defaultHighlight  = Color(189, 147, 249);
        defaultActiveText = Color(248, 248, 242);
        defaultInactiveText = Color(128, 128, 128);

        // Button Colors
        defaultButtonText = Color(248, 248, 242);
        defaultButtonBackground = Color(80, 80, 80);
        defaultButtonForeground = Color(248, 248, 242);
        defaultButtonBorderLight = Color(180, 180, 180);
        defaultButtonBorderDark  = Color(90, 90, 90);

        // ContextMenu Colors
        defaultContextMenuBackground = Color(50, 50, 50);
        defaultContextMenuText = Color(248, 248, 242);
        defaultContextMenuBorder = Color(90, 90, 90);
        defaultContextMenuHighlight = defaultHighlight;

        // Modal Colors
        defaultModalOverlay = Color(0, 0, 0, 140);
        defaultModalBackground = Color(255, 255, 255); // White modal background.
        defaultModalText = Color(39, 40, 34);           // Dark text.
        defaultModalBorder = Color(180, 180, 180);
        defaultModalButtonBackground = defaultButtonBackground;
        defaultModalButtonText = defaultButtonText;
        defaultModalButtonHighlight = defaultHighlight;
        defaultModalButtonBorder = defaultButtonBorderDark;

        // TextInput Colors
        defaultTextInputBackground = Color(50, 50, 50);
        defaultTextInputText = Color(248, 248, 242);
        defaultTextInputBorderLight = Color(180, 180, 180);
        defaultTextInputBorderDark = Color(90, 90, 90);

        // ListView Colors
        defaultListViewBackground = Color(50, 50, 50);
        defaultListViewText = Color(248, 248, 242);
        defaultListViewBorder = Color(90, 90, 90);
        defaultGridBorder = Color(150, 150, 150);         // Lighter gray for contrast
        defaultListViewSelectedItem = Color(189, 147, 249);

        // ProgressBar Colors
        defaultProgressBarBackground = Color(128, 128, 128);
        defaultProgressBarBorder = Color(90, 90, 90);
        defaultProgressBarForeground = Color(248, 248, 242);
        defaultProgressBarText = Color(248, 248, 242);

        // Label Colors
        defaultLabelText = Color(248, 248, 242);
        defaultLabelBackground = Color(39, 40, 34);

        // CheckBox Colors
        defaultCheckboxEnabled = Color(248, 248, 242);
        defaultCheckboxDisabled = Color(128, 128, 128);
        defaultCheckboxChecked = Color(166, 226, 46);
        defaultCheckboxUnchecked = Color(50, 50, 50);
        defaultCheckboxBorderLight = Color(180, 180, 180);
        defaultCheckboxBorderDark = Color(90, 90, 90);

        // OptionSelect Colors
        defaultSelectOptionSelected = Color(189, 147, 249);
        defaultSelectOptionUnselected = Color(50, 50, 50);
        defaultSelectOptionBorderLight = Color(180, 180, 180);
        defaultSelectOptionBorderDark = Color(90, 90, 90);
        defaultSelectOptionTextSelected = Color(39, 40, 34);
        defaultSelectOptionTextUnselected = Color(248, 248, 242);

        // Focus Colors
        defaultFocusBorder = Color(189, 147, 249);
        defaultFocusBackground = Color(189, 147, 249, 30);

        // Slider Colors
        defaultSliderTrack = Color(50, 50, 50);
        defaultSliderThumb = Color(189, 147, 249);
        defaultSliderBorder = Color(90, 90, 90);

        // Canvas Colors
        defaultCanvasBackground = Color(39, 40, 34);
        defaultCanvasGrid = Color(50, 50, 50);
        defaultCanvasBorder = Color(90, 90, 90);
        
        // Tooltip Colors
        defaultTooltipBackground = Color(50, 50, 50);      // Dark gray background
        defaultTooltipText = Color(248, 248, 242);         // Light text
        defaultTooltipBorder = Color(90, 90, 90);          // Medium gray border
    }
};

} // namespace ui
