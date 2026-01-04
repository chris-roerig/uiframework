#pragma once

namespace ui {

enum class ScalingMode {
    Fit,      // Maintain aspect ratio, fit within bounds
    Fill,     // Maintain aspect ratio, fill bounds (may crop)
    Stretch   // Ignore aspect ratio, stretch to fill bounds
};

} // namespace ui
