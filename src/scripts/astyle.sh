#!/bin/bash
echo $1 | grep 'values-list.cpp' 2>&1 > /dev/null
if [[ $? -eq 0 ]]; then
	exit 0
fi
astyle -A2 -xb -xf -xL -xW -U -w -t -xn -xc -xl -xk -xV -C -xG -xU -S -K -N -M80 -U -W1 -j -xt1 -Y --suffix=none $*
