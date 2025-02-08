project('sdl_ui_demo', 'cpp',
  version : '0.1.0',
  default_options : ['cpp_std=c++17']
)

sdl2_dep = dependency('sdl2', required: true)
ttf_dep = dependency('SDL2_ttf', required: true)
sdl2_image_dep = dependency('sdl2_image', required: true)

executable('sdl_ui_demo',
  sources : [
    'demos/main.cpp',
    'Helpers.cpp',
    'UICore.cpp',
    'UI.cpp',
    'UIElements/ContextMenu.cpp',
    'UIElements/Modal.cpp',
    'UIElements/ListView.cpp',
    'UIElements/Label.cpp',
    'UIElements/Button.cpp',
    'UIElements/TextBox.cpp',
    'UIElements/CheckBox.cpp',
    'UIElements/OptionSelect.cpp',
    'UIElements/Canvas.cpp',
    'UIElements/Image.cpp',
  ],
  dependencies : [sdl2_dep, ttf_dep, sdl2_image_dep],
  install : true
)
