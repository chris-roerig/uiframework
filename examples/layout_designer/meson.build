project('layout_designer', 'cpp',
  version : '1.0.0',
  default_options : ['warning_level=3', 'cpp_std=c++17'])

# Find dependencies
sdl2_dep = dependency('sdl2')
sdl2_ttf_dep = dependency('SDL2_ttf')
sdl2_image_dep = dependency('SDL2_image')

# UI Framework library path
uiframework_inc = include_directories('../../lib/include')
uiframework_lib = static_library('uiframework',
  sources: [
    '../../lib/src/UI.cpp',
    '../../lib/src/UICore.cpp',
    '../../lib/src/UIElement.cpp',
    '../../lib/src/Button.cpp',
    '../../lib/src/Label.cpp',
    '../../lib/src/TextBox.cpp',
    '../../lib/src/Canvas.cpp',
    '../../lib/src/FontManager.cpp',
    '../../lib/src/VirtualKeyboard.cpp',
    '../../lib/src/TTFManager.cpp',
    '../../lib/src/Logger.cpp',
    '../../lib/src/Helpers.cpp',
    '../../lib/src/CheckBox.cpp',
    '../../lib/src/HBoxLayout.cpp',
    '../../lib/src/VBoxLayout.cpp',
    '../../lib/src/GridLayout.cpp',
    '../../lib/src/Layout.cpp',
    '../../lib/src/LayoutContainer.cpp',
    '../../lib/src/ProgressBar.cpp',
    '../../lib/src/Slider.cpp',
    '../../lib/src/ListView.cpp',
    '../../lib/src/OptionSelect.cpp',
    '../../lib/src/ContextMenu.cpp',
    '../../lib/src/Modal.cpp',
    '../../lib/src/Image.cpp',
    '../../lib/src/Sprite.cpp',
    '../../lib/src/AnimatedSprite.cpp',
    # 🆕 New framework components
    '../../lib/src/InteractiveElement.cpp',
    '../../lib/src/Rendering/RenderContext.cpp',
    '../../lib/src/Utils/TextUtils.cpp',
    '../../lib/src/Utils/BorderRenderer.cpp',
    '../../lib/src/Utils/TooltipRenderer.cpp',
    '../../lib/src/UIElements/CycleList.cpp',
    '../../lib/src/UIElements/TabbedPanel.cpp',
    '../../lib/src/ElementPool.cpp',
    '../../lib/src/RealTime/UIUpdateQueue.cpp',
    '../../lib/src/Focus/FocusManager.cpp'
  ],
  include_directories: uiframework_inc,
  dependencies: [sdl2_dep, sdl2_ttf_dep, sdl2_image_dep]
)

# Layout Designer executable
executable('layout_designer',
  sources: [
    'main.cpp',
    'LayoutDesigner.cpp'
  ],
  include_directories: uiframework_inc,
  link_with: uiframework_lib,
  dependencies: [sdl2_dep, sdl2_ttf_dep, sdl2_image_dep],
  install: false
)

# Simple Layout Designer executable
executable('simple_layout_designer',
  sources: [
    'simple_main.cpp',
    'SimpleLayoutDesigner.cpp'
  ],
  include_directories: uiframework_inc,
  link_with: uiframework_lib,
  dependencies: [sdl2_dep, sdl2_ttf_dep, sdl2_image_dep],
  install: false
)

# Keyboard Layout Designer (pure SDL) - DISABLED due to duplicate case error
# executable('keyboard_layout_designer',
#   sources: [
#     'keyboard_layout_designer.cpp'
#   ],
#   include_directories: uiframework_inc,
#   dependencies: [sdl2_dep, sdl2_ttf_dep],
#   install: false
# )
