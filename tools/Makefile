MULTI_CORES ?= $(shell grep -c ^processor /proc/cpuinfo)
TOOLS = ${PWD}
SHELL = /bin/bash
INSTALL_DIR = __install

ZLIB := zlib-1.2.8
LZO := lzo-2.08
MTD_UTILS := mtd-utils-9f10713
MEMTESTER := memtester-4.3.0
BONNIE := bonnie++-1.03e
ETHTOOL := ethtool-3.18
GDB := gdb-7.10.1
TERMCAP := termcap-1.3.1
NCURSES := ncurses-5.9
HTOP := htop-1.0.3
PROCPS := procps-3.2.8
SDCARD_TEST := sdcard_test.sh
I2C_TOOL := i2c-tools-3.1.2
STRESS := stress-1.0.4
STRESSNG := stress-ng-0.07.09
NETSTRESS := netstress_1.2.0

unexport CC
unexport CPP
unexport CXX
unexport LD
unexport AR
unexport NM
unexport STRIP
unexport OBJCOPY
unexport OBJDUMP

CC="${CROSS_COMPILE}gcc"
CXX="${CROSS_COMPILE}g++"
CPP="${CROSS_COMPILE}gcc"

check_exist = if [ ! -e $(1) ]; then tar -jxvf $(1).tar.bz2; fi
check_exist_cp = if [ -e $(INSTALL_DIR)/$(1) ]; then file_list="`cd $(INSTALL_DIR)/$(1); ls`"; \
		for file in $$file_list; do \
		if [ -f ${ROOTFS_DIR}/rootfs/$(1)/$$file ]; then \
			rm -rf ${ROOTFS_DIR}/rootfs/$(1)/$$file; \
		fi; done; \
		cp -arfv $(INSTALL_DIR)/$(1)/* ${ROOTFS_DIR}/rootfs/$(1); fi

.PHONY: memtester bonnie ethtool
all: memtester bonnie ethtool

init:
	@if [ ! -e "${TOOLS}/${INSTALL_DIR}" ]; then \
		mkdir ${TOOLS}/${INSTALL_DIR}; \
	fi
	@if [ -z "${CROSS_COMPILE}" ]; then \
		echo "Please execute \"source build/envsetup.sh\" firstly.  Stop."; exit 1; \
	fi

zlib: init
	@echo ">>>>>>>>>>>>>>>>>>> $@ compiling >>>>>>>>>>>>>>>>>>>"
	$(call check_exist, ${ZLIB})
	@cd ${ZLIB}; prefix=${TOOLS}/${INSTALL_DIR} CC=${CC} CFLAGS="-O4" ./configure; make; make install;

lzo: init
	@echo ">>>>>>>>>>>>>>>>>>> $@ compiling >>>>>>>>>>>>>>>>>>>"
	$(call check_exist, ${LZO})
	@cd ${LZO}; ./configure --prefix=${TOOLS}/${INSTALL_DIR} CC=${CC} --host=${NVT_HOST}; make; make install;

mtd-utils: zlib lzo
	@echo ">>>>>>>>>>>>>>>>>>> $@ compiling >>>>>>>>>>>>>>>>>>>"
	$(call check_exist, ${MTD_UTILS})
	@cd ${MTD_UTILS}; CROSS=${CROSS_COMPILE} PREFIX=${TOOLS}/${INSTALL_DIR} WITHOUT_XATTR=1 \
	ZLIBCPPFLAGS=-I${TOOLS}/${INSTALL_DIR}/include ZLIBLDFLAGS=-L${TOOLS}/${INSTALL_DIR}/lib \
	LZOLDFLAGS=-L${TOOLS}/${INSTALL_DIR}/lib make;
	cd ${MTD_UTILS}; CROSS=${CROSS_COMPILE} PREFIX=${TOOLS}/${INSTALL_DIR} WITHOUT_XATTR=1 \
	ZLIBCPPFLAGS=-I${TOOLS}/${INSTALL_DIR}/include ZLIBLDFLAGS=-L${TOOLS}/${INSTALL_DIR}/lib \
	LZOLDFLAGS=-L${TOOLS}/${INSTALL_DIR}/lib DESTDIR=${TOOLS}/${INSTALL_DIR} \
	SBINDIR=${TOOLS}/${INSTALL_DIR}/usr/sbin MANDIR=${TOOLS}/${INSTALL_DIR}/usr/share/man make install;

memtester: init
	@echo ">>>>>>>>>>>>>>>>>>> $@ compiling >>>>>>>>>>>>>>>>>>>"
	$(call check_exist, ${MEMTESTER})
	@cd ${MEMTESTER}; echo "${CC} -O2 -DPOSIX -D_POSIX_C_SOURCE=200809L -D_FILE_OFFSET_BITS=64 -DTEST_NARROW_WRITES -c" > conf-cc; echo "${CC} -s" > conf-ld; make;
	@cd ${MEMTESTER}; mkdir ${TOOLS}/${INSTALL_DIR}/bin/; cp -arvf memtester ${TOOLS}/${INSTALL_DIR}/bin/;

bonnie: init
	@echo ">>>>>>>>>>>>>>>>>>> $@ compiling >>>>>>>>>>>>>>>>>>>"
	$(call check_exist, ${BONNIE})
	@cd ${BONNIE}; ./configure --host=${NVT_HOST} --prefix=${TOOLS}/${INSTALL_DIR}; make; mkdir -p ${TOOLS}/${INSTALL_DIR}/sbin/; cp bonnie++ zcav ${TOOLS}/${INSTALL_DIR}/sbin/;

ethtool: init
	@echo ">>>>>>>>>>>>>>>>>>> $@ compiling >>>>>>>>>>>>>>>>>>>"
	$(call check_exist, ${ETHTOOL})
	@cd ${ETHTOOL}; ./configure --host=${NVT_HOST} --prefix=${TOOLS}/${INSTALL_DIR}; make; make install;

gdb: termcap ncurses
	@echo ">>>>>>>>>>>>>>>>>>> $@ compiling >>>>>>>>>>>>>>>>>>>"
	$(call check_exist, ${GDB})
	@cd ${GDB}; ./configure LDFLAGS="-L${TOOLS}/${INSTALL_DIR}/lib" CXXFLAGS="-I${TOOLS}/${INSTALL_DIR}/include" --host=${NVT_HOST} --prefix=${TOOLS}/${INSTALL_DIR} --without-x --disable-gdbtk --disable-tui --without-included-regex --without-included-gettext; make -j$(MULTI_CORES); make install

termcap: init
	@echo ">>>>>>>>>>>>>>>>>>> $@ compiling >>>>>>>>>>>>>>>>>>>"
	$(call check_exist, ${TERMCAP})
	@cd ${TERMCAP}; ./configure --host=${NVT_HOST} --prefix=${TOOLS}/${INSTALL_DIR}; make -j$(MULTI_CORES); make install;

ncurses: init
	@echo ">>>>>>>>>>>>>>>>>>> $@ compiling >>>>>>>>>>>>>>>>>>>"
	$(call check_exist, ${NCURSES})
	@cd ${NCURSES}; ./configure LDFLAGS="-L${TOOLS}/${INSTALL_DIR}/lib" CXXFLAGS="-I${TOOLS}/${INSTALL_DIR}/include" --host=${NVT_HOST} --prefix=${TOOLS}/${INSTALL_DIR}; make -j$(MULTI_CORES); make install;

htop: ncurses
	@echo ">>>>>>>>>>>>>>>>>>> $@ compiling >>>>>>>>>>>>>>>>>>>"
	$(call check_exist, ${HTOP})
	@cd ${HTOP}; ./configure --host=${NVT_HOST} --prefix=${TOOLS}/${INSTALL_DIR} --disable-unicode LDFLAGS="-static -L${TOOLS}/${INSTALL_DIR}/lib" CPPFLAGS="-I${TOOLS}/${INSTALL_DIR}/include" CFLAGS="-I${TOOLS}/${INSTALL_DIR}/include"; make; make install;

procps: ncurses
	@echo ">>>>>>>>>>>>>>>>>>> $@ compiling >>>>>>>>>>>>>>>>>>>"
	$(call check_exist, ${PROCPS})
	@cd ${PROCPS}; make lib64=lib SHARED=0 CFLAGS="-I${TOOLS}/${INSTALL_DIR}/include/ncurses -I${TOOLS}/${INSTALL_DIR}/include" \
			DESTDIR=${TOOLS}/${INSTALL_DIR} CC=${CC} LDFLAGS="-L${TOOLS}/${INSTALL_DIR}/lib" install="install -D" install

mmctest: init
	@echo ">>>>>>>>>>>>>>>>>>> $@ compiling >>>>>>>>>>>>>>>>>>>"
	@mkdir -p ${TOOLS}/${INSTALL_DIR}/usr/bin/; cp ${SDCARD_TEST} ${TOOLS}/${INSTALL_DIR}/usr/bin/

i2c_tool: init
	@echo ">>>>>>>>>>>>>>>>>>> $@ compiling >>>>>>>>>>>>>>>>>>>"
	$(call check_exist, ${I2C_TOOL})
	@cd ${I2C_TOOL}; CC=${CC} make; prefix=${TOOLS}/${INSTALL_DIR}/usr make install;

stress: init
	@echo ">>>>>>>>>>>>>>>>>>> $@ compiling >>>>>>>>>>>>>>>>>>>"
	$(call check_exist, ${STRESS})
	@cd ${STRESS}; ./configure --prefix=${TOOLS}/${INSTALL_DIR} --host=${NVT_HOST}; make; make install;

stress-ng: init
	@echo ">>>>>>>>>>>>>>>>>>> $@ compiling >>>>>>>>>>>>>>>>>>>"
	$(call check_exist, ${STRESSNG})
	@cd ${STRESSNG}; make CC=${CC} HAVE_LIB_AIO=0 HAVE_SYS_CAP_H=0 HAVE_FLOAT_DECIMAL=0 HAVE_SECCOMP_H=0 HAVE_LIB_Z=0 DESTDIR=${TOOLS}/${INSTALL_DIR} install;

netstress: init
	@echo ">>>>>>>>>>>>>>>>>>> $@ compiling >>>>>>>>>>>>>>>>>>>"
	$(call check_exist, ${NETSTRESS})
	@cd ${NETSTRESS}; mkdir -p ${TOOLS}/${INSTALL_DIR}/bin/; ${CC} -o ${TOOLS}/${INSTALL_DIR}/bin/$@ netstress.c

install:
	@echo ">>>>>>>>>>>>>>>>>>> $@ >>>>>>>>>>>>>>>>>>>"
	@$(call check_exist_cp,bin)
	@$(call check_exist_cp,sbin)
	@$(call check_exist_cp,usr/bin)
	@$(call check_exist_cp,usr/sbin)
	@$(call check_exist_cp,usr/lib)
	@$(call check_exist_cp,lib)

clean:
	@echo ">>>>>>>>>>>>>>>>>>> Remove >>>>>>>>>>>>>>>>>>>"
	@rm -rf ${TOOLS}/${INSTALL_DIR}
	@rm -rf ${ZLIB} ${LZO} ${MTD_UTILS} ${MEMTESTER} ${BONNIE} ${ETHTOOL} ${TERMCAP} ${GDB} ${NCURSES} ${HTOP} ${IPERF} ${PROCPS} ${I2C_TOOL} ${STRESS} ${STRESSNG} ${PROCPS} ${NETSTRESS}
