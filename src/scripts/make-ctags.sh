#!/bin/bash
rm tags
ctags -R --exclude='*.php' --exclude=boost_1_66_0 --exclude=boost-cpp --exclude=mods/lib --exclude=mods/lmdb --exclude=mods/php --exclude='*.py' --exclude=Makefile* --exclude='duktape*' --languages='c++'
