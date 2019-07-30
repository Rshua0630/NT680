#!/bin/bash
img_name=$1
copy_tmp_dir=$2
size=$3
CURPWD=$(pwd)

# 1. Generate raw fat32 image
dd if=/dev/zero of=$img_name bs=1M count=$size
mkfs.vfat $img_name
file_list="`ls $copy_tmp_dir | grep -v "\bEOF\b"`"
mkdir $CURPWD/bin/mnt
sudo mount -o loop $img_name bin/mnt
if [ -z "$file_list" ]; then 
	echo -e "\e[1;33mRecovery partition is empty.\e[0m"
	pushd $copy_tmp_dir
	sudo cp EOF $CURPWD/bin/mnt
	popd
else
	echo -e "\e[1;33mRecovery partition will copy the following files:\e[0m\n$file_list"
	pushd $copy_tmp_dir
	sudo cp -r $file_list $CURPWD/bin/mnt
	sudo cp EOF $CURPWD/bin/mnt
	popd
fi
sudo umount bin/mnt
