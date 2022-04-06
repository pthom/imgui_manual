#!/bin/bash

# Environment variables
red=`tput setaf 1`
purple=`tput setaf 93`
blue=`tput setaf 21`
green=`tput setaf 2`
gold=`tput setaf 214`
reset=`tput sgr0`

# we need to add [x11] parameter to vcpkg on line 25 like so - run("./vcpkg install glfw3 sdl2[x11]") so I just copy the modified file
echo "Running command:
    ${green}cp vcpkg_install_third_parties.py $jrpc_beta/external/hello_imgui/tools/${reset}"
cp vcpkg_install_third_parties.py $jrpc_beta/external/hello_imgui/tools/

echo "Running command:
    ${green}docker-compose -p JRPC-linux-buildsystèmes -f $jrpc_beta/build/build-Linux.yaml up --no-deps --build${reset}"
docker-compose -p JRPC-linux-buildsystèmes -f $jrpc_beta/build/build-Linux.yaml up --no-deps --build

echo "Executing command:
    ${green}$jrpc_beta/bin/src/imgui_manual${reset}"
cd $jrpc_beta/bin/src
$jrpc_beta/bin/src/imgui_manual