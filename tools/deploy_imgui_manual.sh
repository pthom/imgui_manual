#!/usr/bin/env bash

THIS_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
REPO_DIR=$THIS_DIR/..
cd $REPO_DIR

if [ ! -d imgui_manual_online ]; then
  echo "Sorry, this script is reserved for the author!"
  exit 1
fi

./tools/emscripten_build.sh
cp build_emscripten/src/imgui_manual* imgui_manual_online/static/manual
cd imgui_manual_online
hugo -d docs
git add static/manual docs/manual
git commit -m "update / build on $(date)"
git push
