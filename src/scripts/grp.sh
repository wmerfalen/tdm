#!/bin/bash
grep -r --exclude-dir=lmdb --exclude-dir=php --exclude-dir=GSL --exclude-dir=duktape-2.2.0\* --exclude-dir=gsl --exclude-dir=boost\* "$@"
