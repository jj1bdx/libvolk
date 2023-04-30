#!/bin/sh
# clean up build directory 
/bin/rm -rf build
# Update cpu_features
git submodule update
# cmake script for macOS and Ubuntu in amd64/x86_64
cmake -S . -B build -DBUILD_TESTING=OFF -DENABLE_TESTING=OFF -DVOLK_CPU_FEATURES=ON -DVOLK_USE_OWN_CPU_FEATURES=ON
# cmake for Apple Silicon (M1/M2/etc.) macOS
cmake -S . -B build -DBUILD_TESTING=OFF -DENABLE_TESTING=OFF -DVOLK_CPU_FEATURES=OFF
#
# do actual building here
#
MAKEFLAGS="-j12" cmake --build build
# after building, do:
#
# sudo zsh
# umask 022
# cmake --install build
