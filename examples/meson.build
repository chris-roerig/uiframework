project('audio_studio_demo', 'cpp',
  version : '1.0.0',
  default_options : ['warning_level=3', 'cpp_std=c++17'])

# Dependencies
sdl2_dep = dependency('sdl2')
sdl2_ttf_dep = dependency('SDL2_ttf')
sdl2_image_dep = dependency('SDL2_image')

# UI Framework library
ui_framework_lib = static_library('uiframework',
  sources: [
    '../lib/src/UI.cpp',
    '../lib/src/UICore.cpp',
    '../lib/src/Button.cpp',
    '../lib/src/Label.cpp',
    '../lib/src/TextBox.cpp',
    '../lib/src/CheckBox.cpp',
    '../lib/src/OptionSelect.cpp',
    '../lib/src/Canvas.cpp',
    '../lib/src/ContextMenu.cpp',
    '../lib/src/ListView.cpp',
    '../lib/src/Modal.cpp',
    '../lib/src/Image.cpp',
    '../lib/src/Sprite.cpp',
    '../lib/src/AnimatedSprite.cpp',
    '../lib/src/ProgressBar.cpp',
    '../lib/src/Layout.cpp',
    '../lib/src/VBoxLayout.cpp',
    '../lib/src/HBoxLayout.cpp',
    '../lib/src/GridLayout.cpp',
    '../lib/src/LayoutContainer.cpp',
    '../lib/src/Slider.cpp',
    '../lib/src/VirtualKeyboard.cpp',
    '../lib/src/UIElement.cpp',
    '../lib/src/Helpers.cpp',
    '../lib/src/Logger.cpp',
    '../lib/src/FontManager.cpp',
    '../lib/src/TTFManager.cpp'
  ],
  include_directories: ['../lib/include'],
  dependencies: [sdl2_dep, sdl2_ttf_dep, sdl2_image_dep]
)

# Audio Studio Demo executable
executable('audio_studio_demo',
  'audio_studio_demo.cpp',
  include_directories: ['../lib/include'],
  link_with: ui_framework_lib,
  dependencies: [sdl2_dep, sdl2_ttf_dep, sdl2_image_dep],
  install: false
)
