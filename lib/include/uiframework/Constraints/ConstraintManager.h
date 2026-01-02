#pragma once

#include <memory>
#include <unordered_map>
#include <mutex>
#include <vector>

namespace ui {

// Forward declarations
class UIElement;

// Anchor types for positioning elements relative to others
enum class AnchorType {
    Above,   // Position above target element
    Below,   // Position below target element
    Left,    // Position to the left of target element
    Right,   // Position to the right of target element
    Center   // Center on target element
};

// Constraint definition for element positioning
struct Constraint {
    std::weak_ptr<UIElement> target;  // Element to anchor to
    AnchorType type;                  // Type of anchoring
    int offset;                       // Pixel offset from anchor point
    
    Constraint(std::weak_ptr<UIElement> target_, AnchorType type_, int offset_ = 0)
        : target(target_), type(type_), offset(offset_) {}
};

// Thread-safe constraint manager for element positioning
class ConstraintManager {
private:
    std::unordered_map<UIElement*, std::vector<Constraint>> constraints;
    std::unordered_map<UIElement*, std::pair<int, int>> cachedPositions;
    mutable std::mutex constraintMutex;
    int gridSize = 0; // Grid size for snapping (0 = disabled)

public:
    ConstraintManager() = default;
    ~ConstraintManager() = default;

    // Add constraint for an element
    void addConstraint(UIElement* element, const Constraint& constraint);
    
    // Remove all constraints for an element
    void removeConstraints(UIElement* element);
    
    // Calculate position based on constraints
    std::pair<int, int> calculatePosition(UIElement* element);
    
    // Update cached position
    void updateCachedPosition(UIElement* element, int x, int y);
    
    // Get cached position (returns {-1, -1} if not cached)
    std::pair<int, int> getCachedPosition(UIElement* element) const;
    
    // Clear all constraints and cache
    void clear();
    
    // Check if element has constraints
    bool hasConstraints(UIElement* element) const;
    
    // Grid snapping
    void setGridSize(int size);
    int getGridSize() const;
};

} // namespace ui
