project('sdl_ui_demo', 'cpp',
  version : '0.1.0',
  default_options : ['cpp_std=c++17']
)

sdl2_dep = dependency('sdl2')
ttf_dep = dependency('SDL2_ttf')

executable('sdl_ui_demo',
  sources : [
    'demos/main.cpp',
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
  ],
  dependencies : [sdl2_dep, ttf_dep],
  install : true
)
