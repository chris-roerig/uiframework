#include "uiframework/Constraints/ConstraintManager.h"
#include "uiframework/UIElements/UIElement.h"
#include "uiframework/Constraints/GridSnap.h"
#include <algorithm>

namespace ui {

void ConstraintManager::addConstraint(UIElement* element, const Constraint& constraint) {
    std::lock_guard<std::mutex> lock(constraintMutex);
    constraints[element].push_back(constraint);
    // Invalidate cached position when constraint is added
    cachedPositions.erase(element);
}

void ConstraintManager::removeConstraints(UIElement* element) {
    std::lock_guard<std::mutex> lock(constraintMutex);
    constraints.erase(element);
    cachedPositions.erase(element);
}

std::pair<int, int> ConstraintManager::calculatePosition(UIElement* element) {
    std::lock_guard<std::mutex> lock(constraintMutex);
    
    // Check if we have cached position
    auto cachedIt = cachedPositions.find(element);
    if (cachedIt != cachedPositions.end()) {
        return cachedIt->second;
    }
    
    // Find constraints for this element
    auto constraintIt = constraints.find(element);
    if (constraintIt == constraints.end() || constraintIt->second.empty()) {
        // No constraints, return current position
        int currentX = element->getX();
        int currentY = element->getY();
        cachedPositions[element] = {currentX, currentY};
        return {currentX, currentY};
    }
    
    // Use the first valid constraint (simple implementation)
    for (const auto& constraint : constraintIt->second) {
        auto targetPtr = constraint.target.lock();
        if (!targetPtr) {
            continue; // Target element no longer exists
        }
        
        int newX = element->getX();
        int newY = element->getY();
        
        switch (constraint.type) {
            case AnchorType::Above:
                newX = targetPtr->getX();
                newY = targetPtr->getY() - element->getHeight() - constraint.offset;
                break;
                
            case AnchorType::Below:
                newX = targetPtr->getX();
                newY = targetPtr->getY() + targetPtr->getHeight() + constraint.offset;
                break;
                
            case AnchorType::Left:
                newX = targetPtr->getX() - element->getWidth() - constraint.offset;
                newY = targetPtr->getY();
                break;
                
            case AnchorType::Right:
                newX = targetPtr->getX() + targetPtr->getWidth() + constraint.offset;
                newY = targetPtr->getY();
                break;
                
            case AnchorType::Center:
                newX = targetPtr->getX() + (targetPtr->getWidth() - element->getWidth()) / 2;
                newY = targetPtr->getY() + (targetPtr->getHeight() - element->getHeight()) / 2;
                break;
        }
        
        // Apply grid snapping if enabled
        if (gridSize > 0) {
            GridSnap::snapPosition(newX, newY, gridSize);
        }
        
        // Cache and return calculated position
        cachedPositions[element] = {newX, newY};
        return {newX, newY};
    }
    
    // No valid constraints found, return current position
    int currentX = element->getX();
    int currentY = element->getY();
    cachedPositions[element] = {currentX, currentY};
    return {currentX, currentY};
}

void ConstraintManager::updateCachedPosition(UIElement* element, int x, int y) {
    std::lock_guard<std::mutex> lock(constraintMutex);
    cachedPositions[element] = {x, y};
}

std::pair<int, int> ConstraintManager::getCachedPosition(UIElement* element) const {
    std::lock_guard<std::mutex> lock(constraintMutex);
    auto it = cachedPositions.find(element);
    if (it != cachedPositions.end()) {
        return it->second;
    }
    return {-1, -1}; // Not cached
}

void ConstraintManager::clear() {
    std::lock_guard<std::mutex> lock(constraintMutex);
    constraints.clear();
    cachedPositions.clear();
}

bool ConstraintManager::hasConstraints(UIElement* element) const {
    std::lock_guard<std::mutex> lock(constraintMutex);
    auto it = constraints.find(element);
    return it != constraints.end() && !it->second.empty();
}

void ConstraintManager::setGridSize(int size) {
    std::lock_guard<std::mutex> lock(constraintMutex);
    gridSize = size;
    // Clear cached positions when grid size changes
    cachedPositions.clear();
}

int ConstraintManager::getGridSize() const {
    std::lock_guard<std::mutex> lock(constraintMutex);
    return gridSize;
}

} // namespace ui
