#!/bin/bash

make clean
rm CMakeCache.txt
rm -rf CMakeFiles/
rm cmake_install.cmake
rm Makefile
rm build.sh
rm clean
rm -rf docs/
rm readme
rm test_certo.sh
rm test_errado.sh
rm test.sh

tar -czf ~/Desktop/compilador.tar.gz *

git checkout -- .
