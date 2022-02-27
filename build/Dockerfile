from ubuntu:18.04

WORKDIR /imgui_manual/bin

RUN apt-get update
RUN apt-get install build-essential -y
RUN apt-get install autoconf -y
RUN apt-get install libssl-dev -y
RUN apt-get install python -y
RUN apt-get install git -y
RUN apt-get install zip unzip tar curl -y
RUN apt-get install wget -y
RUN apt-get install libx11-dev libxft-dev libxext-dev -y
RUN apt-get install libxinerama-dev libxcursor-dev xorg-dev libglu1-mesa-dev -y
RUN apt-get install libsdl2-dev libsdl2-image-dev -y
RUN wget https://github.com/Kitware/CMake/releases/download/v3.22.2/cmake-3.22.2.tar.gz && tar -zxvf cmake-3.22.2.tar.gz && cd cmake-3.22.2 && ./bootstrap && make && make install