# Using UI Framework in Your Project

## Method 1: System Installation

```bash
# Install framework
cd uiframework
./install_lib.sh

# In your project
pkg-config --cflags --libs uiframework
```

## Method 2: Submodule/Subdirectory

```cmake
# CMakeLists.txt
add_subdirectory(uiframework)
target_link_libraries(your_app uiframework)
```

## Method 3: Meson Subproject

```meson
# meson.build
uiframework_dep = dependency('uiframework', fallback : ['uiframework', 'uiframework_dep'])
executable('your_app', 'main.cpp', dependencies : uiframework_dep)
```

## Simple Usage

```cpp
#include <UIFramework.h>

int main() {
    UI ui("My App", 800, 600);
    
    auto button = ui.createButton("Click me!", 10, 10, [](){
        std::cout << "Clicked!" << std::endl;
    });
    
    ui.run();
    return 0;
}
```
