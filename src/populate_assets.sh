#!/usr/bin/env bash

echo "Populating assets"

THIS_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
cd $THIS_DIR/../external || exit 1
SRC_DIR=$THIS_DIR
EXTERNAL_DIR=$THIS_DIR/../external
IMGUI_DIR=$THIS_DIR/../external/imgui/

echo "THIS_DIR=$THIS_DIR"
cd $THIS_DIR
if [ ! -d assets/code ]; then
  mkdir -p assets/code
fi
cd assets/code


if [ ! -d imgui_manual ]; then
  mkdir imgui_manual
fi
cd imgui_manual
cp -f $SRC_DIR/*.* .
cp -f $SRC_DIR/../Readme.md .
cp -f $SRC_DIR/../LICENSE .
cp -r $SRC_DIR/source_parse .
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

if [ ! -d imgui ]; then
  mkdir imgui
fi
cd imgui
cp $IMGUI_DIR/* .
cp $IMGUI_DIR/docs/README.md .
cp $IMGUI_DIR/docs/FAQ.md .
cd ..

if [ ! -d fplus ]; then
  mkdir fplus
fi
cd fplus
fplus_dir=$EXTERNAL_DIR/FunctionalPlus
cp $fplus_dir/*.md .
cp $fplus_dir/LICENSE .
cp $fplus_dir/include/fplus/fplus.hpp .
cd ..
