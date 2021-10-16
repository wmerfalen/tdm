#!/bin/bash

cd $WEBROOT
find mods -type f -name '*.hpp' | grep -v 'mods/catch2' | grep -v 'mods/lmdb' >> ./cscope.files 
find mods -type f -name '*.cpp' | grep -v 'mods/catch2' | grep -v 'mods/lmdb' >> ./cscope.files 
ls *.h *.cpp *.hpp >> ./cscope.files 
cscope -Rb

echo "cscope.out and cscope.files are in $WEBROOT"
