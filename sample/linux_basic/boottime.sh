#uboot_criteria=750000	#Uboot difference
ker_criteria=3300000	#Uboot start to Linux kernel
fs_criteria=3600000	#Uboot start to fs

# arg 1: /proc/nvt_info/bootts boot time data
# arg 2: To measure stage (e.g. Uboot ker...)
# arg 3: Pass criteria in us (Uboot to stage time, e.g. Kernel end - Uboot start)
parse_boot_data()
{
	boot_data=$1
	parse_stage=$2
	cond_pass=$3
	time_start=""
	time_end=""
	tmp=""

	for n in $boot_data
	do
		curr=$n;
		if [ "$prev" == "Uboot" ] && [ -z "$tmp" ]; then
			tmp=1;
		elif [ "$prev" == "Uboot" ] && [ ! -z "$tmp" ]; then
			time_start=$curr;
		elif [ "$prev" == "$parse_stage" ] && [ -z "$tmp" ]; then
			tmp=1;
		elif [ "$prev" == "$parse_stage" ] && [ ! -z "$tmp" ]; then
			time_end=$curr;
		fi
		prev=$n;
	done

	time_diff=$((time_end-time_start))
	if [ $time_diff -gt $cond_pass ]; then
		echo $time_diff "us"
		return 1
	else
		return 0
	fi
}

time_data=`cat /proc/nvt_info/bootts`

#parse_boot_data "$time_data" Uboot $uboot_criteria
#ret=$?
#if [ $ret -eq 1 ]; then
#	echo "Uboot boot time can't pass test criteria $uboot_criteria us"
#	ucmd exam msg EXECUTED_LINUX NG
#fi

parse_boot_data "$time_data" ker $ker_criteria
ret=$?
if [ $ret -eq 1 ]; then
	echo "Uboot to Linux kernel boot time can't pass test $ker_criteria us"
	ucmd exam msg EXECUTED_LINUX NG
fi

parse_boot_data "$time_data" fs $fs_criteria
ret=$?
if [ $ret -eq 1 ]; then
	echo "Uboot to fs boot time can't pass test $fs_criteria us"
	ucmd exam msg EXECUTED_LINUX NG
fi

ucmd exam msg EXECUTED_LINUX OK
