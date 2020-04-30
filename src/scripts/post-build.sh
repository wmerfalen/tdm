#!/bin/bash
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
