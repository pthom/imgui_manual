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

if [ ! -d imgui_markdown ]; then
  mkdir imgui_markdown
fi
cd imgui_markdown
cp -f $EXTERNAL_DIR/imgui_markdown/* .
cd ..


if [ ! -d hello_imgui ]; then
  mkdir hello_imgui
fi
cd hello_imgui
cp -f $EXTERNAL_DIR/hello_imgui/src/hello_imgui/*.h .
cp -f $EXTERNAL_DIR/hello_imgui/src/hello_imgui/*.md .
cp -f $EXTERNAL_DIR/hello_imgui/LICENSE .
cp -f $EXTERNAL_DIR/hello_imgui/README.md .
rm *.src.md
cd ..

if [ ! -d implot_demo ]; then
  mkdir implot_demo
fi
cd implot_demo
cp -f $SRC_DIR/*.* .
cp $SRC_DIR/../Readme.md .
cp $SRC_DIR/../LICENSE .
cd ..
