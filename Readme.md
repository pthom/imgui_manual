# Dear ImGui: an interactive manual

Source for an online interactive manual for [Dear ImGui](https://github.com/ocornut/imgui).

[The manual is online here](https://traineq.org/imgui_manual/src/imgui_manual.html)

## Build instructions

First, init the submodules:
````
git submodule update --init --recursive
````


### Build instructions for emscripten

Install emscripten if not present

````
./external/hello_imgui/tools/emscripten/install_emscripten.sh
````

Build for emscripten using [tools/emscripten_build.sh](tools/emscripten_build.sh)
````
./tools/emscripten_build.sh
````

Launch a web server
````
python3 -m http.server
````

Then, browse to http://localhost:8000/src/implot_demo.html

### Build instructions on desktop (linux, MacOS, Windows)

Install third parties via vcpkg (SDL)
````
python  external/hello_imgui/tools/vcpkg_install_third_parties.py
````

Run cmake, using vcpkg toolchain
````
mkdir build
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=../external/hello_imgui/vcpkg/scripts/buildsystems/vcpkg.cmake
````

Build and run
````
make -j 4
./src/implot_demo
````


### Note
This demo is based on [Hello ImGui](https://github.com/pthom/hello_imgui).
