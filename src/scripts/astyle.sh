#!/bin/bash
astyle -A2 -xb -xf -xL -xW -U -w -t -xn -xc -xl -xk -xV -C -xG -xU -S -K -N -M80 -U -W1 -j -xt1 -Y --suffix=none $*
