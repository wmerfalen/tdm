#!/bin/bash
BINARY=/home/xkold/cmpp/bin/circle
if [[ "$1" == "strip-all" ]]; then
	B=$(du -sh $BINARY | awk '{print $1}')
	strip --strip-all $BINARY 2>&1 > /dev/null
	A=$(du -sh $BINARY | awk '{print $1}')
	echo 
	echo -n "Before: $B"
	echo
	echo -n "After $A"
	echo
fi
if [[ -f $PWD/../dont-kill-gc ]]; then
	echo 'not killing gc due to ../dont-kill-gc file existence'
	exit 0
fi
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
VPID=$(ps aux | grep /usr/bin/valgrind.bin | head -n 1 | awk '{print $2}')
if [[ ! -z $VPID ]]; then
	kill -9 $VPID
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
killall circle
exit 0
