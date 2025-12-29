// This file is deprecated and will be removed
// Theme management is now handled through UICore
// No more global theme state for thread safety

#pragma once

// Deprecated - kept for backward compatibility only
// Use UICore::getTheme() instead
namespace ui {
    class Theme;
}

// This will be removed in future versions
extern std::shared_ptr<ui::Theme> g_currentTheme [[deprecated("Use UICore::getTheme() instead")]];
