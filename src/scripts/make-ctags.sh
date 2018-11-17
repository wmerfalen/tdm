#!/bin/bash
#DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
#cd $DIR/../
#rm tags 2>/dev/null
cd /home/fud/code/siege-mud/src
ctags -R -f /home/fud/code/siege-mud/src/tags  --links=no --totals=yes \
	--exclude='*.php' --exclude=boost_1_66_0 \
	--exclude=liblmdb \
	--exclude=sqlightning \
	--exclude=/home/fud/code/siege-mud/src/util/ \
	--exclude=boost-cpp --exclude=mods/lib \
	--exclude=mods/lmdb --exclude=mods/php \
	--exclude='*.py' --exclude=Makefile* \
	--exclude='duktape*' \
	-h '.h.hpp' *.cpp *.h *.hpp mods/*.hpp mods/*.cpp
