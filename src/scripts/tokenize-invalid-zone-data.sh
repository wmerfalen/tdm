#!/bin/bash
grep 'skipping command' ./debug.log  | cut -d'(' -f 2 | cut -d')' -f 1 | sort -u
