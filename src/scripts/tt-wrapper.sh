#!/bin/bash

TD_CLASS="$1"

while [[ 1 ]]; do
	cd $WEBROOT/scripts/tintin-scripts
	rlwrap -a -H $PWD/histories/${TD_CLASS} ~/bin/tt++ -r ./${TD_CLASS}.tt -e 'start'
  sleep 1
done

