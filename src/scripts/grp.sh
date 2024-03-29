#!/bin/bash
grep -r --exclude-dir=doc \
	--exclude-dir=boost-cpp \
	--exclude-dir=lmdb \
	--exclude-dir=php \
	--exclude-dir=GSL \
	--exclude-dir=duktape-2.2.0\* \
	--exclude-dir=gsl \
	--exclude-dir=web-portal \
	--exclude-dir=libpqxx \
	--exclude-dir=cppzmq \
	--exclude-dir=yaml-cpp \
	--exclude-dir=liblmdb \
	--exclude-dir=catch2 \
	--exclude-dir=todo \
	--exclude=cscope.out \
	--exclude='*.vim' \
	--exclude='todos' \
	--exclude='catch2.*p' \
	--exclude='*.o' \
	--exclude-dir=boost\* "$@"
