if [ $1 == 0 ]; then
	count=1;
else
	count=$1;
fi
x=0
while [ "$x" -lt "$count" ]; do
	x=$((x+1))
	bonnie\+\+ -d /var/ -u 0 -s 100
	ret=$?
	if [ "$ret" != "0" ]; then
		ucmd exam msg EXECUTED_LINUX NG
	fi

	dd if=/dev/urandom of=/var/image.bin bs=1M count=10
	ret=$?
	if [ "$ret" != "0" ]; then
		ucmd exam msg EXECUTED_LINUX NG
	fi
done

ucmd exam msg EXECUTED_LINUX OK
