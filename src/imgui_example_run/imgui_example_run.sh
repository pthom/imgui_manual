#!/bin/bash

THIS_SCRIPT_DIR="$( cd "$( dirname "$0" )" && pwd )"
IMGUI_DIR=$THIS_SCRIPT_DIR/../../external/imgui
cd $IMGUI_DIR/examples/example_sdl_opengl3
make && ./example_sdl_opengl3
