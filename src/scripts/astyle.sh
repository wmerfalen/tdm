#!/bin/bash
astyle -A2 -t -xn -xc -K -xG -S -f -M80 -N -C -U -W1 -j $@
