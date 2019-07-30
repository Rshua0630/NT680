find /bin /sbin /lib /usr -type f -exec cksum {} \; > /tmp/after_resume
NUM_CUR=`awk 'END{print FNR}' /tmp/after_resume`
if [ -f /tmp/before_suspend ]; then
	NUM_PREV=`awk 'END{print FNR}' /tmp/before_suspend`
	if [ $NUM_CUR != $NUM_PREV ]; then
		echo "File line can't match with previous one."
		ucmd exam msg EXECUTED_RESUME NG
		exit 0;
	else
		echo "File line can match with previous one."
	fi
fi
ret="`diff /tmp/before_suspend /tmp/after_resume`"
if [ ! -z "$ret" ]; then
	echo "Suspend/resume file system is not match"
	echo $ret;
	ucmd exam msg EXECUTED_RESUME NG
else
	echo "Successfully"
	ucmd exam msg EXECUTED_RESUME OK
fi
