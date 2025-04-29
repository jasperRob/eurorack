#!/bin/bash
rm -rf build
mkdir build
cd build
echo -e "\nRunning CMake...\n"
cmake -DPICO_SDK_PATH=/home/jasper/projects/rpi/pico-sdk ..
echo -e "\nRunning Make...\n"
make midi_in
cd ..
