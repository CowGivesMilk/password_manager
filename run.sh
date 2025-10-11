#! /bin/bash
mkdir -p build
cd build || exit
if cmake .. && make -j 8; then
  if ./tests; then
    ./passman
  fi
fi