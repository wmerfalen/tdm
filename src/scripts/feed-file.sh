#!/bin/bash

FILE=$(echo $@ | cut -d'/' -f 8)
if [[ $# -eq 2 ]]; then
	FILE=$(echo $1 | cut -d'/' -f $2)
fi

echo feed_file: "'${FILE}'"
