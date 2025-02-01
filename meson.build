project('sdl_ui_lib', 'cpp',
  version : '0.1.0',
  default_options : ['cpp_std=c++17']
)

sdl2_dep = dependency('sdl2')
ttf_dep = dependency('SDL2_ttf')

executable('sdl_ui_demo',
  sources : ['main.cpp', 'UI.cpp'],
  dependencies : [sdl2_dep, ttf_dep],
  install : true
)

