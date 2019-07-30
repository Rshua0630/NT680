memtester 5M 1
ret=$?
if [ "$ret" != "0" ]; then
	ucmd exam msg EXECUTED_LINUX NG
else
	ucmd exam msg EXECUTED_LINUX OK
fi
