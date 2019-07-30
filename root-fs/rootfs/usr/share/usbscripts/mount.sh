#!/bin/sh

MODULES=`lsmod | grep dwc3`
if [ -z "$MODULES" ]; then
	insmod /lib/modules/4.1.0/kernel/drivers/usb/dwc3/dwc3.ko
fi
MODULES=`lsmod | grep configfs`
if [ -z "$MODULES" ]; then
	insmod /lib/modules/4.1.0/kernel/fs/configfs/configfs.ko
fi
MODULES=`lsmod | grep libcomposite`
if [ -z "$MODULES" ]; then
	insmod /lib/modules/4.1.0/kernel/drivers/usb/gadget/libcomposite.ko
fi
MODULES=`lsmod | grep usb_f_mass_storage`
if [ -z "$MODULES" ]; then
	insmod /lib/modules/4.1.0/kernel/drivers/usb/gadget/function/usb_f_mass_storage.ko
fi
MODULES=`lsmod | grep g_mass_storage`
BLOCKDEV="mmcblk0"
SDDEV=""

if [ -b "/dev/$BLOCKDEV" ]; then
	if [ -b "/dev/${BLOCKDEV}p1" ]; then
		SDDEV="/dev/${BLOCKDEV}p1"
	else
		SDDEV="/dev/${BLOCKDEV}"
	fi
fi

if [ -z "$MODULES" ] && [ "$SDDEV" != "" ]; then
	insmod /lib/modules/4.1.0/kernel/drivers/usb/gadget/legacy/g_mass_storage.ko luns=$1 idVendor=$2 idProduct=$3 iManufacturer=$4 iProduct=$5 iSerialNumber=$6 file=$SDDEV nofua=1 stall=0 removable=1
else
	echo "Cannot find SD card"
fi

#echo "mount MSDC"