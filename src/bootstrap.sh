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

cd $DIR/mods
if [[ ! -d $DIR/mods/catch2 ]]; then
	mkdir $DIR/mods/catch2
fi

echo "# status: grabbing Catch2..."
cd $DIR/mods/catch2
git clone 'https://github.com/catchorg/Catch2'
cd Catch2
mkdir build
cd build
cmake ..
make
cd $DIR
echo "# done fetching Catch2"

echo "#############################################################"
echo " VERY IMPORTANT!!!"
echo " VERY IMPORTANT!!!"
echo "#############################################################"
echo "# Take the following steps to configure your postgres user! #"
echo "1) login to your postgres account and run this command: "
echo "ALTER USER postgres WITH PASSWORD 'yourpasswordhere'"
echo "2) create the directory cmpp/data"
echo "3) vim cmpp/data/postgres-pw.txt"
echo "4) type 'yourpasswordhere' without the quotes"
echo "5) type :wq to exit vim"
echo "6) chmod 400 cmpp/data/postgres-pw.txt"

