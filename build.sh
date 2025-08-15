#! /bin/bash
mkdir -p build
cd build || exit
if cmake .. && make -j 8; then
  ./tests
fi

