#!/usr/bin/env bash

# Simple build and deployment script for ImGui Manual
# Builds the manual with emscripten and deploys to GitHub Pages

set -e

THIS_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
REPO_DIR=$THIS_DIR/..
cd $REPO_DIR

if [ ! -d imgui_manual_online ]; then
  echo "Error: imgui_manual_online directory not found!"
  exit 1
fi

# Build with emscripten
echo "Building ImGui Manual with emscripten..."
./tools/emscripten_build.sh

# Copy built files to manual directory
echo "Copying built files to imgui_manual_online/manual/..."
cp build_emscripten/src/imgui_manual* imgui_manual_online/website/manual/

# Commit and push changes
cd imgui_manual_online
echo "Committing and pushing changes..."
git add website/
git commit -m "Update manual - build on $(date '+%Y-%m-%d %H:%M:%S')"
git push

echo "Deployment complete!"

