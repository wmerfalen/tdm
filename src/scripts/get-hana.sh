#!/bin/bash
git clone 'https://github.com/boostorg/hana'
cd hana
sudo apt-get update
sudo apt-get -y install cmake
mkdir boost-build
cd boost-build
CXX=$(which g++-7) cmake ../
CXX=$(which g++-7) sudo cmake --build . --target install
