#!/bin/bash
astyle -A2 -t -xn -xc -xl -xk -xV -C -xG -S -K -N -M80 -U -W1 -j $@
