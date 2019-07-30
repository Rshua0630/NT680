#!/bin/sh
#
# For Bot to prepare release file
#

ROOT_DIR=$PWD
RELEASE_FOLDER=${ROOT_DIR}/out
HEADER_FOLDER=linux-4.1_header
KERNEL_HEADER=${RELEASE_FOLDER}/${HEADER_FOLDER}
MODULE_FOLDER=modules
MODULE_PATH=${RELEASE_FOLDER}/${MODULE_FOLDER}

packing_header()
{
	echo "Packing headers ..."
	mkdir -p ${KERNEL_HEADER}/arch
	cp -rf arch/${ARCH} ${KERNEL_HEADER}/arch
	cp -a usr drivers include init Kbuild Makefile modules.order \
		Module.symvers scripts .config ${KERNEL_HEADER}
	find ${KERNEL_HEADER} -name "*.o" | xargs rm -rf
	find ${KERNEL_HEADER} -name "*.svn" | xargs rm -rf
	find ${KERNEL_HEADER}/arch -name "*.c" | xargs rm -rf
	find ${KERNEL_HEADER}/drivers -name "*.c" | xargs rm -rf
	find ${KERNEL_HEADER}/init -name "*.c" | xargs rm -rf
	find ${KERNEL_HEADER} -name "*.cmd" -o -name "*.o" \
		-o -name "modules.builtin" -o -name "modules.order" \
		-o -name "*.ko" -o -name "*.a" | xargs rm -rf
	rm -rf ${KERNEL_HEADER}/arch/${ARCH}/boot/*
	cp -rf arch/${ARCH}/boot/dts ${KERNEL_HEADER}/arch/${ARCH}/boot/dts
	tar -czf ${KERNEL_HEADER}.tgz -C ${RELEASE_FOLDER} ${HEADER_FOLDER} \
		|| exit 1
	rm -rf ${KERNEL_HEADER}
}
packing_modules()
{
	echo "Packing modules..."
	mkdir -p ${MODULE_PATH}
	find -name "*.ko" | xargs -i cp {} ${MODULE_PATH}
	tar -czf ${MODULE_PATH}_no_strip.tgz -C ${RELEASE_FOLDER} \
		 ${MODULE_FOLDER} || exit 1
	echo "Strip all modules..."
	$STRIP -d ${MODULE_PATH}/*
	tar -czf ${MODULE_PATH}.tgz -C ${RELEASE_FOLDER} ${MODULE_FOLDER} \
		|| exit 1
	rm -rf ${MODULE_PATH}
}
packing_others()
{
	echo "Packing uImage..."
	if [ -e arch/${ARCH}/boot/uImage.ngz ]; then
		cp arch/${ARCH}/boot/uImage.ngz ${RELEASE_FOLDER}
	else
		cp arch/${ARCH}/boot/uImage ${RELEASE_FOLDER}
	fi
	echo "Packing dtb..."
	cp arch/${ARCH}/boot/dts/*.dtb ${RELEASE_FOLDER}
	echo "Packing vmlinux..."
	cp vmlinux ${RELEASE_FOLDER}
}

cleanup()
{
	rm -rf ${RELEASE_FOLDER}
}

if [ "$1" = "packing" ]; then
	echo Prepare files...
	if [ -z $ARCH ];then
		echo No export ARCH=?
		exit 1;
	fi
	cleanup
	mkdir -p ${RELEASE_FOLDER}
	packing_header
	packing_modules
	packing_others
	exit 0
fi

if [ "$1" = "clean" ]; then
	cleanup
	exit 0
fi
