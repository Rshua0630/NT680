#!/bin/bash
# Program:
#    Copy necessary shared libraries from ToolChain to rootfs automatically
# History:
# 2016/11/11      Wayne Lin Modified for rootfs path
# 2014/01/23      Lincy Lin First release

MY_ROOTFS_PATH=$1

copylinkfile()
{
	# use readlink command to get the link path
	source_releative_path=""
	link_source=$(readlink -f $1)
	#echo $link_source
	if [ ! -z "`echo $link_source | grep "/sysroot/usr/lib"`" ]; then
		list=`echo $link_source | awk -F'/' '{for(i=NF;i>0;i--) { if ($i=="sysroot")  {for (n=i+1; n<=NF; n++) {print $n;} break;}}}'`
	else
		list=`echo $link_source | awk -F'/' '{for(i=NF;i>0;i--) { if ($i=="lib")  {for (n=i; n<=NF; n++) {print $n;} break;}}}'`
	fi
	#echo $list
	for n in $list; do source_releative_path="$source_releative_path/$n"; done
	#echo source_releative_path $source_releative_path
	destination=${MY_ROOTFS_PATH}/$source_releative_path
	#echo destination $destination
	if [ -f "$link_source" ] && [ ! -f "$destination" ]; then
		cp -af $link_source $destination
		copysoReturnList="$copysoReturnList $destination"
	fi
	# check if source file modify time is newer than destination file
	if [ -f "$link_source" ] && [ -f "${destination}" ] && [ -z "$issysrootfile" ]; then
		smodifytime=$(stat -c "%Y" $link_source)
		dmodifytime=$(stat -c "%Y" $destination)
		#echo smodifytime $smodifytime $link_source
		#echo dmodifytime $dmodifytime $destination
		if [ $smodifytime -gt $dmodifytime ]; then
			cp -af $link_source $destination
			copysoReturnList="$copysoReturnList $destination"
		fi
	fi
	#echo "copy $link_source to $destination"
	ReturnValue=$link_source
}

copyso()
{
	# use awk to get the *.so path
	if [ "$2" != "necessary" ] ; then
		#filelist=$(${LDD} "$1" | awk '{print $1}' | grep 'so')
		filelist="$(readelf -l $1 | grep "Requesting program interpreter" | awk -F'Requesting program interpreter: ' '{print $NF}' | sed 's/\[//g' | sed 's/\]//g') \
			$(readelf -d $1 | grep "(NEEDED)" | awk -F'[' '{print $NF}' | awk -F']' '{print $1}' | sed 's/\[//g' | sed 's/\]//g')"
	else
		filelist=$1
	fi
	#copysoReturnList=""
	for filename in $filelist
	do
	  #echo filename = $filename
		# sysroot lib
		if [ -f "${SYSROOT_PATH}/lib/${filename}" ] ; then
			sourcefile=${SYSROOT_PATH}/lib/${filename}
			dstfile=${MY_ROOTFS_PATH}/lib/${filename}
			issysrootfile="1"
		else
			# sysroot usr/lib
			if [ -f "${SYSROOT_PATH}/usr/lib/${filename}" ] ; then
				sourcefile=${SYSROOT_PATH}/usr/lib/${filename}
				dstfile=${MY_ROOTFS_PATH}/usr/lib/${filename}
				issysrootfile="1"
			else
				sourcefile=${LIBRARY_DIR}/${filename}
				dstfile=${MY_ROOTFS_PATH}/lib/${filename}
				issysrootfile=""
			fi
		fi
		#echo sourcefile $sourcefile
		#echo dstfile $dstfile
		# if file is a link file , need to copy the original file also
		if [ -L "$sourcefile" ] ; then
			copylinkfile $sourcefile $issysrootfile
			if [ -L $ReturnValue ] ; then
				copylinkfile $ReturnValue $issysrootfile
			fi
		fi

		#dstfile=${MY_ROOTFS_PATH}/lib/${filename}
		if [ -f "$sourcefile" ] && [ ! -f "${dstfile}" ]; then
			cp -avf $sourcefile $dstfile
			copysoReturnList="$copysoReturnList $dstfile"
		fi
		# check if source file modify time is newer than destination file

		if [ -z "$issysrootfile" ] && [ -f "$sourcefile" ] && [ -f "${dstfile}" ]; then
			smodifytime=$(stat -c "%Y" $sourcefile)
			dmodifytime=$(stat -c "%Y" $dstfile)
			#echo smodifytime $smodifytime $sourcefile
			#echo dmodifytime $dmodifytime $dstfile
			if [ $smodifytime -gt $dmodifytime ]; then
				cp -avf $sourcefile $dstfile
				copysoReturnList="$copysoReturnList $dstfile"
			fi
		fi
	done
	#echo copysoReturnList=${copysoReturnList}
}
 
copyallso()
{
	# 1. Search bin use shared library  - layer1
	#search bin file except link file
	#echo $1
	searchpath=$1
	if [ -e "${MY_ROOTFS_PATH}/$searchpath" ] ; then
		echo $searchpath
		binlist=$(ls ${MY_ROOTFS_PATH}/$searchpath)
		for binname in $binlist
		do
			binPath=${MY_ROOTFS_PATH}/$searchpath/${binname}
			if [ ! -L "$binPath" ]; then
				#echo binPath=$binPath
				bininfo=$(file $binPath | awk '{print $2}')
				#echo bininfo=$bininfo
				if [ "$bininfo" = "ELF" ] ; then
					#echo ${binPath}
					copyso $binPath
				fi
			fi
		done
	fi
}

searchNewAddedlib()
{
	#echo liblistAdd=${liblistAdd}
	liblist=${liblistAdd}
	liblistAdd=""
	for libPath in $liblist
	do
		copysoReturnList=""
		copyso $libPath
		liblistAdd="$liblistAdd $copysoReturnList"
	done
}

copylibusedso()
{
	#search lib use shared library  - layer2
	#search lib file except link file
	
	#sysroot lib
	liblist=$(ls ${MY_ROOTFS_PATH}/lib)
	#test
	#liblist="libnvtauth.so "
	liblistAdd=""
	for libname in $liblist
	do
		#echo libname=${libname}
		libPath=${MY_ROOTFS_PATH}/lib/${libname}
		if [ ! -L "$libPath" ] && [ -f "$libPath" ] ; then
			libinfo=$(file $libPath | awk '{print $2}')
			#echo bininfo=$bininfo
			if [ "$libinfo" = "ELF" ] ; then
	    			#echo libPath=${libPath}
	    			copysoReturnList=""
				copyso $libPath
				liblistAdd="$liblistAdd $copysoReturnList"
			fi
		fi
	done
	
	liblist=""
	#sysroot usr/lib
	liblist=$(ls ${MY_ROOTFS_PATH}/usr/lib)
	for libname in $liblist
	do
		#echo libname=${libname}
		libPath=${MY_ROOTFS_PATH}/usr/lib/${libname}
		if [ ! -L "$libPath" ] && [ -f "$libPath" ] ; then
			libinfo=$(file $libPath | awk '{print $2}')
			#echo bininfo=$bininfo
			if [ "$libinfo" = "ELF" ] ; then
	    			#echo libPath=${libPath}
	    			copysoReturnList=""
				copyso $libPath
				liblistAdd="$liblistAdd $copysoReturnList"
			fi
		fi
	done

	# search for new added library
	while [ ! -z "$liblistAdd" ]; do
		searchNewAddedlib
	done
}

copyNecessaryso()
{
	#search lib use shared library  - layer2
	#search lib file except link file
	uclibc=$(echo ${CROSS_COMPILE} | grep 'uclibc')
	#echo glibc = $glibc
	if [ ! -z "$uclibc" ]; then
		liblist=$(cat ${LIBRARY_DIR}/necessary/uclibc/liblist)
	else
		liblist=$(cat ${LIBRARY_DIR}/necessary/glibc/liblist)
	fi
	#echo $liblist
	for libname in $liblist
	do
		copyso $libname necessary
	done
}

# ld shared library is always need for dynamic link
copy_ld_so()
{
	cp -avu ${SYSROOT_PATH}/lib/ld* ${MY_ROOTFS_PATH}/lib/
}

echo -e "copyso search bin path (\e[1;33m$1\e[0m) ==> " $@ | awk -F"$1 " '{print $1$NF}'
while [ "$(($#-1))" -gt 0 ]; do
        copyallso $2
        shift
done
echo -e "\e[1;33m\rlib\e[0m"
copylibusedso
echo -e "\e[1;33m\rnecessary\e[0m"
copyNecessaryso
copy_ld_so
