#!/bin/sh
 
MNT_PATH=/mnt

MNT_DIR=
SD1_DIR=sd
SD2_DIR=sd2
SD3_DIR=sd3
EMMC1_DIR=emmc1
EMMC2_DIR=emmc2
USB1_DIR=usb
USB2_DIR=usb2
PST_DIR=pstore
DRIVE_A=""
DRIVE_B=""

UGXSTRG_STAT_MOUNTED="ready"
UGXSTRG_STAT_NODEV="nodev"
UGXSTRG_STAT_FS_ERR="unfmt"

my_umount()
{
	FOLDER=`grep "/dev/$1" /proc/mounts | cut -d ' ' -f 2`
	if [ ! -z "$FOLDER" ]; then
		#if unmount ok, send notify to uITRON
		if [ $FOLDER == ${MNT_PATH}/${SD1_DIR} ]; then
			ugxstrg_sent ${UGXSTRG_STAT_NODEV} ${SD1_DIR}
		elif [ $FOLDER == ${MNT_PATH}/${SD2_DIR} ]; then
			ugxstrg_sent ${UGXSTRG_STAT_NODEV} ${SD2_DIR}
		elif [ $FOLDER == ${MNT_PATH}/${USB1_DIR} ]; then
			ugxstrg_sent ${UGXSTRG_STAT_NODEV} ${USB1_DIR}
		elif [ $FOLDER == ${MNT_PATH}/${USB2_DIR} ]; then
			ugxstrg_sent ${UGXSTRG_STAT_NODEV} ${USB2_DIR}
		else
			ugxstrg_sent ${UGXSTRG_STAT_NODEV} ${SD3_DIR}
		fi

		umount -l "$FOLDER";
	fi
}
 
my_mount()
{
	BUSNUM=`echo $DEVPATH | cut -d '/' -f 4`
	if [ "$BUSNUM" == "nt96660_mmc.0" ] || [ "$BUSNUM" == "f0420000.mmc" ]; then
		MNT_DIR=$SD1_DIR
	elif [ "$BUSNUM" == "nt96660_mmc.1" ] || [ "$BUSNUM" == "f0500000.mmc" ]; then
		MNT_DIR=$SD2_DIR
	elif [ "$BUSNUM" == "f1700000.nvt_usb3host" ]; then
		MNT_DIR=$USB1_DIR
	elif [ "$BUSNUM" == "f0600000.nvt_usb2host" ]; then
		MNT_DIR=$USB2_DIR
	else
		MNT_DIR=$SD3_DIR
	fi

	if [ -b /dev/$1 ]; then
		MOUNTDEV="/dev/$1"
		if [ -b "/dev/$1p1" ]; then
			MOUNTDEV="/dev/$1p1"
		elif [ -b "/dev/${1}1" ]; then
			MOUNTDEV="/dev/${1}1"
		fi
	fi

	time_offset_sig=`date +%z | cut -c 1`
	time_offset_h=`date +%z | cut -c 2-3`
	time_offset_m=`date +%z | cut -c 4-5`
	time_offset=`expr $local_time - $utc_time`
	if [ $time_offset_sig == + ]; then
		time_offset_sig="";
	fi
	time_offset_total_m=$time_offset_sig`expr $time_offset_h \* 60 + $time_offset_m`
	uctrl usetup -timeoffset $time_offset_h:$time_offset_m

	mkdir -p "${MNT_PATH}/${MNT_DIR}" || exit 1
	fs_type=`blkid "$MOUNTDEV" | awk -F'TYPE=' '{print $NF}'`
	if [ "${fs_type}" == "\"vfat\"" ]; then
		if ! mount -o dirsync,time_offset=$time_offset_total_m "$MOUNTDEV" "${MNT_PATH}/${MNT_DIR}" 2>&1 | tee -a /tmp/mountstat; then
			echo "$MOUNTDEV $MNT_PATH/$MNT_DIR ignore defaults 0 0" >> /tmp/.nvt_mounts
			ugxstrg_sent ${UGXSTRG_STAT_FS_ERR} $MNT_DIR;
			exit 1
		fi
	elif [ "${fs_type}" == "\"ntfs\"" ]; then
		if ! mount -t ntfs "$MOUNTDEV" "${MNT_PATH}/${MNT_DIR}" 2>&1 | tee -a /tmp/mountstat; then
			echo "$MOUNTDEV $MNT_PATH/$MNT_DIR ignore defaults 0 0" >> /tmp/.nvt_mounts
			ugxstrg_sent ${UGXSTRG_STAT_FS_ERR} $MNT_DIR;
			continue
		fi
	elif [ "${fs_type}" == "\"exfat\"" ]; then
		if ! mount -t exfat "$MOUNTDEV" "${MNT_PATH}/${MNT_DIR}" 2>&1 | tee -a /tmp/mountstat; then
			echo "$MOUNTDEV $MNT_PATH/$MNT_DIR ignore defaults 0 0" >> /tmp/.nvt_mounts
			ugxstrg_sent ${UGXSTRG_STAT_FS_ERR} $MNT_DIR;
			continue
		fi
	else
		echo "$MOUNTDEV FAIL" >> /tmp/.nvt_mounts
		ugxstrg_sent ${UGXSTRG_STAT_FS_ERR} $MNT_DIR;
		exit 1
	fi

	#if mount ok, send notify to uITRON
	ugxstrg_sent ${UGXSTRG_STAT_MOUNTED} $MNT_DIR
}

ugxstrg_init()
{
	source /etc/profile_prjcfg;
	if [ "$NVT_ITRON_DRIVE_A" == "NVT_ITRON_DRIVE_EMMC_1" ]; then
		DRIVE_A=${EMMC1_DIR};
	elif [ "$NVT_ITRON_DRIVE_A" == "NVT_ITRON_DRIVE_EMMC_2" ]; then
		DRIVE_A=${EMMC2_DIR};
	elif [ "$NVT_ITRON_DRIVE_A" == "NVT_ITRON_DRIVE_SD_1" ]; then
		DRIVE_A=${SD1_DIR};
	elif [ "$NVT_ITRON_DRIVE_A" == "NVT_ITRON_DRIVE_SD_2" ]; then
		DRIVE_A=${SD2_DIR};
	elif [ "$NVT_ITRON_DRIVE_A" == "NVT_ITRON_DRIVE_USB_1" ]; then
		DRIVE_A=${USB1_DIR};
	elif [ "$NVT_ITRON_DRIVE_A" == "NVT_ITRON_DRIVE_USB_2" ]; then
		DRIVE_A=${USB2_DIR};
	else
		DRIVE_A="";
	fi

	if [ "$NVT_ITRON_DRIVE_B" == "NVT_ITRON_DRIVE_EMMC_1" ]; then
		DRIVE_B=${EMMC1_DIR};
	elif [ "$NVT_ITRON_DRIVE_B" == "NVT_ITRON_DRIVE_EMMC_2" ]; then
		DRIVE_B=${EMMC2_DIR};
	elif [ "$NVT_ITRON_DRIVE_B" == "NVT_ITRON_DRIVE_SD_1" ]; then
		DRIVE_B=${SD1_DIR};
	elif [ "$NVT_ITRON_DRIVE_B" == "NVT_ITRON_DRIVE_SD_2" ]; then
		DRIVE_B=${SD2_DIR};
	elif [ "$NVT_ITRON_DRIVE_B" == "NVT_ITRON_DRIVE_USB_1" ]; then
		DRIVE_B=${USB1_DIR};
	elif [ "$NVT_ITRON_DRIVE_B" == "NVT_ITRON_DRIVE_USB_2" ]; then
		DRIVE_B=${USB2_DIR};
	else
		DRIVE_B="";
	fi

	if [ "$NVT_ITRON_DRIVE_C" == "NVT_ITRON_DRIVE_EMMC_1" ]; then
		DRIVE_C=${EMMC1_DIR};
	elif [ "$NVT_ITRON_DRIVE_C" == "NVT_ITRON_DRIVE_EMMC_2" ]; then
		DRIVE_C=${EMMC2_DIR};
	elif [ "$NVT_ITRON_DRIVE_C" == "NVT_ITRON_DRIVE_SD_1" ]; then
		DRIVE_C=${SD1_DIR};
	elif [ "$NVT_ITRON_DRIVE_C" == "NVT_ITRON_DRIVE_SD_2" ]; then
		DRIVE_C=${SD2_DIR};
	elif [ "$NVT_ITRON_DRIVE_C" == "NVT_ITRON_DRIVE_USB_1" ]; then
		DRIVE_C=${USB1_DIR};
	elif [ "$NVT_ITRON_DRIVE_C" == "NVT_ITRON_DRIVE_USB_2" ]; then
		DRIVE_C=${USB2_DIR};
	else
		DRIVE_C="";
	fi

	if [ "$NVT_ITRON_DRIVE_D" == "NVT_ITRON_DRIVE_EMMC_1" ]; then
		DRIVE_D=${EMMC1_DIR};
	elif [ "$NVT_ITRON_DRIVE_D" == "NVT_ITRON_DRIVE_EMMC_2" ]; then
		DRIVE_D=${EMMC2_DIR};
	elif [ "$NVT_ITRON_DRIVE_D" == "NVT_ITRON_DRIVE_SD_1" ]; then
		DRIVE_D=${SD1_DIR};
	elif [ "$NVT_ITRON_DRIVE_D" == "NVT_ITRON_DRIVE_SD_2" ]; then
		DRIVE_D=${SD2_DIR};
	elif [ "$NVT_ITRON_DRIVE_D" == "NVT_ITRON_DRIVE_USB_1" ]; then
		DRIVE_D=${USB1_DIR};
	elif [ "$NVT_ITRON_DRIVE_D" == "NVT_ITRON_DRIVE_USB_2" ]; then
		DRIVE_D=${USB2_DIR};
	else
		DRIVE_D="";
	fi
}

ugxstrg_check_drive_is_mounted()
{
	is_mounted=`cat /proc/mounts | grep "/mnt/$DRIVE_A"`
	is_unformated=`if [ -f /tmp/.nvt_mounts ]; then cat /tmp/.nvt_mounts | grep "/mnt/$DRIVE_A"; fi`
	if [ -z "$is_mounted" ] && [ -z "$is_unformated" ] && [ ! -z $DRIVE_A ]; then
		ugxstrg_sent ${UGXSTRG_STAT_NODEV} $DRIVE_A;
	fi

	is_mounted=`cat /proc/mounts | grep "/mnt/$DRIVE_B"`
	is_unformated=`if [ -f /tmp/.nvt_mounts ]; then cat /tmp/.nvt_mounts | grep "/mnt/$DRIVE_B"; fi`
	if [ -z "$is_mounted" ] && [ -z "$is_unformated" ] && [ ! -z $DRIVE_B ]; then
		ugxstrg_sent ${UGXSTRG_STAT_NODEV} $DRIVE_B;
	fi
}

# arg 1: ready/unformatted/nodev
# arg 2: folder name
ugxstrg_sent()
{
	if [ ! -z $DRIVE_A ] && [ "$DRIVE_A" == "$2" ]; then
		ugxstrg -status $1 $2 A
	fi
	if [ ! -z $DRIVE_B ] && [ "$DRIVE_B" == "$2" ]; then
		ugxstrg -status $1 $2 B
	fi
	if [ ! -z $DRIVE_C ] && [ "$DRIVE_C" == "$2" ]; then
		ugxstrg -status $1 $2 C
	fi
	if [ ! -z $DRIVE_D ] && [ "$DRIVE_D" == "$2" ]; then
		ugxstrg -status $1 $2 D
	fi
}

# arg 1: ready/unformatted/nodev
# arg 2: folder name
ugxstrg_sent_pstore()
{
	ugxstrg -status $1 $2 E
}

ugxstrg_init

if [ -z $DEVPATH ]; then
	# This is for boot stage handling
	MMCBUSPATH="/sys/bus/mmc/devices/"
	MMCDEVLIST=`ls $MMCBUSPATH`
	time_offset_sig=`date +%z | cut -c 1`
	time_offset_h=`date +%z | cut -c 2-3`
	time_offset_m=`date +%z | cut -c 4-5`
	time_offset=`expr $local_time - $utc_time`
	if [ $time_offset_sig == + ]; then
		time_offset_sig="";
	fi
	time_offset_total_m=$time_offset_sig`expr $time_offset_h \* 60 + $time_offset_m`

	for n in $MMCDEVLIST
	do
		# Check if it is not SD device
		SD_TYPE=`cat $MMCBUSPATH/$n/type`
		if [ $SD_TYPE == SDIO ]; then
			continue
		fi

		if [ $SD_TYPE == MMC ]; then
			# To check if it's the emmc storage device
			if [ -f $MMCBUSPATH/$n/bga ]; then
				BGA=`cat $MMCBUSPATH/$n/bga`
				if [ 1 == $BGA ]; then
					# Get the block device name
					BLOCKDEV=`ls $MMCBUSPATH/$n/block`

					# Check if device is mounted
					MOUNTED=`grep $BLOCKDEV /proc/mounts`
					if [ ! -z "$MOUNTED" ]; then
						continue
					fi

					if [ "$NVT_ROOTFS_TYPE" == "NVT_ROOTFS_TYPE_EMMC" ]; then
						if [ -b "/dev/${BLOCKDEV}p1" ]; then
							if ! mount -t ext4 /dev/${BLOCKDEV}p1 /mnt/${PST_DIR}; then
								echo yes | mkfs.ext4 /dev/${BLOCKDEV}p1;
								if ! mount -t ext4 /dev/${BLOCKDEV}p1 /mnt/${PST_DIR}; then
									ugxstrg_sent_pstore ${UGXSTRG_STAT_FS_ERR} ${PST_DIR};
									exit 1;
								fi
							fi
							ugxstrg_sent_pstore ${UGXSTRG_STAT_MOUNTED} ${PST_DIR};
						fi
						if [ -b "/dev/${BLOCKDEV}p5" ]; then
							if ! mount -t ext4 /dev/${BLOCKDEV}p5 /mnt/${EMMC1_DIR}; then
								echo yes | mkfs.ext4 /dev/${BLOCKDEV}p5;
								if ! mount -t ext4 /dev/${BLOCKDEV}p5 /mnt/${EMMC1_DIR}; then
									ugxstrg_sent ${UGXSTRG_STAT_FS_ERR} ${EMMC1_DIR};
									exit 1;
								fi
							fi
							ugxstrg_sent ${UGXSTRG_STAT_MOUNTED} ${EMMC1_DIR};
						fi
						if [ -b "/dev/${BLOCKDEV}p6" ]; then
							if ! mount -t ext4 /dev/${BLOCKDEV}p6 /mnt/${EMMC2_DIR}; then
								echo yes | mkfs.ext4 /dev/${BLOCKDEV}p6;
								if ! mount -t ext4 /dev/${BLOCKDEV}p6 /mnt/${EMMC2_DIR}; then
									ugxstrg_sent ${UGXSTRG_STAT_FS_ERR} ${EMMC2_DIR};
									exit 1;
								fi
							fi
							ugxstrg_sent ${UGXSTRG_STAT_MOUNTED} ${EMMC2_DIR};
						fi
					else
						# Using fdisk to check if it needs to be partitioned
						if [ -f /etc/autofdisk.sh ]; then
							mknod /dev/${BLOCKDEV} b `cat /sys/block/${BLOCKDEV}/dev | sed "s/:/\ /g"`
							/etc/autofdisk.sh ${BLOCKDEV}
							if [ $? != 0 ]; then
								echo -e "\e[1;31m\rUpdate rootfs failed. #1\r\e[0m"
								exit 1;
							fi
						fi
						sync
						sleep 1
						if [ -b "/dev/${BLOCKDEV}p1" ]; then
							if ! mount -t ext4 /dev/${BLOCKDEV}p1 /mnt/${EMMC1_DIR}; then
								echo yes | mkfs.ext4 /dev/${BLOCKDEV}p1;
								if ! mount -t ext4 /dev/${BLOCKDEV}p1 /mnt/${EMMC1_DIR}; then
									ugxstrg_sent ${UGXSTRG_STAT_FS_ERR} ${EMMC1_DIR};
									exit 1;
								fi
							fi
							ugxstrg_sent ${UGXSTRG_STAT_MOUNTED} ${EMMC1_DIR};
						fi
						if [ -b "/dev/${BLOCKDEV}p2" ]; then
							if ! mount -t ext4 /dev/${BLOCKDEV}p2 /mnt/${EMMC2_DIR}; then
								echo yes | mkfs.ext4 /dev/${BLOCKDEV}p2;
								if ! mount -t ext4 /dev/${BLOCKDEV}p2 /mnt/${EMMC2_DIR}; then
									ugxstrg_sent ${UGXSTRG_STAT_FS_ERR} ${EMMC2_DIR};
									exit 1;
								fi
							fi
							ugxstrg_sent ${UGXSTRG_STAT_MOUNTED} ${EMMC2_DIR};
						fi
					fi
				fi
			fi
			continue
		fi

		# Get the block device name
		BLOCKDEV=`ls $MMCBUSPATH/$n/block`
		# Check if device is mounted
		MOUNTED=`grep $BLOCKDEV /proc/mounts`

		# Create folder
		if [ ! -z `echo $n | grep mmc0` ]; then
			MNT_DIR=$SD1_DIR
		elif [ ! -z `echo $n | grep mmc1` ]; then
			MNT_DIR=$SD2_DIR
		else
			MNT_DIR=$SD3_DIR
		fi

		if [ ! -z "$MOUNTED" ]; then
			ugxstrg_sent ${UGXSTRG_STAT_MOUNTED} $MNT_DIR;
			continue
		fi

		# Check if /dev/mmcblk* exists
		if [ -b /dev/$BLOCKDEV ]; then
			MOUNTDEV="/dev/$BLOCKDEV"
			if [ -b "/dev/${BLOCKDEV}p1" ]; then
				MOUNTDEV="/dev/${BLOCKDEV}p1"
			fi
		else
			continue
		fi

		uctrl usetup -timeoffset $time_offset_h:$time_offset_m
		# Inserted but can't be mounted
		fat_type=`blkid "$MOUNTDEV" | awk -F'TYPE=' '{print $NF}'`
		if [ "${fat_type}" == "\"vfat\"" ]; then
			if ! mount -o dirsync,time_offset=$time_offset_total_m "$MOUNTDEV" "${MNT_PATH}/${MNT_DIR}" 2>&1 | tee -a /tmp/mountstat; then
				echo "$MOUNTDEV $MNT_PATH/$MNT_DIR ignore defaults 0 0" >> /tmp/.nvt_mounts
				ugxstrg_sent ${UGXSTRG_STAT_FS_ERR} $MNT_DIR;
				continue
			fi
		elif [ "${fat_type}" == "\"exfat\"" ]; then
			if ! mount -t exfat "$MOUNTDEV" "${MNT_PATH}/${MNT_DIR}" 2>&1 | tee -a /tmp/mountstat; then
				echo "$MOUNTDEV $MNT_PATH/$MNT_DIR ignore defaults 0 0" >> /tmp/.nvt_mounts
				ugxstrg_sent ${UGXSTRG_STAT_FS_ERR} $MNT_DIR;
				continue
			fi
		else
			echo "Unkown SD type!!!"
			echo "$MOUNTDEV $MNT_PATH/$MNT_DIR ignore defaults 0 0" >> /tmp/.nvt_mounts
			ugxstrg_sent ${UGXSTRG_STAT_FS_ERR} $MNT_DIR;
			continue
		fi

		ugxstrg_sent ${UGXSTRG_STAT_MOUNTED} $MNT_DIR;
	done
	ugxstrg_check_drive_is_mounted;
	touch /tmp/.nvt_mounts
else
	# This is for booted up stage
	case "${ACTION}" in
	add|"")
		my_umount ${MDEV}
		my_mount ${MDEV}
		;;
	remove)
		my_umount ${MDEV}
		;;
	esac
fi
