#!/usr/bin/env bash

echo "Populating assets"

THIS_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
cd $THIS_DIR/../external || exit 1
SRC_DIR=$THIS_DIR
EXTERNAL_DIR=$THIS_DIR/../external

echo "THIS_DIR=$THIS_DIR"
#exit 0

cd $THIS_DIR
if [ ! -d assets/code ]; then
  mkdir -p assets/code
fi

cd assets/code
cp -f $SRC_DIR/demo.cpp .
cp -f $SRC_DIR/CMakeLists.txt .

if [ ! -d implot ]; then
  mkdir implot
fi
cd implot
cp -f $EXTERNAL_DIR/implot/* .
cd ..

if [ ! -d ImGuiColorTextEdit ]; then
  mkdir ImGuiColorTextEdit
fi
cd ImGuiColorTextEdit
cp -f $EXTERNAL_DIR/ImGuiColorTextEdit/* .
cd ..
