#!/bin/bash

# Update all UI element implementations to use renderImpl and RenderContext
cd /Users/chrisroerig/Projects/uiframework

# List of source files to update (excluding Button.cpp which is already done)
sources=(
    "lib/src/CheckBox.cpp"
    "lib/src/TextBox.cpp"
    "lib/src/Label.cpp"
    "lib/src/Canvas.cpp"
    "lib/src/ContextMenu.cpp"
    "lib/src/ListView.cpp"
    "lib/src/Modal.cpp"
    "lib/src/OptionSelect.cpp"
    "lib/src/Image.cpp"
    "lib/src/Sprite.cpp"
    "lib/src/AnimatedSprite.cpp"
    "lib/src/ProgressBar.cpp"
    "lib/src/LayoutContainer.cpp"
    "lib/src/Slider.cpp"
    "lib/src/VirtualKeyboard.cpp"
    "lib/src/UIElements/CycleList.cpp"
    "lib/src/UIElements/TabbedPanel.cpp"
)

for source in "${sources[@]}"; do
    if [ -f "$source" ]; then
        echo "Updating $source..."
        
        # Replace render method signature
        sed -i '' 's/void \([A-Za-z]*\)::\(render\|renderMenuBar\|renderCollapsed\|renderExpanded\)(SDL_Renderer\* renderer, TTF_Font\* font, std::shared_ptr<Theme> theme)/void \1::renderImpl(const RenderContext\& ctx)/g' "$source"
        
        # Remove ErrorHandling validation (now handled by RenderContext)
        sed -i '' '/if (!ErrorHandling::validateRenderParams(renderer, theme)) {/,/}/d' "$source"
        
        # Replace theme->*Colors() calls with ctx.*Colors()
        sed -i '' 's/theme->buttonColors()/ctx.buttonColors()/g' "$source"
        sed -i '' 's/theme->checkboxColors()/ctx.checkboxColors()/g' "$source"
        sed -i '' 's/theme->textInputColors()/ctx.textInputColors()/g' "$source"
        sed -i '' 's/theme->optionSelectColors()/ctx.optionSelectColors()/g' "$source"
        sed -i '' 's/theme->labelColors()/ctx.labelColors()/g' "$source"
        sed -i '' 's/theme->progressBarColors()/ctx.progressBarColors()/g' "$source"
        sed -i '' 's/theme->sliderColors()/ctx.sliderColors()/g' "$source"
        sed -i '' 's/theme->listViewColors()/ctx.listViewColors()/g' "$source"
        sed -i '' 's/theme->modalColors()/ctx.modalColors()/g' "$source"
        sed -i '' 's/theme->contextMenuColors()/ctx.contextMenuColors()/g' "$source"
        sed -i '' 's/theme->tabbedPanelColors()/ctx.tabbedPanelColors()/g' "$source"
        sed -i '' 's/theme->cycleListColors()/ctx.cycleListColors()/g' "$source"
        
        # Replace renderer with ctx.renderer
        sed -i '' 's/\([^a-zA-Z_]\)renderer\([^a-zA-Z_]\)/\1ctx.renderer\2/g' "$source"
        
        # Replace font with ctx.font
        sed -i '' 's/\([^a-zA-Z_]\)font\([^a-zA-Z_]\)/\1ctx.font\2/g' "$source"
        
        # Handle special cases for function parameters
        sed -i '' 's/getCachedText([^,]*, [^,]*, [^,]*, ctx\.renderer, ctx\.font)/getCachedText("main", text, sdlColor, ctx.renderer, ctx.font)/g' "$source"
    fi
done

echo "Source file updates complete!"
