#!/bin/bash

. ./tools/setenv.sh 

if [ -x "$(command -v git)" ]; then
    echo "${gold}git version${reset}"
    git --version
else
    echo "${gold}Installing git${reset}"
    sudo apt-get install git
fi

if [ -x "$(command -v docker)" ]; then
    echo "${gold}docker version${reset}"
    docker --version
else
    echo "${gold}Installing docker${reset}"
    sudo apt-get install docker
fi

if [ -x "$(command -v docker-compose)" ]; then
    echo "${gold}docker-compose version${reset}"
    docker-compose --version
else
    echo "${gold}Installing docker-compose${reset}"
    sudo apt-get install docker-compose
fi

echo -e "${gold}Initializing git repo and dependencies${reset}"
git submodule update --init --recursive
banner

while :
do
  # Getting necessary user input
  read -p "What do you want to do? 
  ${green}1)${reset}: Build IoTeX Full Node
  ${green}2)${reset}: Build IoTeX Pebble Tracker Firmware
  ${green}3)${reset}: Build IoTeX JRPC Blockchain Visualizer
  input> " APPLICATION

  case $APPLICATION in
    1) # Build IoTeX Full Node
      cd $jrpc_beta/external/JRPC/
      bash $jrpc_beta/external/JRPC/install.sh 
      break ;;

    2) # Build IoTeX Pebble Tracker Firmware
      cd $jrpc_beta/external/pebble-firmware/
      bash $jrpc_beta/external/pebble-firmware/install.sh
      break ;;

    3) # Build IoTeX JRPC Blockchain Visualizer
      cd $jrpc_beta/build/
      bash $jrpc_beta/build/install.sh 
      break ;;
      
    *)
      echo -e "${red}Incorrect selection. Please make a valid choice${reset}"

  esac
done