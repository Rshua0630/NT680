#!/bin/bash
CHK_PATCH_SCRIPT="scripts/nvtcheckpatch.pl"
CHK_RESULT="$OUTPUT_DIR/nvt_check_result"

help()
{
	echo -e "\e[1;33mUsage:\e[0m"
	echo -e "\t\e[1;37m\$BUILD_DIR/nvt-tools/nvt_check_cstyle.sh <Check Path>\e[0m"
	echo -e "\te.g."
	echo -e "\t\t\$ \$BUILD_DIR/nvt-tools/nvt_check_cstyle.sh linux-supplement/misc/nvt_pstore/"
}

env_init()
{
	cd $KERNELDIR
	rm -rf $OUTPUT_DIR/nvt_check_result
	mkdir -p $OUTPUT_DIR/nvt_check_result
}

nvt_check_cstyle_error()
{
	log_all="$CHK_RESULT/all`echo $1 | sed -e "s/\//+/g"`"
	log_err="$CHK_RESULT/err`echo $1 | sed -e "s/\//+/g"`"
	log_warn="$CHK_RESULT/warn`echo $1 | sed -e "s/\//+/g"`"
	$CHK_PATCH_SCRIPT -f $1 > $log_all
	ret="`cat $log_all | grep "ERROR"`"
	if [ -z "$ret" ]; then
		# No error
		return 0;
	else
		# Error
		return 1;
	fi
}

check_file_by_path()
{
	# Get file list
	if [ -f $1 ]; then
		file_list=$1
	else
		base_path=$1
		file_list="`find $base_path -type f -name "*.h" -o -name "*.c" ! -name "*.mod.c"`"
	fi
	# Check all files
	RET=0
	for n in $file_list
	do
		nvt_check_cstyle_error $n
		ret=$?
		if [ $ret -ne 0 ]; then
			RET=$ret
		fi
	done
	# List check result
	if [ $RET -ne 0 ]; then
		echo -e "\e[1;37m!!!ERROR!!!\e[0m"
		echo -e "\e[1;33m\tPlease check on this path: $CHK_RESULT\e[0m"
		pushd $CHK_RESULT > /dev/null 2>&1;
		result_checked_files=`ls`
		for n in $result_checked_files
		do
			if [ -n "`cat $CHK_RESULT/$n | grep "ERROR"`" ]; then
				echo -ne "\t$n" | sed -e "s/+/\//g" | sed -e "s/all//g"
				echo -e "\e[1;31m -> $n\e[0m"
			else
				rm $CHK_RESULT/$n
			fi
		done
		popd >/dev/null 2>&1;
	else
		echo -e "\e[1;33mCoding style test pass!!\e[0m"
	fi
}

if [ -z "$1" ]; then
	help
else
	check_path="$PWD/$1"
	env_init
	check_file_by_path $check_path
fi
