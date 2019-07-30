#! /bin/sh
if [ "$1" == "" ]; then
echo "parameter is none" > /tmp/error.txt
exit 1
fi
mounted=`mount | grep $1 | wc -l`

# mounted, assume we umount
if [ $mounted -ge 1 ]; then
echo "R/media/$1" >> /tmp/usbmnt.log
echo "R/media/$1" > /tmp/fifo.1
if ! umount "/media/$1"; then
exit 1
fi

if ! rmdir "/media/$1"; then
exit 1
fi
# not mounted, lets mount under /media
else
if ! mkdir -p "/media/$1"; then
exit 1
fi

if ! mount "/dev/$1" "/media/$1"; then
# failed to mount, clean up mountpoint
if ! rmdir "/media/$1"; then
exit 1
fi
exit 1
fi
echo "A/media/$1" >> /tmp/usbmnt.log
echo "A/media/$1" > /tmp/fifo.1
fi

exit 0
