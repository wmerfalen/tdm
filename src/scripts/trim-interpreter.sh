#!/bin/bash
cp interpreter.cpp interpreter.cpp.backup

echo -e 'cat interpreter.cpp | ' > m
 cat $1 | grep add_command | cut -d, -f 3 | sed -e's|^|grep -v |' -e 's/$/ |/' >> m
 echo ' tee interpreter.changed' >> m
chmod +x m
./m
mv interpreter.changed interpreter.cpp

