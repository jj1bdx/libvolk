#!/bin/sh
# cmake script for macOS and Ubuntu
cmake -S . -B build -DBUILD_TESTING=OFF -DENABLE_TESTING=OFF -DVOLK_CPU_FEATURES=ON -DVOLK_USE_OWN_CPU_FEATURES=ON
MAKEFLAGS="-j12" cmake --build build
# after building, do:
# sudo zsh
# umask 022
# cmake --install build
