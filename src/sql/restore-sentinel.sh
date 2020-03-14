#!/bin/bash
if [[ -z "$1" ]]; then
	echo 'Usage: restore-sentinel.sh <users-primary-key-id> ... <N>'
	exit 1
fi

echo '' > restore.sql
echo '' > ./.restore.tmp

for i in $@; do
	echo -en "#define MENTOC_PLAYER_ID $i\n$(cat restore-sentinel-weapon.sql)" > ./.restore.tmp
	cpp ./.restore.tmp -E | grep -v '^#' >> restore.sql
	rm ./.restore.tmp
done
echo "Your sql file is at: $PWD/restore.sql"

