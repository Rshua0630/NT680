#!/bin/sh

MODULES=`lsmod | grep xhci_plat_hcd`
if [ -z "$MODULES" ]; then
	modprobe xhci_plat_hcd
fi

#echo "mount host"
