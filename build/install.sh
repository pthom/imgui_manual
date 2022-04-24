#!/bin/bash

# Environment variables
red=`tput setaf 1`
purple=`tput setaf 93`
blue=`tput setaf 21`
green=`tput setaf 2`
gold=`tput setaf 214`
reset=`tput sgr0`

read -p "Wat do you want to do with the JRPC Blockchain Visualizer?
  ${green}1)${reset}: Build the Demo Application 
  ${green}2)${reset}: Run the JRPC Blockchain Visualizer Docker container in Debug mode
  " JRPC_Blockchain_Visualizer_flag

case $JRPC_Blockchain_Visualizer_flag in
  1) # Build Application
    export COMMAND="bash -c 'python ../external/hello_imgui/tools/vcpkg_install_third_parties.py && cmake .. -DCMAKE_TOOLCHAIN_FILE=../external/hello_imgui/vcpkg/scripts/buildsystems/vcpkg.cmake && make -j 4 && /imgui_manual/tools/emscripten_build.sh'"
    
    echo -e "Installing custom third party build script: \n${green}cp vcpkg_install_third_parties.py $jrpc_beta/external/hello_imgui/tools/${reset}"
    cp $jrpc_beta/build/vcpkg_install_third_parties.py $jrpc_beta/external/hello_imgui/tools/
    
    echo -e "Starting docker-compose script: \n${green}docker-compose -p JRPC-linux-buildsystèmes -f $jrpc_beta/build/build-Linux.yaml up --no-deps --build${reset}"
    docker-compose -p JRPC-linux-buildsystèmes -f $jrpc_beta/build/build-Linux.yaml up --no-deps --build
    
    echo -e "Starting Application: \n${green}$jrpc_beta/bin/src/imgui_manual${reset}"
    cd $jrpc_beta/bin/src
    $jrpc_beta/bin/src/imgui_manual ;;

  2) # Run the Docker container in Debug mode
    export COMMAND="sleep infinity"
    echo -e "Starting docker-compose script: \n${green}nohup docker-compose -p JRPC-linux-buildsystèmes -f $jrpc_beta/build/build-Linux.yaml up --no-deps --build &${reset}"
    nohup docker-compose -p JRPC-linux-buildsystèmes -f $jrpc_beta/build/build-Linux.yaml up --no-deps --build &
    sleep 5
    
    echo -e "Entering into the Cocker container in debug mode: \n${green}docker exec -it jrpc-linux-buildsystem /bin/bash${reset}"
    docker exec -it jrpc-linux-buildsystem /bin/bash 
    
    echo -e "Stopping docker-compose script: \n${green}docker-compose -p JRPC-linux-buildsystèmes -f $jrpc_beta/build/build-Linux.yaml down${reset}"
    docker-compose -p JRPC-linux-buildsystèmes -f $jrpc_beta/build/build-Linux.yaml down ;;
esac