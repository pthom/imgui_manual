# implot demo

Source for an online emscripten demo of [implot](https://github.com/epezent/implot)


## Build

### Emscripten

### Desktop

````
git submodule update --init --recursive
external/hello_imgui/tools/vcpkg_install_third_parties.py
mkdir build
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=../external/hello_imgui/vcpkg/scripts/buildsystems/vcpkg.cmake
````
