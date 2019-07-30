#!/bin/sh

modprobe cfg80211

MODULES=`lsmod | grep bcmdhd`

if [ -z "$MODULES" ]; then
	insmod /lib/modules/4.1.0/extra/net/bcmdhd.1.363.59.144.x.cn.nvt/bcmdhd.ko dhd_slpauto=0 firmware_path=/lib/firmware/ap6255/fw_bcm43455c0_ag_apsta.bin nvram_path=/lib/firmware/ap6255/nvram_ap6255.txt config_path=/lib/firmware/ap6255/config.txt
else
	echo "bcmdhd already exist"
fi

TRY=0
while [ 1 ]
do
	WLAN0=`ifconfig -a | grep wlan0`
	echo "WLAN0=$WLAN0"

	if [ -z "$WLAN0" ]; then
		echo "TRY=$TRY"
		if [ $TRY -gt 5 ]; then
			rmmod bcmdhd
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
