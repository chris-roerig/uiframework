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
    }
};

} // namespace ui
