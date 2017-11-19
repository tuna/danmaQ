#!/bin/bash
mkdir build-release/
cd build-release/
cmake .. -DCMAKE_BUILD_TYPE=Release
make
cd ..

mkdir build-debug/
cd build-debug/
cmake .. -DCMAKE_BUILD_TYPE=Debug
make
cd ..
