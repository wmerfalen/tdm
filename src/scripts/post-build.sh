#!/bin/bash
function log(){
	echo "$(date -I): '$1'" >> /tmp/cm-post-build-general
}
function clear_log(){
	echo '' > /tmp/cm-post-build-general
}
clear_log
if [[ -f $PWD/../dont-kill-gc ]]; then
	log 'Not clearing due to dkgc'
	echo -e '\e[32m[post-build.sh] not killing gc due to ' $PWD/../dont-kill-gc '\e[0m'
	exit 0
fi

GCOUNT=$(pgrep gdb | wc -l)
if [[ $GCOUNT -gt 0 ]]; then
	killall gdb
fi
pgrep rlwrap | wc -l >> ~/logs/cm-post-build.log
GCOUNT=$(pgrep rlwrap | wc -l)
if [[ $GCOUNT -gt 0 ]]; then
	killall rlwrap
fi
exit 0
