#!/bin/sh
#NVT RTC Test version 2.0

if [ ! -n "$1" ]; then
	echo "1. Please enter the date you'd like to test"
	echo "Format: rtc_test.sh MMDDhhmmYYYY"
	echo "Ex: rtc_test.sh 020511292015"
	echo "Which means 2015/02/05 11:29"
	echo " "
	echo "2. Use rtc_test.sh s to do full-date scan"
	echo " "
	echo "3. Use rtc_test.sh a to do WakeAlarm test"
elif [ "$1" == "a" ]; then
	if [ ! -n "$2" ]; then
		echo "Please enter the WakeAlarm time you'd like to test"
		echo "Format: rtc_test.sh a [second]"
		echo "Ex: rtc_test.sh a 300"
		echo "Which means device will boot at 300s later"
	else
		echo +$2 > /sys/class/rtc/rtc0/wakealarm
		sleep 1
		poweroff
	fi
elif [ "$1" == "s" ]; then
	for i in 01 03 05 07 08 10 12; do
		for j in 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31; do
			YearDate=${i}$j
			echo "The time before setting is:"
			date
			date ${YearDate}11112015 > /dev/null
			hwclock -u -w
			echo "Current time is:"
			hwclock
			echo " "
			echo "The time before setting is:"
			date
			date ${YearDate}11112016 > /dev/null
			hwclock -u -w
			echo "Current time is:"
			hwclock
			echo " "
		done
	done


	for i in 04 06 09 11; do
		for j in 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30; do
			YearDate=${i}$j
			echo "The time before setting is:"
			date
			date ${YearDate}11112015 > /dev/null
			hwclock -u -w
			echo "Current time is:"
			hwclock
			echo " "
			echo "The time before setting is:"
			date
			date ${YearDate}11112016 > /dev/null
			hwclock -u -w
			echo "Current time is:"
			hwclock
			echo " "
		done
	done

	for j in 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28; do
		YearDate=02$j
		echo "The time before setting is:"
		date
		date ${YearDate}11112015 > /dev/null
		hwclock -u -w
		echo "Current time is:"
		hwclock
		echo " "
		echo "The time before setting is:"
		date
		date ${YearDate}11112016 > /dev/null
		hwclock -u -w
		echo "Current time is:"
		hwclock
		echo " "
	done

#LeapYear
	echo "The time before setting is:"
	date
	date 022911112016 > /dev/null
	hwclock -u -w
	echo "Current time is:"
	hwclock
	echo " "

else	
	echo "The time before setting is:"
	date
	date $1 > /dev/null
	hwclock -u -w
	echo "Current time is:"
	hwclock
fi
