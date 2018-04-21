#!/bin/bash
cd test-suites
for i in `ls -I README`; do
	telnet localhost 4000 < $i
done

cd ../
cd concurrent-test-suites
for i in `cat FILES`; do
	cat "$i"-a - | nc localhost 4000 &
	sleep 1
	telnet localhost 4000 < "$i"-b
	killall nc
done

