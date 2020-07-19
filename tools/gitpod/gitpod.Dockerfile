FROM gitpod/workspace-full

# Install custom tools, runtimes, etc.
# For example "bastet", a command-line tetris clone:
# RUN brew install bastet
#
# More information: https://www.gitpod.io/docs/config-docker/

# Install emscripten toolchain
RUN cd ~ \
    && git clone https://github.com/emscripten-core/emsdk.git \
    && cd emsdk \
    && git pull \
    && ./emsdk install latest \
    && ./emsdk activate latest
