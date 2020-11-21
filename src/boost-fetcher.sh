#!/bin/bash

DEFAULT_FETCHER=wget
if [[ ! -z "$MENTOC_HTTP_FETCHER" ]]; then
	$MENTOC_HTTP_FETCHER 'https://dl.bintray.com/boostorg/release/1.72.0/source/boost_1_72_0.tar.gz'
else
	$DEFAULT_FETCHER 'https://dl.bintray.com/boostorg/release/1.72.0/source/boost_1_72_0.tar.gz'
fi

chmod -w ./boost_1_72_0.tar.gz
tar -xvzf ./boost_1_72_0.tar.gz
cd boost_1_72_0/ && ./bootstrap.sh
./b2

