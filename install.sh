#git clone https://github.com/jrynkiew/jrpc_beta
#cd jrpc_beta

sudo apt-get install docker
sudo apt-get install docker-compose

git submodule update --init --recursive

cd build

cp -R vcpkg ../external/hello_imgui/

#docker build --no-cache -t jrpc-linux-buildsystem .

docker-compose -p JRPC-linux-buildsystèmes -f ./build-Linux.yaml up --no-deps --build

cd ../bin/src

./imgui_manual
