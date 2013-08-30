#!/bin/bash

cmake CMakeLists.txt --clean-first --build .
make clean
make
