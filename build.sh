#! /bin/bash
mkdir -p build
cd build
cmake ..
make -j 8
if [ $? -eq 0 ]; then
 ./tests
fi