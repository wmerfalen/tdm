#!/bin/bash
#DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
#cd $DIR/../
rm $WEBROOT/tags 2>/dev/null
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
	--totals=no \
	-h '.h.hpp' *.cpp *.h *.hpp mods/*.hpp mods/*.cpp mods/*/*.*p &
cscope -R -b -i cscope.files &

cd $WEBROOT 
grep -R 'ACMD(do_' . | grep -vE '^./doc/' | grep -vE '^tags:' | grep -v 'scripts/vk-data' | grep '{' | sed -E 's|.cpp:[0-9]+:||g' | sed -E 's|ACMD\(do_|->|g' | sed -E 's|\).*\{||g' > $WEBROOT/../lib/ACMD.list &

cd $WEBROOT
grep -R 'SUPERCMD(do_' . | grep -vE '^./doc/' | grep -vE '^tags:' | grep -v 'scripts/vk-data' | grep '{' | sed -E 's|.cpp:[0-9]+:||g' | sed  -E 's|SUPERCMD\(do_|->|g' | sed -E 's|^\./||g' | sed -E 's|\).*\{||g' | sed -E 's|\.cpp:\s+|.cpp:|g' | sed -E 's|^|{yel}|' | sed -E 's|\.cpp:->|{/yel}:->{grn}|g' | sed -E 's|$|{/grn}|g'  > $WEBROOT/../lib/SUPERCMD.list &
