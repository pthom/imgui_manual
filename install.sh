#git clone https://github.com/jrynkiew/jrpc_beta
#cd jrpc_beta

sudo apt-get install docker
sudo apt-get install docker-compose

git submodule update --init --recursive

cd build

#we need to add [x11] parameter to vcpkg on line 25 like so - run("./vcpkg install glfw3 sdl2[x11]")
cp vcpkg_install_third_parties.py ../external/hello_imgui/tools/

docker-compose -p JRPC-linux-buildsystèmes -f ./build-Linux.yaml up --no-deps --build

cd ../bin/src

./imgui_manual
