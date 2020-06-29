# implot demo

Source for an online emscripten demo of [implot](https://github.com/epezent/implot)


## Build

````
git submodule update --init --recursive
````

### Emscripten

````
./external/hello_imgui/tools/emscripten/install_emscripten.sh
````

````
mkdir build_emscripten
cd build_emscripten
source ~/emsdk/emsdk_env.sh 
emcmake cmake ..
````

````
python3 -m http.server

````

### Desktop

````
external/hello_imgui/tools/vcpkg_install_third_parties.py
mkdir build
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=../external/hello_imgui/vcpkg/scripts/buildsystems/vcpkg.cmake
````
