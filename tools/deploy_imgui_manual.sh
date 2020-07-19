#!/usr/bin/env bash

THIS_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
REPO_DIR=$THIS_DIR/..
cd $REPO_DIR

./tools/emscripten_build.sh
cd build_emscripten
rsync -vaz src pascal@traineq.org:HTML/imgui_manual
