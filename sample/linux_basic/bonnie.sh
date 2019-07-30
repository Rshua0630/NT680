parse_str=`cat /proc/mounts  | grep "\/mnt\/sd"`
if [ -z "$parse_str" ]; then
	ucmd exam msg EXECUTED_LINUX NG
else
	bonnie\+\+ -d /mnt/sd -s 200 -u 0 -r 10
	ret=$?
	if [ "$ret" != "0" ]; then
		ucmd exam msg EXECUTED_LINUX NG
	else
		ucmd exam msg EXECUTED_LINUX OK
	fi
fi
