#pragma once
#include "ThemeBase.h"

namespace ui {

class ThemeSolarizedDark : public ThemeBase {
public:
    ThemeSolarizedDark() {
        // Main App Colors
        defaultBackground = Color(0, 43, 54);
        defaultForeground = Color(131, 148, 150);
        defaultHighlight  = Color(181, 137, 0);
        defaultActiveText = Color(131, 148, 150);
        defaultInactiveText = Color(88, 110, 117);

        // Button Colors
        defaultButtonText = Color(131, 148, 150);
        defaultButtonBackground = Color(38, 139, 210);
        defaultButtonForeground = Color(131, 148, 150);
        defaultButtonBorderLight = Color(101, 123, 131);
        defaultButtonBorderDark  = Color(88, 110, 117);

        // ContextMenu Colors
        defaultContextMenuBackground = Color(7, 54, 66);  // Slightly lighter than main background.
        defaultContextMenuText = Color(131, 148, 150);
        defaultContextMenuBorder = Color(88, 110, 117);
        defaultContextMenuHighlight = defaultHighlight;

        // Modal Colors
        defaultModalOverlay = Color(0, 0, 0, 140);
        defaultModalBackground = Color(240, 240, 240); // Light modal background.
        defaultModalText = Color(0, 43, 54);           // Dark text.
        defaultModalBorder = Color(180, 180, 180);
        defaultModalButtonBackground = defaultButtonBackground;
        defaultModalButtonText = defaultButtonText;
        defaultModalButtonHighlight = defaultHighlight;
        defaultModalButtonBorder = defaultButtonBorderDark;
    }
};

} // namespace ui
