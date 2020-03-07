#!/bin/bash
#DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
#cd $DIR/../
#rm tags 2>/dev/null
echo "scripts/ctags creation $(date)" >> ~/logs/ctags.log
cd $WEBROOT
ls *.cpp *.h *.hpp mods/*/*.*p mods/*.*p | grep -v ':' | grep -v 'mods/lmdb/' 2>/dev/null > cscope.files
ctags -R -f $WEBROOT/tags  --links=no --totals=yes \
	--exclude='*.php' --exclude=boost_1_66_0 \
	--exclude=liblmdb \
	--exclude=sqlightning \
	--exclude=$HOME/code/siege-mud/src/util/ \
	--exclude=boost-cpp --exclude=mods/lib \
	--exclude=mods/lmdb --exclude=mods/php \
	--exclude='*.py' --exclude=Makefile* \
	--exclude='duktape*' \
	-h '.h.hpp' *.cpp *.h *.hpp mods/*.hpp mods/*.cpp &
cscope -R -b -i cscope.files &
