find /mnt/usb -type f -exec cksum {} \; > /tmp/usb3_after_resume
ret="`diff /tmp/usb3_before_suspend /tmp/usb3_after_resume`"
if [ ! -z "$ret" ]; then
	echo "Suspend/resume file system is not match"
	echo $ret;
	ucmd exam msg EXECUTED_RESUME NG
else
	echo "Successfully"
	ucmd exam msg EXECUTED_RESUME OK
fi
