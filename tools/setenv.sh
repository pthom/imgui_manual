#!/bin/bash

# Environment variables
red=`tput setaf 1`
purple=`tput setaf 93`
blue=`tput setaf 21`
green=`tput setaf 2`
gold=`tput setaf 214`
reset=`tput sgr0`

export jrpc_beta=$PWD
export JRPC_Blockchain_Visualizer_build_command=``

function banner {
echo "     ____._____________________________  
    |    |\______   \______   \_   ___ \ 
    |    | |       _/|     ___/    \  \/ 
/\__|    | |    |   \|    |   \     \____
\________| |____|_  /|____|    \______  /
                  \/                  \/ "
}

