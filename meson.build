project('uiframework', 'cpp',
  version : '1.0.0',
  default_options : ['cpp_std=c++17', 'buildtype=release']
)

# Add compiler arguments to fix the libcpp assertions issue
add_project_arguments('-U_LIBCPP_ENABLE_ASSERTIONS', language : 'cpp')

sdl2_dep = dependency('sdl2', required: true)
ttf_dep = dependency('SDL2_ttf', required: true)
sdl2_image_dep = dependency('sdl2_image', required: true)
catch2_dep = dependency('catch2-with-main', required: false)

# Common source files
common_sources = [
  'src/Helpers.cpp',
  'src/UICore.cpp', 
  'src/UI.cpp',
  'src/FontManager.cpp',
  'src/UIElement.cpp',
  'src/ContextMenu.cpp',
  'src/Modal.cpp',
  'src/ListView.cpp',
  'src/Label.cpp',
  'src/Button.cpp',
  'src/TextBox.cpp',
  'src/CheckBox.cpp',
  'src/OptionSelect.cpp',
  'src/Canvas.cpp',
  'src/Image.cpp',
  'src/Sprite.cpp',
  'src/AnimatedSprite.cpp',
  'src/ProgressBar.cpp',
  'src/Slider.cpp',
  'src/VirtualKeyboard.cpp',
  'src/LayoutContainer.cpp',
  'src/Layout.cpp',
  'src/VBoxLayout.cpp',
  'src/HBoxLayout.cpp',
  'src/GridLayout.cpp',
]

# Build as shared library
uiframework_lib = shared_library('uiframework',
  sources : common_sources,
  dependencies : [sdl2_dep, ttf_dep, sdl2_image_dep],
  include_directories : ['include'],
  install : true,
  version : '1.0.0'
)

# Build as static library (optional)
uiframework_static = static_library('uiframework',
  sources : common_sources,
  dependencies : [sdl2_dep, ttf_dep, sdl2_image_dep],
  include_directories : ['include'],
  install : true
)

# Install headers
install_headers('include/uiframework/UI.h', subdir : 'uiframework')
install_subdir('include/uiframework', install_dir : get_option('includedir'))

# Progressive test executable
executable('progressive_test',
  sources : ['examples/progressive_test.cpp'] + common_sources,
  dependencies : [sdl2_dep, ttf_dep, sdl2_image_dep],
  include_directories : ['include'],
  install : true
)


if catch2_dep.found()
  executable('ui_tests',
    sources : [
      'tests/test_main.cpp',
      'tests/test_uicore.cpp',
      'tests/test_button.cpp',
      'tests/test_label.cpp',
      'tests/test_checkbox.cpp',
      'tests/test_progressbar.cpp',
      'tests/test_canvas.cpp',
      'tests/test_theme.cpp',
      'tests/test_image.cpp',
      'tests/test_sprite.cpp',
      'tests/test_animatedsprite.cpp',
      'tests/test_listview.cpp',
      'tests/test_modal.cpp',
      'tests/test_contextmenu.cpp',
      'tests/test_textbox.cpp',
      'tests/test_optionselect.cpp',
      'tests/test_deadlock_prevention.cpp',
      'tests/test_slider.cpp',
      'tests/test_virtualkeyboard.cpp',
      'tests/test_layout.cpp',  # Layout tests re-enabled after fixes
      'tests/test_threading_improvements.cpp',
    'tests/test_performance_benchmarks.cpp',  # New threading and performance tests
    ] + common_sources,
    dependencies : [sdl2_dep, ttf_dep, sdl2_image_dep, catch2_dep],
    include_directories : ['include'],
    install : false
  )
endif

