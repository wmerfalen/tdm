#!/bin/bash

FILE=$(echo $@ | cut -d'/' -f 8)

echo feed_file: "'${FILE}'"
