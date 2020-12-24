#!/bin/bash
cd $WEBROOT
cd ../lib/objects

LATEST=$(grep -R vnum | cut -d ':' -f 3 | cut -d '#' -f 1 | sort -n | uniq | grep -v 1000 | tail -n 1)
echo $(( $LATEST + 1 ))
