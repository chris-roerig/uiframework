cmake_minimum_required(VERSION 3.16)
project(audio_recorder_example VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find SDL2 dependencies
find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 REQUIRED sdl2)
pkg_check_modules(SDL2_TTF REQUIRED SDL2_ttf)
pkg_check_modules(SDL2_IMAGE REQUIRED SDL2_image)

# UI Framework sources (minimal set for audio recorder)
set(UIFRAMEWORK_SOURCES
    ../../src/UI.cpp
    ../../src/UICore.cpp
    ../../src/UIElement.cpp
    ../../src/Button.cpp
    ../../src/Label.cpp
    ../../src/Canvas.cpp
    ../../src/FontManager.cpp
    ../../src/Helpers.cpp
)

# Create UI Framework library
add_library(uiframework_audio ${UIFRAMEWORK_SOURCES})
target_include_directories(uiframework_audio PUBLIC ../../include)
target_link_libraries(uiframework_audio ${SDL2_LIBRARIES} ${SDL2_TTF_LIBRARIES} ${SDL2_IMAGE_LIBRARIES})
target_compile_options(uiframework_audio PUBLIC ${SDL2_CFLAGS_OTHER})

# Audio Recorder executable
add_executable(audio_recorder
    main.cpp
    AudioRecorder.cpp
)

target_include_directories(audio_recorder PRIVATE ../../include)
target_link_libraries(audio_recorder uiframework_audio ${SDL2_LIBRARIES} ${SDL2_TTF_LIBRARIES} ${SDL2_IMAGE_LIBRARIES})
target_compile_options(audio_recorder PRIVATE ${SDL2_CFLAGS_OTHER})
