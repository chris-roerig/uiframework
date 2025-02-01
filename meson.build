project('sdl_ui_demo', 'cpp',
  version : '0.1.0',
  default_options : ['cpp_std=c++17']
)

sdl2_dep = dependency('sdl2')
ttf_dep = dependency('SDL2_ttf')

executable('sdl_ui_demo',
  sources : [
    'main.cpp',
    'UICore.cpp',
    'UI.cpp'
    # Theme files are header-only, so they don't require separate compilation.
  ],
  dependencies : [sdl2_dep, ttf_dep],
  install : true
)
