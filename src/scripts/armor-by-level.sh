#!/bin/bash
cd $MUD_ROOT
cd ../lib/objects/armor

for file in $(grep -R 'rarity: ' | grep $1 | cut -d':' -f 1 | uniq | sort); do
	echo "\"armor/$file\","
done
