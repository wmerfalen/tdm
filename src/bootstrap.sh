#!/bin/bash
DIR=$(pwd)
sudo apt-get update
sudo apt-get -y install cmake libssl-dev libpqxx-dev
mkdir build
rm -rf yaml-cpp
git clone 'https://github.com/jbeder/yaml-cpp'
chmod +x boost-fetcher.sh
./boost-fetcher.sh
cd $DIR
ln -s boost_1_72_0 boost
rm boost_1_72_0.tar.gz
