#!/bin/bash

. ./tools/setenv.sh 

if [ -x "$(command -v git)" ]; then
    echo "${red}git version${reset}"
    git --version
else
    echo "Installing git"
    sudo apt-get install git
fi

if [ -x "$(command -v docker)" ]; then
    echo "${red}docker version${reset}"
    docker --version
else
    echo "Installing docker"
    sudo apt-get install docker
fi

if [ -x "$(command -v docker-compose)" ]; then
    echo "${red}docker-compose version${reset}"
    docker-compose --version
else
    echo "Installing docker-compose"
    sudo apt-get install docker-compose
fi

echo -e "Initializing git repo and dependencies: \n${red}git submodule update --init --recursive${reset}"
git submodule update --init --recursive

# Getting necessary user input
read -p "What do you want to do? 
  (${red}1${reset}): build and run the JRPC IoTeX Full Node install script
  (${green}2${reset}): build a different JRPC Application
" APPLICATION

case $APPLICATION in
  1) # Build IoTeX Full Node
    cd $jrpc_beta/external/JRPC/
    bash $jrpc_beta/external/JRPC/install.sh ;;

  2) # Build Application
    read -p "Choose the application to build:
      (${red}1${reset}): Pebble Tracker Firmware 
      (${green}2${reset}): JRPC Blockchain Visualiser Demo 
      " APPLICATION_flag
esac

case $APPLICATION_flag in 
  1) # Build Pebble Tracker Firmware Build System
    cd $jrpc_beta/external/pebble-firmware/
    bash $jrpc_beta/external/pebble-firmware/install.sh ;;

  2) # Build JRPC Beta Data Visualiser
    # cd $jrpc_beta/build
    # bash $jrpc_beta/build/install.sh ;;
    read -p "Do you want to do with the JRPC Blockchain Visualizer Demo?
      (${red}1${reset}): Build Application 
      (${green}2${reset}): Run the Docker container in Debug mode
      " JRPC_Blockchain_Visualizer_flag

    case $JRPC_Blockchain_Visualizer_flag in
      1) # Build Application
        export COMMAND="bash -c 'python ../external/hello_imgui/tools/vcpkg_install_third_parties.py && cmake .. -DCMAKE_TOOLCHAIN_FILE=../external/hello_imgui/vcpkg/scripts/buildsystems/vcpkg.cmake && make -j 4 && /imgui_manual/tools/emscripten_build.sh'"
        echo -e "Installing custom third party build script: \n${green}cp vcpkg_install_third_parties.py $jrpc_beta/external/hello_imgui/tools/${reset}"
        cp vcpkg_install_third_parties.py $jrpc_beta/external/hello_imgui/tools/
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
esac

# MODE_flag=$(case "$MODE" in
#   (1)    
#     echo "--no-deps --build" 
#     sudo apt-get install docker >&-
#     sudo apt-get install docker-compose >&-
#     git submodule update --init --recursive >&-;;
#   (2)    echo ""  ;;
# esac)

# # Getting necessary user input
# read -p "Choose your Destiny:
#         (${red}1${reset}): Build Full IoTeX Node 
#         (${green}2${reset}): Build a different Application 
#         " APPLICATION


# case APPLICATION in
#   (1)
#     cd $jrpc_beta/external/JRPC/
#     bash $jrpc_beta/external/JRPC/install.sh ;;
#   (2)
#     read -p "Which Application do you want to build?
#         (${red}F${reset}) IoTeX Full Node
#         (${green}A${reset}) Pebble Tracker Firmware update buildkit
#         " TYPE 



# TYPE_flag=$(case "$TYPE" in
#   (F|f)    
#     cd $jrpc_beta/external/JRPC/
#     bash $jrpc_beta/external/JRPC/install.sh ;;
#   (A|a)    
#     cd $jrpc_beta/external/pebble-firmware/
#     bash $jrpc_beta/external/pebble-firmware/install.sh ;;
# esac)

# # Install dependencies
# sudo apt-get install docker
# sudo apt-get install docker-compose
# git submodule update --init --recursive






# # Environment variables
# red=`tput setaf 1`
# purple=`tput setaf 93`
# blue=`tput setaf 21`
# green=`tput setaf 2`
# yellow=`tput setaf 214`
# reset=`tput sgr0`

# MODE_flag=``
# DEBUG_flag=``
# DOWNLOADDATA_flag=``

# latestversion=$(curl --silent "https://api.github.com/repos/iotexproject/iotex-core/releases/latest" | grep -Po '"tag_name": "\K.*?(?=")')

# . ./tools/setenv.sh $latestversion

# # Start of script - getting necessary user input
# read -p "Do you want to install Testnet (${red}T${reset}) or Mainnet (${green}M${reset})?: " MODE
# MODE_flag=$(case "$MODE" in
#   (T|t)    echo "Testnet"  ;;
#   (M|m)    echo "Mainnet"  ;;
# esac)

# read -p "Do you want to set up the node in Debug (${red}D${reset}) or Release (${green}R${reset}) mode?: " DEBUG
# DEBUG_flag=$(case "$DEBUG" in
#   (D|d)    echo ""    ;;
#   (R|r)    echo "-d"  ;;
# esac)

# read -p "Do you want to Download the latest data? No (${red}N${reset}) or Yes (${green}Y${reset}): " DOWNLOADDATA
# DOWNLOADDATA_flag=$(case "$DOWNLOADDATA" in
#   (Y|y)    echo "true"    ;;
#   (N|n)    echo "false"  ;;
# esac)

# read -p "Do you want to build the Analytics node as well? No (${red}N${reset}) or Yes (${green}Y${reset}): " ANALYTICS
# ANALYTICS_flag=$(case "$ANALYTICS" in
#   (Y|y)    echo "analytics"    ;;
#   (N|n)    echo "minimal"  ;;
# esac)


# # make directory for JRPC log files
# mkdir -p $JRPC/log/$MODE_flag/JRPC
# LOGFILE=$JRPC/log/$MODE_flag/JRPC/message.log


# echo "**************" | tee -a "$LOGFILE"
# echo "$(date)" | tee -a "$LOGFILE"
# echo "Mode: $MODE_flag" | tee -a "$LOGFILE"
# echo "Debug: $DEBUG" | tee -a "$LOGFILE"
# echo "Download Data: $DOWNLOADDATA_flag" | tee -a "$LOGFILE"
# echo -e "*************\n" | tee -a "$LOGFILE"


# # Set URL to get testnet or mainnet data, depending on user's choice
# URL=https://t.iotex.me/$MODE_flag-data-with-idx-latest


# # Confirm if we continue with the installation of the latest version - and inform the user of the download data filesize
# case $DOWNLOADDATA_flag in
#   true)
#     echo "This will download ${blue}$(curl -sIL https://t.iotex.me/$MODE_flag-data-with-idx-latest | grep -i x-goog-stored-content-length | awk '{print $2/1024/1024/1024 " GB"}')${reset} from https://t.iotex.me/$MODE_flag-data-with-idx-latest" ;;
#   false)
#     echo "This script will attempt to use data located in ${green}$JRPC/data/$MODE_flag/IoTeX${reset}"
#     echo "If there is no data in this directory, the blockchain will attempt to synchronize from block height 0. You will need to provide your infura key in the configuration files" ;;
#   *)
#     echo "${red}Not a valid answer. Terminating${reset}"
#     exit 0 ;;
# esac


# read -p "${green}Do you wish to proceed with the installation?${reset} No (${red}N${reset}) or Yes (${green}Y${reset}): " CONTINUE
# case $CONTINUE in
#   y|Y)
#     echo "We will now install the $MODE_flag IoTeX server node $latestversion" ;;
#   n|N)
#     echo "${red}Terminating install${reset}"
#     exit 0 ;;
#   *)
#     echo "${red}Not a valid answer. Terminating${reset}"
#     exit 0 ;;
# esac


# # Setting up file and folder structure, and extract the downloaded data into it
# echo "${green}Creating $JRPC/data/$MODE_flag/IoTeX directory${reset}"
# mkdir -p $JRPC/data/$MODE_flag/IoTeX

# echo "${green}Creating $JRPC/log/$MODE_flag/IoTeX directory${reset}"
# mkdir -p $JRPC/log/$MODE_flag/IoTeX

# echo "${green}Creating $JRPC/temp/$MODE_flag/IoTeX directory${reset}"
# mkdir -p $JRPC/temp/$MODE_flag/IoTeX


# case $DOWNLOADDATA_flag in
#   true) 
#     # If in future releases IoTeX requires a data patch, uncomment this script to attempt to download the latest patch and install it.
#     # You will need to uncomment line 15 in JRPC/etc/$MODE_flag/IoTeX/config-override.yaml
#     curl https://raw.githubusercontent.com/iotexproject/iotex-bootstrap/$latestversion/trie.db.patch > $JRPC/data/$MODE_flag/IoTeX/trie.db.patch

#     echo "${green}Downloading latest data${reset}"
#     curl -L -o $JRPC/temp/$MODE_flag/IoTeX/data.tar.gz https://t.iotex.me/$MODE_flag-data-with-idx-latest 

#     echo "${green}Extracting data to temporary directory${reset}"
#     tar xvf $JRPC/temp/$MODE_flag/IoTeX/data.tar.gz -C $JRPC/temp/$MODE_flag/IoTeX/

#     echo "${green}Moving data files to the destination directory and cleaning up the $JRPC/temp directory${reset}"
#     mv $JRPC/temp/$MODE_flag/IoTeX/data/* $JRPC/data/$MODE_flag/IoTeX
#     rm -r $JRPC/temp/$MODE_flag/IoTeX/data/ ;;

#   false)
#     echo "This script will attempt to use data located in ${green}$JRPC/data/$MODE_flag/IoTeX${reset}"
#     echo "If there is no data in this directory, the blockchain will attempt to synchronize from block height 0. You will need to provide your Infura or Pocket endpoint in the configuration files" ;;
    
#     # If the node does not start synching from block height 0, it might be due to incorrect Pocket Ethereum Mainnet Tracing endpoint."
#     # If you do not have any endpoints to use, you can uncomment the below line to download the static poll.db data files, which will overcome the need to connect to Mainnet Ethereum Tracing nodes.
#     # curl -L https://storage.googleapis.com/blockchain-golden/poll."${MODE_flag,}".tar.gz > $JRPC/temp/$MODE_flag/IoTeX/poll.tar.gz; tar -xzf $JRPC/temp/$MODE_flag/IoTeX/poll.tar.gz -C $JRPC/data/$MODE_flag/IoTeX/ ;;

#   *)
#     echo "${red}Not a valid answer. Terminating${reset}"
#     exit 0 ;;
# esac

# # Start the build scripts for the iotex-core, babel-api and redis servers 

# case $MODE_flag in  
#   Testnet|Mainnet) 
#     echo "${green}Building the $MODE_flag IoTeX server node${reset}" 
#     echo "${green}Starting docker-compose scripts${reset}"
#     echo "running command ${purple} docker-compose -p JRPC-$MODE_flag -f $JRPC/etc/$MODE_flag/Docker/docker-compose-$MODE_flag-$ANALYTICS_flag.yaml up $DEBUG_flag --no-deps --build${reset}"
#     docker-compose -p JRPC-$MODE_flag -f $JRPC/etc/$MODE_flag/Docker/docker-compose-$MODE_flag-$ANALYTICS_flag.yaml up $DEBUG_flag --no-deps --build ;; 
#   *) 
#     echo "${red}Not a valid mode. Please enter in M for Mainnet or T for Testnet${reset}" ;; 
# esac


















