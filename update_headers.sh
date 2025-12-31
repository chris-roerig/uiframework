#!/bin/bash

# Update all UI element headers to use renderImpl instead of render
cd /Users/chrisroerig/Projects/uiframework

# List of header files to update (excluding Button.h which is already done)
headers=(
    "lib/include/uiframework/UIElements/CheckBox.h"
    "lib/include/uiframework/UIElements/TextBox.h"
    "lib/include/uiframework/UIElements/Label.h"
    "lib/include/uiframework/UIElements/Canvas.h"
    "lib/include/uiframework/UIElements/ContextMenu.h"
    "lib/include/uiframework/UIElements/ListView.h"
    "lib/include/uiframework/UIElements/Modal.h"
    "lib/include/uiframework/UIElements/OptionSelect.h"
    "lib/include/uiframework/UIElements/Image.h"
    "lib/include/uiframework/UIElements/Sprite.h"
    "lib/include/uiframework/UIElements/AnimatedSprite.h"
    "lib/include/uiframework/UIElements/ProgressBar.h"
    "lib/include/uiframework/UIElements/LayoutContainer.h"
    "lib/include/uiframework/UIElements/Slider.h"
    "lib/include/uiframework/UIElements/VirtualKeyboard.h"
    "lib/include/uiframework/UIElements/CycleList.h"
    "lib/include/uiframework/UIElements/TabbedPanel.h"
)

for header in "${headers[@]}"; do
    if [ -f "$header" ]; then
        echo "Updating $header..."
        # Replace the render method declaration with renderImpl
        sed -i '' 's/void render(SDL_Renderer\* renderer, TTF_Font\* font, std::shared_ptr<class Theme> theme) override;/protected:\
    void renderImpl(const RenderContext\& ctx) override;\
\
public:/g' "$header"
    fi
done

echo "Header updates complete!"
