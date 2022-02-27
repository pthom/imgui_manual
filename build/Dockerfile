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
RUN wget https://github.com/Kitware/CMake/releases/download/v3.20.0/cmake-3.20.0.tar.gz && tar -zxvf cmake-3.20.0.tar.gz && cd cmake-3.20.0 && ./bootstrap && make && make install 
RUN apt-get install libsdl2-dev libsdl2-image-dev -y