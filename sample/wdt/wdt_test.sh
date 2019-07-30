#!/bin/sh
#NVT WDT Test version 1.0
if [ ! -n "$1" ]; then
	echo "Please enter the reboot time you'd like to test"
	echo "Format: wdt_test.sh [time in s]"
	echo "Range 1 ~ 89 s"
	echo " "
	echo "Or you could use parameter a to kick watchdog repeatedly"
	echo "Format: wdt_test.sh a &"
	echo " "
elif [ "$1" == "a" ]; then
	while [ 1 ]
		do
			echo a > /dev/watchdog
			sleep 1
		done
else
	watchdog -T $1 /dev/watchdog
	echo "Time count:"
	while [ 1 ]
		do
			date
			sleep 1
		done
fi
