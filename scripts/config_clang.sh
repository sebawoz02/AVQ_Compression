#!/bin/bash

sudo apt-get update

# Install clang and its tools.
sudo apt-get -y install clang clangd clang-format clang-tidy clang-tools lld

# Install cmake just in case.
sudo apt-get -y install cmake

echo "Installation complete."