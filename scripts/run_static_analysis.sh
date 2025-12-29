#!/bin/bash

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"
BUILD_DIR="$PROJECT_ROOT/build"

echo "🔍 Running Static Analysis for UI Framework"
echo "Project: $PROJECT_ROOT"

# Ensure build directory exists
if [ ! -d "$BUILD_DIR" ]; then
    echo "❌ Build directory not found. Run 'meson setup build' first."
    exit 1
fi

echo ""
echo "🔍 Running cppcheck..."
cppcheck --enable=all \
         --std=c++17 \
         --suppress=missingIncludeSystem \
         --suppress=unusedFunction \
         --suppress=unmatchedSuppression \
         --suppress=toomanyconfigs \
         --suppress=normalCheckLevelMaxBranches \
         --inline-suppr \
         --quiet \
         --error-exitcode=0 \
         -I "$PROJECT_ROOT/include" \
         "$PROJECT_ROOT/src" "$PROJECT_ROOT/include" 2>&1 | \
         grep -E "(error|warning|style|performance)" | head -20

echo ""
echo "✅ Static analysis completed! Found issues to address in Phase 3.1"
echo "📊 Baseline established - ready for systematic improvements"
