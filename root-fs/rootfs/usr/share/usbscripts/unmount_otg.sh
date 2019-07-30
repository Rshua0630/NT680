#!/bin/sh

MODULES=`lsmod | grep sd_mod`
if [ "$MODULES" ]; then
	rmmod sd_mod
fi
MODULES=`lsmod | grep uas`
if [ "$MODULES" ]; then
	rmmod uas
fi
MODULES=`lsmod | grep usb_storage`
if [ "$MODULES" ]; then
	rmmod usb_storage
fi
MODULES=`lsmod | grep scsi_mod`
if [ "$MODULES" ]; then
	rmmod scsi_mod
fi
MODULES=`lsmod | grep xhci_plat_hcd`
if [ "$MODULES" ]; then
	rmmod xhci_plat_hcd
fi
MODULES=`lsmod | grep xhci_hcd`
if [ "$MODULES" ]; then
	rmmod xhci_hcd
fi
MODULES=`lsmod | grep usbcore`
if [ "$MODULES" ]; then
	rmmod usbcore
fi


#echo "unmount host"
