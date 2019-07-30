#!/bin/sh
MODULES=`lsmod | grep g_mass_storage`
if [ "$MODULES" ]; then
	rmmod g_mass_storage.ko
fi
MODULES=`lsmod | grep usb_f_mass_storage`
if [ "$MODULES" ]; then
	rmmod usb_f_mass_storage.ko
fi
MODULES=`lsmod | grep libcomposite`
if [ "$MODULES" ]; then
	rmmod libcomposite.ko
fi
MODULES=`lsmod | grep configfs`
if [ "$MODULES" ]; then
	rmmod configfs.ko
fi
MODULES=`lsmod | grep dwc3`
if [ "$MODULES" ]; then
	rmmod dwc3.ko
fi

MOUNT_OPTIONS=`cat /proc/mounts | grep "/mnt/sd" | awk -F' ' '{NUM=NF-2; print $NUM}'`
#echo $MOUNT_OPTIONS
mount -o remount,$MOUNT_OPTIONS /mnt/sd

#echo "unmount MSDC"