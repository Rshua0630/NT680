#!/bin/sh

#cd /lib/modules/4.1.0/kernel/drivers/usb/host/
#modprobe ehci-hcd

modprobe cfg80211

MODULES=`lsmod | grep nvtwifi`

if [ -z "$MODULES" ]; then
	insmod /lib/modules/4.1.0/extra/net/18211_nvtfmac/nvtwifi.ko
else
	echo "nvtwifi already exist"
fi

TRY=0
while [ 1 ]
do
	WLAN0=`ifconfig -a | grep wlan0`
	echo "WLAN0=$WLAN0"

	if [ -z "$WLAN0" ]; then
		echo "TRY=$TRY"
		if [ $TRY -gt 5 ]; then
			rmmod nvtwifi.ko
			exit 255
		else
			TRY=$((${TRY}+1))
			sleep 1
		fi
	else
		echo "$WLAN0 found"
		exit 0
	fi
done
