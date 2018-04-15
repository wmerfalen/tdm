#!/bin/bash
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd $DIR/../
rm tags 2>/dev/null
ctags -R -f tags --links=no --totals=yes \
	--exclude='*.php' --exclude=boost_1_66_0 \
	--exclude=boost-cpp --exclude=mods/lib \
	--exclude=mods/lmdb --exclude=mods/php \
	--exclude='*.py' --exclude=Makefile* \
	--exclude='duktape*' --languages='C++' --languages='C'
