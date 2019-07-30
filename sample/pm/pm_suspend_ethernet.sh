#!/bin/sh
count=2
ping -c $count $1 > /tmp/pm_eth.log
ping_ok=`cat "/tmp/pm_eth.log"| grep "$count packets received"`
if [ ! -z "$ping_ok" ]; then
	echo $1 "==>PM ping PASS"
	ucmd exam msg EXECUTED_SUSPEND OK
else
	echo $1 "==>PM ping FAIL"
	ucmd exam msg EXECUTED_SUSPEND NG
fi
