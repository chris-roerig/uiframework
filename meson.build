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
  'lib/src/FontConfig.cpp',
  'lib/src/TTFManager.cpp',
  'lib/src/UIElement.cpp',
  'lib/src/InteractiveElement.cpp',
  'lib/src/Utils/TextUtils.cpp',
  'lib/src/Rendering/RenderContext.cpp',
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
  'lib/src/ConstraintManager.cpp',
  'lib/src/GridSnap.cpp',
  'lib/src/PercentageSize.cpp',
  'lib/src/ElementPool.cpp',
  'lib/src/RealTime/UIUpdateQueue.cpp',
  'lib/src/EmbeddedFontRegistry.cpp',
  'lib/src/Utils/BorderRenderer.cpp',
  'lib/src/Utils/TooltipRenderer.cpp',
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

# Comprehensive Demo - Complete UI Framework Showcase
executable('comprehensive_demo',
  sources : ['examples/comprehensive_demo.cpp'] + common_sources,
  dependencies : [sdl2_dep, ttf_dep, sdl2_image_dep],
  include_directories : ['lib/include'],
  install : false
)

# Constraint System Demo - Phase 1 Implementation
executable('constraint_demo',
  sources : ['examples/constraint_demo.cpp'] + common_sources,
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
      'dev/tests/test_constraints.cpp',  # Phase 1 constraint system tests
      'dev/tests/test_threading_improvements.cpp',
      'dev/tests/test_performance_benchmarks.cpp',  # New threading and performance tests
      'dev/tests/test_phase1_improvements.cpp',  # Phase 1 improvement tests
      'dev/tests/test_integration.cpp',  # Phase 2 integration tests
      'dev/tests/test_memory_benchmarks.cpp',  # Phase 2 memory benchmarks
      'dev/tests/test_focus_hints_cleanup.cpp',  # Phase 2 focus hints cleanup test
      'dev/tests/test_coordinate_validation.cpp',  # Phase 3 coordinate validation test
      'dev/tests/test_weakptr_cleanup.cpp',  # Phase 4 weak_ptr cleanup test
      'dev/tests/test_lockfree_queue.cpp',  # Phase 1 audio-ready: lock-free queue test
      'dev/tests/test_batching_system.cpp',  # Phase 2 audio-ready: batching system test
      'dev/tests/test_nonblocking_access.cpp',  # Phase 3 audio-ready: non-blocking access test
      'dev/tests/test_predictable_memory.cpp',  # Phase 4 audio-ready: memory-predictable test
      'dev/tests/test_timing_support.cpp',  # Phase 5 audio-ready: high-resolution timing test
      'dev/tests/test_bulk_operations.cpp'  # Phase 6 bulk updates: SIMD-optimized bulk operations test
    ] + common_sources,
    dependencies : [sdl2_dep, ttf_dep, sdl2_image_dep, catch2_dep],
    include_directories : ['lib/include'],
    install : false
  )
endif

