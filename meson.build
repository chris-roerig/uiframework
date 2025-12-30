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
  'lib/src/Helpers.cpp',
  'lib/src/UICore.cpp', 
  'lib/src/UI.cpp',
  'lib/src/Logger.cpp',
  'lib/src/FontManager.cpp',
  'lib/src/TTFManager.cpp',
  'lib/src/UIElement.cpp',
  'lib/src/Focus/FocusManager.cpp',
  'lib/src/ContextMenu.cpp',
  'lib/src/Modal.cpp',
  'lib/src/ListView.cpp',
  'lib/src/Label.cpp',
  'lib/src/Button.cpp',
  'lib/src/TextBox.cpp',
  'lib/src/CheckBox.cpp',
  'lib/src/OptionSelect.cpp',
  'lib/src/Canvas.cpp',
  'lib/src/Image.cpp',
  'lib/src/Sprite.cpp',
  'lib/src/AnimatedSprite.cpp',
  'lib/src/ProgressBar.cpp',
  'lib/src/Slider.cpp',
  'lib/src/VirtualKeyboard.cpp',
  'lib/src/UIElements/CycleList.cpp',
  'lib/src/UIElements/TabbedPanel.cpp',
  'lib/src/LayoutContainer.cpp',
  'lib/src/Layout.cpp',
  'lib/src/VBoxLayout.cpp',
  'lib/src/HBoxLayout.cpp',
  'lib/src/GridLayout.cpp',
]

# Build as shared library
uiframework_lib = shared_library('uiframework',
  sources : common_sources,
  dependencies : [sdl2_dep, ttf_dep, sdl2_image_dep],
  include_directories : ['lib/include'],
  install : true,
  version : '1.0.0'
)

# Build as static library (optional)
uiframework_static = static_library('uiframework',
  sources : common_sources,
  dependencies : [sdl2_dep, ttf_dep, sdl2_image_dep],
  include_directories : ['lib/include'],
  install : true
)

# Install headers
install_headers('lib/include/uiframework/UI.h', subdir : 'uiframework')
install_subdir('lib/include/uiframework', install_dir : get_option('includedir'))

# Progressive test executable
executable('progressive_test',
  sources : ['dev/examples/progressive_test.cpp'] + common_sources,
  dependencies : [sdl2_dep, ttf_dep, sdl2_image_dep],
  include_directories : ['lib/include'],
  install : true
)

# DSL demo executable
executable('dsl_demo',
  sources : ['dev/examples/dsl_demo.cpp'] + common_sources,
  dependencies : [sdl2_dep, ttf_dep, sdl2_image_dep],
  include_directories : ['lib/include', 'dev/examples'],
  install : false
)

# Focus cycling demo
executable('focus_demo',
  sources : ['examples/focus_demo.cpp'] + common_sources,
  dependencies : [sdl2_dep, ttf_dep, sdl2_image_dep],
  include_directories : ['lib/include'],
  install : false
)

# Focus groups demo (Phase 4)
executable('focus_groups_demo',
  sources : ['examples/focus_groups_demo.cpp'] + common_sources,
  dependencies : [sdl2_dep, ttf_dep, sdl2_image_dep],
  include_directories : ['lib/include'],
  install : false
)


# Static analysis targets
run_target('static-analysis',
  command : [find_program('tools/scripts/run_static_analysis.sh')]
)

if catch2_dep.found()
  executable('ui_tests',
    sources : [
      'dev/tests/test_main.cpp',
      'dev/tests/test_uicore.cpp',
      'dev/tests/test_button.cpp',
      'dev/tests/test_label.cpp',
      'dev/tests/test_checkbox.cpp',
      'dev/tests/test_progressbar.cpp',
      'dev/tests/test_canvas.cpp',
      'dev/tests/test_theme.cpp',
      'dev/tests/test_image.cpp',
      'dev/tests/test_sprite.cpp',
      'dev/tests/test_animatedsprite.cpp',
      'dev/tests/test_listview.cpp',
      'dev/tests/test_modal.cpp',
      'dev/tests/test_contextmenu.cpp',
      'dev/tests/test_textbox.cpp',
      'dev/tests/test_optionselect.cpp',
      'dev/tests/test_deadlock_prevention.cpp',
      'dev/tests/test_slider.cpp',
      'dev/tests/test_virtualkeyboard.cpp',
      'dev/tests/test_cyclelist.cpp',
      'dev/tests/test_tabbedpanel.cpp',
      'dev/tests/test_focusmanager.cpp',
      'dev/tests/test_focus_visibility.cpp',
      'dev/tests/test_tab_navigation.cpp',
      'dev/tests/test_focus_groups.cpp',
      'dev/tests/test_layout.cpp',  # Layout tests re-enabled after fixes
      'dev/tests/test_threading_improvements.cpp',
      'dev/tests/test_performance_benchmarks.cpp',  # New threading and performance tests
      'dev/tests/test_phase1_improvements.cpp',  # Phase 1 improvement tests
      'dev/tests/test_integration.cpp',  # Phase 2 integration tests
      'dev/tests/test_memory_benchmarks.cpp',  # Phase 2 memory benchmarks
      'dev/tests/test_focus_hints_cleanup.cpp',  # Phase 2 focus hints cleanup test
      'dev/tests/test_coordinate_validation.cpp',  # Phase 3 coordinate validation test
      'dev/tests/test_weakptr_cleanup.cpp'  # Phase 4 weak_ptr cleanup test
    ] + common_sources,
    dependencies : [sdl2_dep, ttf_dep, sdl2_image_dep, catch2_dep],
    include_directories : ['lib/include'],
    install : false
  )
endif

