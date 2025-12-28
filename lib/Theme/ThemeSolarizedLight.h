#pragma once
#include "ThemeBase.h"

namespace ui {

class ThemeSolarizedLight : public ThemeBase {
public:
    ThemeSolarizedLight() {
        // Main App Colors
        defaultBackground = Color(253, 246, 227);
        defaultForeground = Color(101, 123, 131);
        defaultHighlight  = Color(203, 75, 22);
        defaultActiveText = Color(180, 180, 180);
        defaultInactiveText = Color(150, 150, 150);

        // Button Colors
        defaultButtonText = defaultInactiveText;
        defaultButtonBackground = Color(180, 180, 180);
        defaultButtonForeground = Color(101, 123, 131);
        defaultButtonBorderLight = Color(220, 220, 220);
        defaultButtonBorderDark  = Color(150, 150, 150);

        // ContextMenu Colors
        defaultContextMenuBackground = Color(240, 230, 200);  // Soft beige.
        defaultContextMenuText = Color(55, 64, 74);
        defaultContextMenuBorder = Color(150, 150, 150);
        defaultContextMenuHighlight = Color(203, 75, 22);

        // Modal Colors
        defaultModalOverlay = Color(0, 0, 0, 140);
        defaultModalBackground = Color(255, 255, 255);
        defaultModalText = Color(101, 123, 131);
        defaultModalBorder = Color(180, 180, 180);
        defaultModalButtonBackground = defaultButtonBackground;
        defaultModalButtonText = defaultButtonText;
        defaultModalButtonHighlight = defaultHighlight;
        defaultModalButtonBorder = defaultButtonBorderDark;

        // TextInput Colors
        defaultTextInputBackground = Color(255, 255, 255);
        defaultTextInputText = Color(101, 123, 131);
        defaultTextInputBorderLight = Color(220, 220, 220);
        defaultTextInputBorderDark = Color(150, 150, 150);

        // ListView Colors
        defaultListViewBackground = Color(255, 255, 255);
        defaultListViewText = Color(101, 123, 131);
        defaultListViewBorder = Color(150, 150, 150);
        defaultGridBorder = Color(101, 123, 131);         // base00 - darker for contrast on light bg
        defaultListViewSelectedItem = Color(203, 75, 22);

        // ProgressBar Colors
        defaultProgressBarBackground = Color(240, 230, 200);
        defaultProgressBarBorder = Color(150, 150, 150);
        defaultProgressBarForeground = Color(133, 153, 0);
        defaultProgressBarText = Color(101, 123, 131);

        // Label Colors
        defaultLabelText = Color(101, 123, 131);
        defaultLabelBackground = Color(253, 246, 227);

        // CheckBox Colors
        defaultCheckboxEnabled = Color(101, 123, 131);
        defaultCheckboxDisabled = Color(150, 150, 150);
        defaultCheckboxChecked = Color(133, 153, 0);
        defaultCheckboxUnchecked = Color(255, 255, 255);
        defaultCheckboxBorderLight = Color(220, 220, 220);
        defaultCheckboxBorderDark = Color(150, 150, 150);

        // OptionSelect Colors
        defaultSelectOptionSelected = Color(203, 75, 22);
        defaultSelectOptionUnselected = Color(255, 255, 255);
        defaultSelectOptionBorderLight = Color(220, 220, 220);
        defaultSelectOptionBorderDark = Color(150, 150, 150);
        defaultSelectOptionTextSelected = Color(255, 255, 255);
        defaultSelectOptionTextUnselected = Color(101, 123, 131);

        // Focus Colors
        defaultFocusBorder = Color(203, 75, 22);
        defaultFocusBackground = Color(203, 75, 22, 30);

        // Slider Colors
        defaultSliderTrack = Color(240, 230, 200);
        defaultSliderThumb = Color(203, 75, 22);
        defaultSliderBorder = Color(150, 150, 150);

        // Canvas Colors
        defaultCanvasBackground = Color(253, 246, 227);
        defaultCanvasGrid = Color(240, 230, 200);
        defaultCanvasBorder = Color(150, 150, 150);
    }
};

} // namespace ui
