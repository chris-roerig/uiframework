project('sdl_ui_demo', 'cpp',
  version : '0.1.0',
  default_options : ['cpp_std=c++17', 'buildtype=release']
)

# Add compiler arguments to fix the libcpp assertions issue
add_project_arguments('-U_LIBCPP_ENABLE_ASSERTIONS', language : 'cpp')

sdl2_dep = dependency('sdl2', required: true)
ttf_dep = dependency('SDL2_ttf', required: true)
sdl2_image_dep = dependency('sdl2_image', required: true)
catch2_dep = dependency('catch2-with-main', required: true, fallback: ['catch2', 'catch2_with_main_dep'])

# Common source files
common_sources = [
  'src/Helpers.cpp',
  'src/UICore.cpp',
  'src/UI.cpp',
  'lib/Resources/FontManager.cpp',
  'lib/UIElements/UIElement.cpp',
  'lib/UIElements/ContextMenu.cpp',
  'lib/UIElements/Modal.cpp',
  'lib/UIElements/ListView.cpp',
  'lib/UIElements/Label.cpp',
  'lib/UIElements/Button.cpp',
  'lib/UIElements/TextBox.cpp',
  'lib/UIElements/CheckBox.cpp',
  'lib/UIElements/OptionSelect.cpp',
  'lib/UIElements/Canvas.cpp',
  'lib/UIElements/Image.cpp',
  'lib/UIElements/Sprite.cpp',
  'lib/UIElements/AnimatedSprite.cpp',
  'lib/UIElements/ProgressBar.cpp',
  'lib/UIElements/Slider.cpp',
  'lib/UIElements/VirtualKeyboard.cpp',
  'lib/UIElements/LayoutContainer.cpp',
  'lib/Layout/Layout.cpp',
  'lib/Layout/VBoxLayout.cpp',
  'lib/Layout/HBoxLayout.cpp',
  'lib/Layout/GridLayout.cpp',
]

# Progressive test executable
executable('progressive_test',
  sources : ['examples/progressive_test.cpp'] + common_sources,
  dependencies : [sdl2_dep, ttf_dep, sdl2_image_dep],
  include_directories : ['.', 'src', 'lib'],
  install : true
)


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
  ] + common_sources,
  dependencies : [sdl2_dep, ttf_dep, sdl2_image_dep, catch2_dep],
  include_directories : ['.', 'src', 'lib'],
  install : false
)

