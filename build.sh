#!/bin/bash

cmake -DCMAKE_BUILD_TYPE=Debug CMakeLists.txt --clean-first --build .
make clean
make VERBOSE=1
