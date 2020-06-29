cmake_minimum_required(VERSION 3.10)
project(my_app LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 14)

set(HELLOIMGUI_USE_SDL_OPENGL3 ON)
add_subdirectory(external/hello_imgui)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${HELLOIMGUI_BASEPATH}/hello_imgui_cmake)
if (EMSCRIPTEN)
    include(${HELLOIMGUI_BASEPATH}/hello_imgui_cmake/emscripten/hello_imgui_emscripten_global_options.cmake)
endif()

add_subdirectory(src)
