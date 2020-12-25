#!/bin/bash
#DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
#cd $DIR/../
#rm tags 2>/dev/null
echo "scripts/ctags creation $(date) webroot: $WEBROOT" >> ~/logs/ctags.log
cd $WEBROOT
ls *.cpp *.h *.hpp mods/weapons/*.*p mods/orm/*.*p mods/*/*.*p mods/*.*p | grep -v ':' | grep -v 'mods/lmdb/' 2>/dev/null > cscope.files
ctags -R -f $WEBROOT/tags  --links=no --totals=yes \
	--exclude='*.php' --exclude=boost_1_66_0 \
	--exclude=liblmdb \
	--exclude=sqlightning \
	--exclude=$WEBROOT/util/ \
	--exclude=boost-cpp --exclude=mods/lib \
	--exclude=mods/lmdb --exclude=mods/php \
	--exclude='*.py' --exclude=Makefile* \
	--exclude='duktape*' \
	-h '.h.hpp' *.cpp *.h *.hpp mods/*.hpp mods/*.cpp mods/*/*.*p &
cscope -R -b -i cscope.files &
