project('audio_recorder_example', 'cpp',
    version : '1.0.0',
    default_options : ['cpp_std=c++17'])

# Dependencies
sdl2_dep = dependency('sdl2')
sdl2_ttf_dep = dependency('SDL2_ttf')
sdl2_image_dep = dependency('SDL2_image')

# UI Framework dependency (assuming it's built)
uiframework_inc = include_directories('../../include')
uiframework_lib = static_library('uiframework',
    sources : [
        '../../src/UI.cpp',
        '../../src/UICore.cpp',
        '../../src/UIElement.cpp',
        '../../src/Button.cpp',
        '../../src/Label.cpp',
        '../../src/Canvas.cpp',
        '../../src/FontManager.cpp',
        '../../src/Helpers.cpp',
        '../../src/TextBox.cpp',
        '../../src/CheckBox.cpp',
        '../../src/OptionSelect.cpp',
        '../../src/VBoxLayout.cpp',
        '../../src/HBoxLayout.cpp',
        '../../src/GridLayout.cpp',
        '../../src/Layout.cpp',
        '../../src/LayoutContainer.cpp',
        '../../src/ListView.cpp',
        '../../src/Modal.cpp',
        '../../src/ProgressBar.cpp',
        '../../src/Slider.cpp',
        '../../src/Image.cpp',
        '../../src/Sprite.cpp',
        '../../src/AnimatedSprite.cpp',
        '../../src/ContextMenu.cpp',
        '../../src/VirtualKeyboard.cpp'
    ],
    include_directories : uiframework_inc,
    dependencies : [sdl2_dep, sdl2_ttf_dep, sdl2_image_dep])

# Audio Recorder Example
audio_recorder_sources = [
    'main.cpp',
    'AudioRecorder.cpp'
]

executable('audio_recorder',
    sources : audio_recorder_sources,
    include_directories : uiframework_inc,
    dependencies : [sdl2_dep, sdl2_ttf_dep, sdl2_image_dep],
    link_with : uiframework_lib,
    install : false)

# Simple test executable
executable('test_audio',
    sources : ['test_audio.cpp', 'AudioRecorder.cpp'],
    dependencies : [sdl2_dep],
    install : false)

# UI-only test executable
executable('ui_test',
    sources : ['ui_test.cpp'],
    include_directories : uiframework_inc,
    dependencies : [sdl2_dep, sdl2_ttf_dep, sdl2_image_dep],
    link_with : uiframework_lib,
    install : false)

# Simple audio recorder executable
executable('simple_audio_recorder',
    sources : ['simple_audio_recorder.cpp', 'AudioRecorder.cpp'],
    include_directories : uiframework_inc,
    dependencies : [sdl2_dep, sdl2_ttf_dep, sdl2_image_dep],
    link_with : uiframework_lib,
    install : false)

# Font debugging test
executable('font_test',
    sources : ['font_test.cpp'],
    include_directories : uiframework_inc,
    dependencies : [sdl2_dep, sdl2_ttf_dep, sdl2_image_dep],
    link_with : uiframework_lib,
    install : false)

# Font debugging executable
executable('font_debug',
    sources : ['font_debug.cpp'],
    include_directories : uiframework_inc,
    dependencies : [sdl2_dep, sdl2_ttf_dep],
    link_with : uiframework_lib,
    install : false)

# Button color test
executable('button_color_test',
    sources : ['button_color_test.cpp'],
    include_directories : uiframework_inc,
    dependencies : [sdl2_dep, sdl2_ttf_dep, sdl2_image_dep],
    link_with : uiframework_lib,
    install : false)
