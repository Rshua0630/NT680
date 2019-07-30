modprobe usbcore
modprobe xhci-hcd
modprobe xhci-plat-hcd

x=0
timeout=1000
while [ "$x" -lt "$timeout" -a ! -b /dev/sda ]; do
	x=$((x+1))
	sleep .1
done

if [ -z "`cat /proc/mounts | grep "/mnt/usb"`" ]; then
	mount /dev/sda1 /mnt/usb
	if [ $? != 0 ]; then
		ucmd exam msg EXECUTED_SUSPEND NG
	fi
fi

find /mnt/usb -type f -exec cksum {} \; > /tmp/usb3_before_suspend
ucmd exam msg EXECUTED_SUSPEND OK
