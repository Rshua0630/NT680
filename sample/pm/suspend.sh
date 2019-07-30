#!/bin/sh
find /bin /sbin /lib /usr -type f -exec cksum {} \; > /tmp/before_suspend
NUM_CUR=`awk 'END{print FNR}' /tmp/before_suspend`
if [ -f /tmp/after_resume ]; then
	NUM_PREV=`awk 'END{print FNR}' /tmp/after_resume`
	if [ $NUM_CUR != $NUM_PREV ]; then
		echo "File line can't match with previous one."
		ucmd exam msg EXECUTED_SUSPEND NG
		exit 0;
	else
		echo "File line can match with previous one."
	fi
fi
cat /proc/meminfo
ucmd exam msg EXECUTED_SUSPEND OK
