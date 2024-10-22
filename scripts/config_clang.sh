#!/bin/bash

sudo apt-get update

# Install clang and its tools.
sudo apt-get -y install clang clangd clang-format clang-tidy clang-tools lld

# Install cmake just in case.
sudo apt-get -y install cmake

# GLFW dependencies.
sudo apt-get install -y libxinerama-dev libxcursor-dev libxi-dev libxrandr-dev libgl1-mesa-dev

echo "Installation complete."