cmake_minimum_required(VERSION 3.16)
project(uiframework VERSION 2.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Use pkg-config for SDL2 (most reliable)
find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 REQUIRED sdl2)
pkg_check_modules(SDL2_TTF REQUIRED SDL2_ttf)
pkg_check_modules(SDL2_IMAGE REQUIRED SDL2_image)

# Collect source files
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Create shared library
add_library(uiframework SHARED ${SOURCES})
target_include_directories(uiframework PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_include_directories(uiframework PRIVATE ${SDL2_INCLUDE_DIRS})
target_link_libraries(uiframework ${SDL2_LDFLAGS} ${SDL2_TTF_LDFLAGS} ${SDL2_IMAGE_LDFLAGS})
target_compile_options(uiframework PRIVATE ${SDL2_CFLAGS_OTHER})

# Create static library
add_library(uiframework_static STATIC ${SOURCES})
target_include_directories(uiframework_static PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_include_directories(uiframework_static PRIVATE ${SDL2_INCLUDE_DIRS})
target_link_libraries(uiframework_static ${SDL2_LDFLAGS} ${SDL2_TTF_LDFLAGS} ${SDL2_IMAGE_LDFLAGS})
target_compile_options(uiframework_static PRIVATE ${SDL2_CFLAGS_OTHER})
set_target_properties(uiframework_static PROPERTIES OUTPUT_NAME uiframework)

# Installation
install(TARGETS uiframework uiframework_static
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)
install(DIRECTORY include/uiframework DESTINATION include)

# pkg-config file
configure_file(uiframework.pc.in uiframework.pc @ONLY)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/uiframework.pc DESTINATION lib/pkgconfig)
