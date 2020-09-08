#!/usr/bin/env bash

THIS_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
REPO_DIR=$THIS_DIR/..
cd $REPO_DIR

if [ ! -d imgui_manual_online ]; then
  echo "Sorry, this script is reserved for the author!"
  exit 1
fi

./tools/emscripten_build_sokol.sh
cp build_emscripten_sokol/src/imgui_manual* imgui_manual_online/static/manual_sokol
cd imgui_manual_online
hugo -d docs
git add static/manual_sokol docs/manual_sokol
git commit -m "update sokol manual / build on $(date)"
git push
