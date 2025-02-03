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
    }
};

} // namespace ui
