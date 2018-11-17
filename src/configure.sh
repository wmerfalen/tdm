sudo apt-get install libscrypt-dev 
git clone 'https://github.com/LMDB/sqlightning'
cd sqlightning
rm -rf build 2>/dev/null
mkdir build
cd build
../configure --disable-tcl --with-gnu-ld --enable-static
make
sudo make install
