#!/bin/sh
modprobe mmc_test

testmmcinit()
{
	if [ $1 == mmc0 ]; then
		MMC_TEST_NAME=`ls /sys/bus/mmc/drivers/mmcblk/ | grep mmc0`
	else
		MMC_TEST_NAME=`ls /sys/bus/mmc/drivers/mmcblk/ | grep mmc1`
	fi

	if [ ! -z $MMC_TEST_NAME ]; then
		echo "unbind mmcblk"
		echo $MMC_TEST_NAME > /sys/bus/mmc/drivers/mmcblk/unbind

		echo "bind mmc_test"
		echo $MMC_TEST_NAME > /sys/bus/mmc/drivers/mmc_test/bind
	fi
}

testmmclist()
{
	if [ $1 == mmc0 ]; then
		echo "mmc0 testlist:"
		cat /sys/kernel/debug/mmc0/$1*/testlist
	else
		echo "mmc1 testlist:"
		cat /sys/kernel/debug/mmc1/$1*/testlist
	fi
}

testmmcstart()
{
	if [ $1 == mmc0 ]; then
		echo "mmc0 test start:"
		cd /sys/kernel/debug/mmc0/$1*/
	else
		echo "mmc1 test start:"
		cd /sys/kernel/debug/mmc1/$1*/
	fi 

	if [ -z $2 ]; then
		for n in $(seq 1 45);
		do
			echo $n > test;
		done
	else
		echo $2 > test;
	fi
}

help()
{
	echo "sdcard_test.sh mmc0/mmc1"
	echo "E.g."
	echo "	sdcard_test.sh mmc0"
	echo "	sdcard_test.sh mmc1"
	echo "	sdcard_test.sh mmc1 30   => Run 30th test item once"
	echo "	sdcard_test.sh help"
}

case "$1" in
	mmc0)
		testmmcinit mmc0
		testmmclist mmc0
		testmmcstart mmc0 $2
		;;
	mmc1)
		testmmcinit mmc1
		testmmclist mmc1
		testmmcstart mmc1 $2
		;;
	help)
		help
		;;
	*)
		help
		;;
esac
