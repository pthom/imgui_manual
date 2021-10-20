#!/bin/bash

THIS_SCRIPT_DIR="$( cd "$( dirname "$0" )" && pwd )"
IMGUI_DIR=$THIS_SCRIPT_DIR/../../external/imgui

source ~/Utils/emsdk/emsdk_env.sh
cd $IMGUI_DIR/examples/example_emscripten_opengl3
USE_FILE_SYSTEM=1 make && make serve
