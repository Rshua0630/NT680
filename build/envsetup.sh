#!/bin/bash
DEFAULT_NVT_ACTIVE_PROJ=DemoKit
DEFAULT_NVT_MODEL_CFG=`cat uitron/Project/DemoKit/ModelConfig.txt  | grep "MODEL" | grep "=" | awk -F'=' '{print $NF}' | tr -d ' ' | fromdos`
DEFAULT_NVT_CROSS=arm-ca53-linux-gnueabihf-4.9-2017.05
DEFAULT_NVTCONFIG_FILE_NAME=.nvt_default

function hmm() {
cat <<EOF
Invoke ". build/envsetup.sh" from your shell to add the following functions to your environment:
- lunch:   lunch <iTron-Kit> <ModelConfig> <Toolchain>
- croot:   Changes directory to the top of the tree.
- m:       Makes from the top of the tree.
- mr:      Makes rootfs.
- cso:     Copy shared libraries to rootfs.

Environemnt options:
- SANITIZE_HOST: Set to 'true' to use ASAN for all host modules. Note that
                 ASAN_OPTIONS=detect_leaks=0 will be set by default until the
                 build is leak-check clean.

Look at the source to view more functions. The complete list is:
EOF
	T=$(gettop)
	local A
	A=""
	for i in `cat $T/build/envsetup.sh | sed -n "/^[ \t]*function /s/function \([a-z_]*\).*/\1/p" | sort | uniq`; do
		A="$A $i"
	done
	echo $A
}

function show_env_by_name()
{
	echo $1 = ${!1}
}

function get_stuff_for_environment()
{
	echo "======================================== NVT Setting ========================================"
	show_env_by_name NVT_PRJCFG_MODEL_CFG
	show_env_by_name NVT_PRJCFG_MODEL_MK
	show_env_by_name LINUX_BUILD_TOP
	show_env_by_name ROOTFS_DIR
	show_env_by_name APP_DIR
	show_env_by_name LIBRARY_DIR
	show_env_by_name INCLUDE_DIR
	show_env_by_name SAMPLE_DIR
	show_env_by_name TOOLS_DIR
	show_env_by_name UBOOT_DIR
	show_env_by_name KERNELDIR
	show_env_by_name BUSYBOX_DIR
	show_env_by_name OUTPUT_DIR
	show_env_by_name LOGS_DIR
	show_env_by_name SUPPLEMENT_DIR
	show_env_by_name BUILD_DIR
	show_env_by_name UITRON_DIR
	show_env_by_name CONFIG_DIR
	show_env_by_name PLATFORM_CFLAGS
	show_env_by_name NVT_HOST
	show_env_by_name CROSS_COMPILE
	show_env_by_name CROSS_TOOLCHAIN_PATH
	show_env_by_name CROSS_TOOLCHAIN_BIN_PATH
	show_env_by_name CC
	show_env_by_name CXX
	show_env_by_name CPP
	show_env_by_name LD
	show_env_by_name LDD
	show_env_by_name AR
	show_env_by_name NM
	show_env_by_name STRIP
	show_env_by_name OBJCOPY
	show_env_by_name PATH
	echo "============================================================================================="
}

function clear_nvt_environment()
{
	unset NVT_PRJCFG_MODEL_CFG
	unset NVT_PRJCFG_MODEL_MK
	unset NVT_PRJCFG_MODEL
	unset LINUX_BUILD_TOP
	unset ROOTFS_DIR
	unset APP_DIR
	unset LIBRARY_DIR
	unset INCLUDE_DIR
	unset SAMPLE_DIR
	unset TOOLS_DIR
	unset PRJCFG_DIR
	unset UBOOT_DIR
	unset KERNELDIR
	unset BUSYBOX_DIR
	unset OUTPUT_DIR
	unset LOGS_DIR
	unset SUPPLEMENT_DIR
	unset BUILD_DIR
	unset UITRON_DIR
	unset CONFIG_DIR
	unset PLATFORM_CFLAGS
	unset NVT_HOST
	unset NVT_CROSS
	unset CROSS_COMPILE
	unset CROSS_TOOLCHAIN_PATH
	unset CROSS_TOOLCHAIN_BIN_PATH
	unset CC
	unset CXX
	unset CPP
	unset LD
	unset LDD
	unset AR
	unset NM
	unset STRIP
	unset OBJCOPY
}

function set_stuff_for_environment()
{
	export LINUX_BUILD_TOP=$PWD
	export ROOTFS_DIR=${LINUX_BUILD_TOP}/root-fs
	export APP_DIR=${LINUX_BUILD_TOP}/application
	export LIBRARY_DIR=${LINUX_BUILD_TOP}/lib
	export INCLUDE_DIR=${LINUX_BUILD_TOP}/include
	export SAMPLE_DIR=${LINUX_BUILD_TOP}/sample
	export TOOLS_DIR=${LINUX_BUILD_TOP}/tools
	export UBOOT_DIR=${LINUX_BUILD_TOP}/u-boot
	export KERNELDIR=${LINUX_BUILD_TOP}/linux-kernel
	export BUSYBOX_DIR=${LINUX_BUILD_TOP}/busybox
	export SUPPLEMENT_DIR=${LINUX_BUILD_TOP}/linux-supplement
	export BUILD_DIR=${LINUX_BUILD_TOP}/build
	export OUTPUT_DIR=${LINUX_BUILD_TOP}/output
	export LOGS_DIR=${LINUX_BUILD_TOP}/logs
	export UITRON_DIR=${LINUX_BUILD_TOP}/uitron
	export CONFIG_DIR=${LINUX_BUILD_TOP}/configs
	export NVT_CROSS=""
	unset NVT_PRJCFG_MODEL
	unset PRJCFG_DIR
	setpaths
}

function gettop
{
	if [ -n "$LINUX_BUILD_TOP" ] ; then
		echo $LINUX_BUILD_TOP
		return
	fi
	echo $PWD
}

function gen_modelconfig()
{
	echo export NVT_PRJCFG_CFG=${NVT_PRJCFG_CFG} > ${BUILD_DIR}/${DEFAULT_NVTCONFIG_FILE_NAME}
	echo -n export NVT_PRJCFG_MODEL_CFG='${UITRON_DIR}/Project/'${NVT_PRJCFG_CFG}/ >> ${BUILD_DIR}/${DEFAULT_NVTCONFIG_FILE_NAME}
	echo ${NVT_PRJCFG_MODEL_CFG} | awk -F'/' '{print $NF}' >> ${BUILD_DIR}/${DEFAULT_NVTCONFIG_FILE_NAME}
	echo export NVT_CROSS=${NVT_CROSS} >> ${BUILD_DIR}/${DEFAULT_NVTCONFIG_FILE_NAME}
	# To configure your selected project type to uitron codebase
	sed -i '/ACTIVE_PROJECT =/d' ${UITRON_DIR}/Project/ProjectConfig.txt
	echo ACTIVE_PROJECT = ${NVT_PRJCFG_CFG} >> ${UITRON_DIR}/Project/ProjectConfig.txt
	# To configure your selected Model type to uitron codebase
	sed -i '/MODEL =/d' ${UITRON_DIR}/Project/${NVT_PRJCFG_CFG}/ModelConfig.txt
	echo MODEL = `echo ${NVT_PRJCFG_MODEL_CFG} | awk -F/ '{print $NF}' |awk -F_ '{for(i=2;i<=NF;i++) printf "%s_", $i;}' | awk -F. '{print $1}'` >> ${UITRON_DIR}/Project/${NVT_PRJCFG_CFG}/ModelConfig.txt
	if [ "${NVT_PRJCFG_CFG}" == "EmuKit" ]; then
		sed -i 's/'"^export USE_ECOS_KERNEL := .*"'/export USE_ECOS_KERNEL := OFF/g' ${UITRON_DIR}/MakeCommon/MakeOption.txt
		sed -i 's/'"^export NETWORK := .*"'/export NETWORK := OFF/g' ${UITRON_DIR}/MakeCommon/MakeOption.txt
	else
		CPU1_TYPE=`cat $NVT_PRJCFG_MODEL_CFG  | grep "CPU1_TYPE" | grep "=" | awk -F'=' '{print $NF}' | tr -d ' ' | fromdos`;
		if [ "$CPU1_TYPE" = "CPU1_ECOS" ]; then
			sed -i 's/'"^export USE_ECOS_KERNEL := .*"'/export USE_ECOS_KERNEL := ON/g' ${UITRON_DIR}/MakeCommon/MakeOption.txt
		else
			sed -i 's/'"^export USE_ECOS_KERNEL := .*"'/export USE_ECOS_KERNEL := OFF/g' ${UITRON_DIR}/MakeCommon/MakeOption.txt
		fi
		NETWORK_TYPE=`cat $NVT_PRJCFG_MODEL_CFG  | grep "NETWORK_TYPE" | grep "=" | awk -F'=' '{print $NF}' | tr -d ' ' | fromdos`;
		if [ "$NETWORK_TYPE" = "NETWORK_NONE" ]; then
			sed -i 's/'"^export NETWORK := .*"'/export NETWORK := OFF/g' ${UITRON_DIR}/MakeCommon/MakeOption.txt
		elif [ "$NETWORK_TYPE" = "NETWORK_CPU1" ]; then
			sed -i 's/'"^export NETWORK := .*"'/export NETWORK := ON_CPU1/g' ${UITRON_DIR}/MakeCommon/MakeOption.txt
		else
			sed -i 's/'"^export NETWORK := .*"'/export NETWORK := ON_CPU2/g' ${UITRON_DIR}/MakeCommon/MakeOption.txt
		fi
	fi
	make -C ${CONFIG_DIR} clean > /dev/null 2>&1
	make -C ${CONFIG_DIR} gen_cfg > /dev/null 2>&1
}

function setpaths()
{
	T=$(gettop)
	if [ ! "$T" ]; then
		echo "Couldn't locate the top of the tree.  Try setting TOP."
		return
	fi

	# Include previous default setting
	if [ -f ${BUILD_DIR}/${DEFAULT_NVTCONFIG_FILE_NAME} ]; then
		source ${BUILD_DIR}/${DEFAULT_NVTCONFIG_FILE_NAME}
	fi

	##################################################################
	#                                                                #
	#              Read me before you modify this code               #
	#                                                                #
	#   This function sets CROSS_TOOLCHAIN_PATH to what it is adding #
	#   to PATH, and the next time it is run, it removes that from   #
	#   PATH.  This is required so lunch can be run more than once   #
	#   and still have working paths.                                #
	#                                                                #
	##################################################################

	# Note: on windows/cygwin, LINUX_BUILD_PATH will contain spaces
	# due to "C:\Program Files" being in the path.

	# out with the old
	if [ -n "$LINUX_BUILD_PATH" ] ; then
		export PATH=${PATH/$LINUX_BUILD_PATH/}
		# strip leading ':', if any
		export PATH=${PATH#*:}
	fi

	if [ -z "$NVT_PRJCFG_MODEL_CFG" ]; then
		export NVT_PRJCFG_CFG=${DEFAULT_NVT_ACTIVE_PROJ}
		export NVT_PRJCFG_MODEL_CFG=${UITRON_DIR}/Project/${DEFAULT_NVT_ACTIVE_PROJ}/ModelConfig_${DEFAULT_NVT_MODEL_CFG}.txt
	fi
	export NVT_PRJCFG_MODEL_MK=${UITRON_DIR}/Project/${NVT_PRJCFG_CFG}/MakeConfig.txt
	# default cross toolchain is glibc
	if [ "$NVT_CROSS" == "arm-ca53-linux-gnueabihf-4.9-2017.05" ]; then
		export NVT_CROSS=arm-ca53-linux-gnueabihf-4.9-2017.05
		if [ -d /opt/ivot/$NVT_CROSS ]; then
			export CROSS_TOOLCHAIN_PATH=/opt/ivot/${NVT_CROSS};
		else
			export CROSS_TOOLCHAIN_PATH=/opt/arm/${NVT_CROSS};
		fi
		export CROSS_TOOLCHAIN_BIN_PATH=${CROSS_TOOLCHAIN_PATH}/usr/bin
		export NVT_HOST=arm-ca53-linux-gnueabihf
		export SYSROOT_PATH=${CROSS_TOOLCHAIN_PATH}/usr/${NVT_HOST}/sysroot
		export CROSS_COMPILE=${CROSS_TOOLCHAIN_BIN_PATH}/${NVT_HOST}-
	else
		export NVT_CROSS=arm-ca53-linux-uclibcgnueabihf-4.9-2017.02
		if [ -d /opt/ivot/$NVT_CROSS ]; then
			export CROSS_TOOLCHAIN_PATH=/opt/ivot/${NVT_CROSS};
		else
			export CROSS_TOOLCHAIN_PATH=/opt/arm/${NVT_CROSS};
		fi
		export CROSS_TOOLCHAIN_BIN_PATH=${CROSS_TOOLCHAIN_PATH}/usr/bin
		export NVT_HOST=arm-ca53-linux-uclibcgnueabihf
		export SYSROOT_PATH=${CROSS_TOOLCHAIN_PATH}/usr/${NVT_HOST}/sysroot
		export CROSS_COMPILE=${CROSS_TOOLCHAIN_BIN_PATH}/${NVT_HOST}-
	fi

	export LINUX_BUILD_PATH=${CROSS_TOOLCHAIN_PATH}/bin
	export PATH=${CROSS_TOOLCHAIN_BIN_PATH}:${PATH}
	export ARCH=arm
	#----------------------------------------------------------------------
	# Machine Dependent Options
	#----------------------------------------------------------------------
	# -march=armv8-a    : Generate code that will run on ARMV8
	# -mtune=cortex-a53 : Optimize for cortex-a53
	# -mfpu=neon        : Do not use floating-point coprocessor instructions
	# -mfloat-abi=hard  : To indicate that NEON variables must be passed in general purpose registers
	#----------------------------------------------------------------------
	# C Language Options
	#----------------------------------------------------------------------
	# -fno-builtin  : Don't recognize built-in functions that do not begin with `__builtin_' as prefix
	# -ffreestanding: Assert that compilation takes place in a freestanding environment
	#----------------------------------------------------------------------
	# Code Generation Options
	#----------------------------------------------------------------------
	# -fno-common   : The compiler should place uninitialized global variables in the data section of the object file, rather than generating them as common blocks
	# -fshort-wchar : Override the underlying type for `wchar_t' to be `short unsigned int' instead of the default for the target.
	#----------------------------------------------------------------------
	if [ "$NVT_CROSS" == "arm-ca53-linux-uclibcgnueabihf-4.9-2017.02" ]; then
		export PLATFORM_CFLAGS="-march=armv8-a -mtune=cortex-a53 -mfpu=neon-fp-armv8 -mfloat-abi=hard -ftree-vectorize -fno-builtin -fno-common "
	else
		export PLATFORM_CFLAGS="-march=armv7-a -mtune=cortex-a53 -mfpu=neon-vfpv4 -mfloat-abi=hard -ftree-vectorize -fno-builtin -fno-common "
	fi
	export CC="${CROSS_COMPILE}gcc"
	export CXX="${CROSS_COMPILE}g++"
	export CPP="${CROSS_COMPILE}gcc"
	export LD=${CROSS_COMPILE}ld
	if [ "$NVT_CROSS" == "arm-ca53-linux-uclibcgnueabihf-4.9-2017.02" ]; then
		export LDD="${CROSS_COMPILE}ldd"
	else
		export LDD="${CROSS_COMPILE}ldd --root=${SYSROOT_PATH}"
	fi
	export AR=${CROSS_COMPILE}ar
	export NM=${CROSS_COMPILE}nm
	export STRIP=${CROSS_COMPILE}strip
	export OBJCOPY=${CROSS_COMPILE}objcopy
	export OBJDUMP=${CROSS_COMPILE}objdump
	if [ -z "`echo $LD_LIBRARY_PATH | grep ${CROSS_TOOLCHAIN_PATH}/usr/lib`" ]; then
		if [ ! -z "${LD_LIBRARY_PATH}" ]; then
			export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:${CROSS_TOOLCHAIN_PATH}/usr/local/lib
		else
			export LD_LIBRARY_PATH=${CROSS_TOOLCHAIN_PATH}/usr/local/lib
		fi
	fi
	gen_modelconfig
}

# Clear this variable.  It will be built up again when the vendorsetup.sh
# files are included at the end of this file.
unset LUNCH_MENU_CHOICES
function add_lunch_combo()
{
	local new_combo=$1
	local c
	for c in ${LUNCH_MENU_CHOICES[@]} ; do
		if [ "$new_combo" = "$c" ] ; then
			return
		fi
	done
	LUNCH_MENU_CHOICES=(${LUNCH_MENU_CHOICES[@]} $new_combo)
}

unset LUNCH_TOOLCHAIN_MENU_CHOICES
function add_lunch_toolchain_combo()
{
	local new_combo=$1
	local c
	for c in ${LUNCH_TOOLCHAIN_MENU_CHOICES[@]} ; do
		if [ "$new_combo" = "$c" ] ; then
			return
		fi
	done
	LUNCH_TOOLCHAIN_MENU_CHOICES=(${LUNCH_TOOLCHAIN_MENU_CHOICES[@]} $new_combo)
}

unset LUNCH_PROJ_CONFIG_MENU_CHOICES
function add_lunch_proj_config_combo()
{
	local new_combo=$1
	local c
	for c in ${LUNCH_PROJ_CONFIG_MENU_CHOICES[@]} ; do
		if [ "$new_combo" = "$c" ] ; then
			return
		fi
	done
	LUNCH_PROJ_CONFIG_MENU_CHOICES=(${LUNCH_PROJ_CONFIG_MENU_CHOICES[@]} $new_combo)
}

function add_prjcfgconfig()
{
	unset LUNCH_PROJ_CONFIG_MENU_CHOICES
	pushd $UITRON_DIR/Project
	for n in `find . -maxdepth 1 -type d | cut -d'.' -f2 | cut -d'/' -f2 | sed '1d' | fromdos`
	do
		add_lunch_proj_config_combo $n
	done
	popd
}

function add_modelconfig()
{
	unset LUNCH_MENU_CHOICES
	local model_list=`cd ${UITRON_DIR}/Project/${NVT_PRJCFG_CFG}; ls --color=never ModelConfig_*`
	for n in ${model_list}
	do
		add_lunch_combo $n
	done
}

function print_lunch_prjcfg_menu()
{
	local uname=$(uname)
	echo
	echo "You're building on" $uname
	echo
	echo "Lunch menu... select your Project config:"

	local i=1
	local choice
	for choice in ${LUNCH_PROJ_CONFIG_MENU_CHOICES[@]}
	do
		echo "     $i. $choice"
		i=$(($i+1))
	done

	echo
}

function print_lunch_menu()
{
	local uname=$(uname)
	echo
	echo "You're building on" $uname
	echo
	echo "Lunch menu... select your Model config:"

	local i=1
	local choice
	for choice in ${LUNCH_MENU_CHOICES[@]}
	do
		echo "     $i. $choice"
		i=$(($i+1))
	done

	echo
}

function print_lunch_toolchain_menu()
{
	local uname=$(uname)
	echo
	echo
	echo "Lunch menu... select your toolchain:"

	local i=1
	local choice
	for choice in ${LUNCH_TOOLCHAIN_MENU_CHOICES[@]}
	do
		echo "     $i. $choice"
		i=$(($i+1))
	done

	echo
}

function check_is_clean()
{
	if [ -d $ROOTFS_DIR/bin ]; then
		return 0;
	elif [ -d $OUTPUT_DIR ]; then
		return 0;
	else
		return 1;
	fi
}

function check_path_is_right()
{
	if [ ! -z "$LINUX_BUILD_TOP" ]; then
		cur_path=`pwd`;
		cond=`echo $cur_path | grep $LINUX_BUILD_TOP`
		if [ ! -z $cond ]; then
			return 1;
		else
			return 0;
		fi
	else
		return 0;
	fi
}

function lunch()
{
	add_prjcfgconfig
	local answer

	check_path_is_right
	if [ $? -eq 0 ]; then
		echo -e "\e[1;45mYour path is wrong!!\e[0m: SDK top path is $LINUX_BUILD_TOP\r\n"
		return 0;
	fi

	check_is_clean
	if [ $? -eq 0 ]; then
		echo -e "\e[1;45mChange model\e[0m: The menu can't be launched, please do clean build\e[1;45m(\$make clean)\e[0m first\r\n"
		echo -e "\e[1;37mCurrent selection: \e[0m"
		echo -e "\e[1;33m\t\t$NVT_PRJCFG_CFG\e[0m"
		echo -e "\e[1;33m\t\t`echo $NVT_PRJCFG_MODEL_CFG | awk -F'/' '{print $NF}'`\e[0m"
		return 0;
	fi

	# Include previous default setting
	if [ -f ${BUILD_DIR}/${DEFAULT_NVTCONFIG_FILE_NAME} ]; then
		echo "Loading default..."
		source ${BUILD_DIR}/${DEFAULT_NVTCONFIG_FILE_NAME}
		NVT_DEFAULT_NVT_MODEL=`echo ${NVT_PRJCFG_MODEL_CFG} | awk -F'/' '{print $NF}'`
		NVT_DEFAULT_NVT_CROSS=${NVT_CROSS}
		NVT_DEFAULT_PRJCFG_CFG=${NVT_PRJCFG_CFG}
	else
		NVT_DEFAULT_NVT_MODEL=${DEFAULT_NVT_MODEL_CFG}
		NVT_DEFAULT_NVT_CROSS=${DEFAULT_NVT_CROSS}
		NVT_DEFAULT_PRJCFG_CFG=${DEFAULT_NVT_ACTIVE_PROJ}
	fi

	# Project config selection
	if [ "$1" ] ; then
		num=1
		answer="NONE"
		for n in ${LUNCH_PROJ_CONFIG_MENU_CHOICES[@]}
		do
			if [ "$1" == "$n" ]; then
				answer=$num
				break
			fi
			num=$((num + 1))
		done
	else
		print_lunch_prjcfg_menu
		echo -n "Which would you like? [${NVT_DEFAULT_PRJCFG_CFG}] "
		read answer
	fi

	local selection=

	if [ -z "$answer" ]
	then
		selection=${NVT_DEFAULT_PRJCFG_CFG}
	elif (echo -n $answer | grep -q -e "^[0-9][0-9]*$")
	then
		if [ $answer -le ${#LUNCH_PROJ_CONFIG_MENU_CHOICES[@]} ]
		then
			selection=${LUNCH_PROJ_CONFIG_MENU_CHOICES[$(($answer-1))]}
			export NVT_PRJCFG_CFG=${selection}
			NVT_DEFAULT_NVT_MODEL="ModelConfig_"`cat uitron/Project/${NVT_PRJCFG_CFG}/ModelConfig.txt | grep "MODEL" | grep "=" | awk -F'=' '{print $NF}' | tr -d ' ' | fromdos`".txt"
		fi
	elif (echo -n $answer | grep -q -e "^[^\-][^\-]*-[^\-][^\-]*$")
	then
		selection=$answer
	fi

	if [ -z "$selection" ]
	then
		echo
		echo "Invalid lunch project config combo: $answer"
		return 1
	fi

	add_modelconfig
	# Model config selection
	if [ "$2" ] ; then
		num=1
		answer="NONE"
		for n in ${LUNCH_MENU_CHOICES[@]}
		do
			if [ "$2" == "$n" ]; then
				answer=$num
				break
			fi
			num=$((num + 1))
		done
	else
		print_lunch_menu
		echo -n "Which would you like? [${NVT_DEFAULT_NVT_MODEL}] "
		read answer
	fi

	local selection=

	if [ -z "$answer" ]
	then
		selection=${NVT_DEFAULT_NVT_MODEL}
	elif (echo -n $answer | grep -q -e "^[0-9][0-9]*$")
	then
		if [ $answer -le ${#LUNCH_MENU_CHOICES[@]} ]
		then
			selection=${LUNCH_MENU_CHOICES[$(($answer-1))]}
		fi
	elif (echo -n $answer | grep -q -e "^[^\-][^\-]*-[^\-][^\-]*$")
	then
		selection=$answer
	fi

	if [ -z "$selection" ]
	then
		echo
		echo "Invalid lunch model config combo: $answer"
		return 1
	fi

	export NVT_PRJCFG_MODEL_CFG=${UITRON_DIR}/Project/${NVT_PRJCFG_CFG}/${selection}
	export NVT_PRJCFG_MODEL_MK=${UITRON_DIR}/Project/${NVT_PRJCFG_CFG}/MakeConfig.txt

	# toolchain selection
	if [ "$3" ] ; then
		num=1
		answer="NONE"
		for n in ${LUNCH_TOOLCHAIN_MENU_CHOICES[@]}
		do
			if [ "$3" == "$n" ]; then
				answer=$num
				break
			fi
			num=$((num + 1))
		done
	else
		print_lunch_toolchain_menu
		echo -n "Which toolchain would you like? [${NVT_DEFAULT_NVT_CROSS}] "
		read answer
	fi

	local selection=

	if [ -z "$answer" ]
	then
		selection=${NVT_DEFAULT_NVT_CROSS}
	elif (echo -n $answer | grep -q -e "^[0-9][0-9]*$")
	then
		if [ $answer -le ${#LUNCH_TOOLCHAIN_MENU_CHOICES[@]} ]
		then
			selection=${LUNCH_TOOLCHAIN_MENU_CHOICES[$(($answer-1))]}
		fi
	elif (echo -n $answer | grep -q -e "^[^\-][^\-]*-[^\-][^\-]*$")
	then
		selection=$answer
	fi

	if [ -z "$selection" ]
	then
		echo
		echo "Invalid lunch toolchain combo: $answer"
		return 1
	fi

	export NVT_CROSS=${selection}
	echo -e "\e[1;33mToolchain: ${NVT_CROSS}\e[0m"
	echo -e "\e[1;33mProject: ${NVT_PRJCFG_CFG}\e[0m"
	echo -e "\e[1;33mModel: ${NVT_PRJCFG_MODEL_CFG}\e[0m"
	gen_modelconfig
	setpaths
}

# Tab completion for lunch.
function _lunch()
{
	local cur prev opts
	COMPREPLY=()
	cur="${COMP_WORDS[COMP_CWORD]}"
	prev="${COMP_WORDS[COMP_CWORD-1]}"

	COMPREPLY=( $(compgen -W "${LUNCH_MENU_CHOICES[*]}" -- ${cur}) )
	return 0
}
complete -F _lunch lunch

function addcompletions()
{
	local T dir f

	# Keep us from trying to run in something that isn't bash.
	if [ -z "${BASH_VERSION}" ]; then
		return
	fi

	# Keep us from trying to run in bash that's too old.
	if [ ${BASH_VERSINFO[0]} -lt 3 ]; then
		return
	fi

	dir="sdk/bash_completion"
	if [ -d ${dir} ]; then
		for f in `/bin/ls ${dir}/[a-z]*.bash 2> /dev/null`; do
			echo "including $f"
			. $f
		done
	fi
}

function croot()
{
	T=$(gettop)
	if [ "$T" ]; then
		\cd $(gettop)
	else
		echo "Couldn't locate the top of the tree.  Try setting TOP."
	fi
}

function m()
{
	local T=$(gettop)
	if [ "$T" ]; then
		cd $(gettop);make;cd -
	else
		echo "Couldn't locate the top of the tree.  Try setting TOP."
	fi
}

function upgrade_kernel()
{
	export KERNELDIR=${LINUX_BUILD_TOP}/linux-kernel
}

function mr()
{
	local T=$(gettop)
	if [ "$T" ]; then
		cd $(gettop);make rootfs;cd -
	else
		echo "Couldn't locate the top of the tree.  Try setting TOP."
	fi
}

function cso()
{
	file=$(gettop)/build/copyso.sh
	sh $file
}
 
# add the default one here
add_lunch_toolchain_combo arm-ca53-linux-gnueabihf-4.9-2017.05

if [ "x$SHELL" != "x/bin/bash" ]; then
	case `ps -o command -p $$` in
		*bash*)
			;;
		*)
			echo "WARNING: Only bash is supported, use of other shell would lead to erroneous results"
			;;
	esac
fi

if [ -f ${PWD}/build/envsetup.sh ]; then
	#addcompletions
	set_stuff_for_environment
	echo "Env setup configured"
else
	echo "Can't support execute env setup on ${PWD}"
fi
