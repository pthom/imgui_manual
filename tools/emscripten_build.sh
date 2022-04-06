#!/usr/bin/env bash

THIS_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
REPO_DIR=$THIS_DIR/..
cd $REPO_DIR

if [ ! -d bin_emscripten ]; then
  mkdir bin_emscripten
fi

cd bin_emscripten
source /emsdk/emsdk_env.sh
emcmake cmake ..
make -j 4
