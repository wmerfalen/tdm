#!/bin/bash
#DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
#cd $DIR/../
rm $WEBROOT/tags 2>/dev/null
echo "scripts/ctags creation $(date) webroot: $WEBROOT" >> ~/logs/ctags.log
cd $WEBROOT
ls *.cpp *.h *.hpp mods/weapons/*.*p mods/orm/*.*p mods/*/*.*p mods/*.*p | grep -v ':' | grep -v 'mods/lmdb/' 2>/dev/null > cscope.files
ctags -R -f $WEBROOT/tags  --links=no --totals=yes \
	--exclude='*.php' --exclude=boost_1_66_0 \
	--exclude=/usr/include \
	--exclude='mods/acl/*' \
	--exclude='mods/item-types.hpp' \
	--exclude=mods/catch.hpp \
	--exclude=mods/catch2.hpp \
	--exclude=mods/lmdb \
	--exclude=telnet.h \
	--exclude=liblmdb \
	--exclude=sqlightning \
	--exclude=$WEBROOT/util/ \
	--exclude=boost-cpp --exclude=mods/lib \
	--exclude=mods/lmdb --exclude=mods/php \
	--exclude='*.py' --exclude=Makefile* \
	--exclude='duktape*' \
	--totals=no \
	--tag-relative=yes \
	--if0=no \
	--fields=+a+f+i+K+n+s+S+z+t \
	--C++-kinds=+f+c+e-g-l+m-u+v \
	-h *.c *.cpp *.h *.hpp mods/*.hpp mods/*.cpp mods/*/*.*p &
cscope -R -b -i cscope.files &


#C++
#    c  classes
#    d  macro definitions
#    e  enumerators (values inside an enumeration)
#    f  function definitions
#    g  enumeration names
#    l  local variables [off]
#    m  class, struct, and union members
#    n  namespaces
#    p  function prototypes [off]
#    s  structure names
#    t  typedefs
#    u  union names
#    v  variable definitions
#    x  external and forward variable declarations [off]

cd $WEBROOT 
grep -R 'ACMD(do_' . | grep -vE '^./doc/' | grep -vE '^tags:' | grep -v 'scripts/vk-data' | grep '{' | sed -E 's|.cpp:[0-9]+:||g' | sed -E 's|ACMD\(do_|->|g' | sed -E 's|\).*\{||g' > $WEBROOT/../lib/ACMD.list &

cd $WEBROOT
grep -R 'SUPERCMD(do_' . | grep -vE '^./doc/' | grep -vE '^tags:' | grep -v 'scripts/vk-data' | grep '{' | sed -E 's|.cpp:[0-9]+:||g' | sed  -E 's|SUPERCMD\(do_|->|g' | sed -E 's|^\./||g' | sed -E 's|\).*\{||g' | sed -E 's|\.cpp:\s+|.cpp:|g' | sed -E 's|^|{yel}|' | sed -E 's|\.cpp:->|{/yel}:->{grn}|g' | sed -E 's|$|{/grn}|g'  > $WEBROOT/../lib/SUPERCMD.list &
