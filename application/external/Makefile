include $(NVT_PRJCFG_MODEL_CFG)

EXTERNAL := ${APP_DIR}/external
SHELL = /bin/bash
INSTALL_DIR = __install

NTP  := ntp-4.2.8p4
ISCSI:= open-iscsi-2.0-873
TZCODE := tzcode
IPERF := iperf-2.0.5
IPERF3 := iperf-3.0.12
WIRELESS_TOOL := wireless_tools.29
ifeq ($(NVT_SDIO_WIFI), $(filter $(NVT_SDIO_WIFI),NVT_SDIO_WIFI_RTK))
HOSTAPD := hostapd-0.8
WPA_SUPPLICANT := wpa_supplicant-0.6.10
else
HOSTAPD := hostapd-2.5
WPA_SUPPLICANT := wpa_supplicant-2.5
endif
DHD_PRIV := dhd_priv
QUECTEL_CM := quectel-CM
ZTE_CONNECT := zte_connect
E2FSPROGS := e2fsprogs-1.43.3
IPTABLES := iptables-1.6.0
EXFAT_UTILS := exfat-utils-1.2.7
NVT_REMOTE_FW := nvt_remote_fw

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

check_exist = if [ ! -e $(1)/.nvt_finish ]; then rm -rf $(1); tar -jxvf $(1).tar.bz2; fi
check_exist_cp_bin = if [ -e $(INSTALL_DIR)/$(1) ]; then mkdir -p ${ROOTFS_DIR}/rootfs/$(1); cp -arfv $(INSTALL_DIR)/$(1)/* ${ROOTFS_DIR}/rootfs/$(1); fi
check_exist_cp_lib = if [ -e $(INSTALL_DIR)/$(1) ]; then cp -arfv $(INSTALL_DIR)/$(1)/* $(LIBRARY_DIR); fi
check_exist_cp_etc = if [ -e $(INSTALL_DIR)/$(1) ]; then mkdir -p ${ROOTFS_DIR}/rootfs/$(1); cp -narv $(INSTALL_DIR)/$(1)/* ${ROOTFS_DIR}/rootfs/$(1); fi
check_exist_lib_rm = if [ -e $(INSTALL_DIR)/lib/$(1) ]; then rm -rf $(INSTALL_DIR)/lib/$(1); fi

.PHONY: $(NVT_CFG_APP_EXTERNAL)
all: $(NVT_CFG_APP_EXTERNAL)

list:
	@echo $(NVT_CFG_APP_EXTERNAL)

init:
	@if [ ! -e "${EXTERNAL}/${INSTALL_DIR}" ]; then \
		mkdir ${EXTERNAL}/${INSTALL_DIR}; \
	fi
	@if [ -z "${CROSS_COMPILE}" ]; then \
		echo "Please execute \"source build/envsetup.sh\" firstly.  Stop."; exit 1; \
	fi

ntp: init
	@echo ">>>>>>>>>>>>>>>>>>> $@ compiling >>>>>>>>>>>>>>>>>>>"
	@if [ ! -f ${NTP}/.nvt_finish ]; then \
		$(call check_exist, ${NTP}); \
		cd ${NTP}; ./configure CPPFLAGS="-I${EXTERNAL}/${INSTALL_DIR}/include -I${INCLUDE_DIR}" LDFLAGS="-L${LIBRARY_DIR} -L${EXTERNAL}/${INSTALL_DIR}/lib" --without-crypto --with-yielding-select=yes --prefix=${EXTERNAL}/${INSTALL_DIR} --host=${NVT_HOST}; make; \
	fi
	@cd ${NTP}; make install;
	@cd ${EXTERNAL}/${INSTALL_DIR}/bin;${CROSS_COMPILE}strip ntpdate;rm ntpd;rm ntpdc;rm ntp-keygen;rm ntpq;rm ntptime;rm ntptrace;rm ntp-wait;rm sntp;rm tickadj;rm update-leap;
	@touch ${NTP}/.nvt_finish

iscsi: init
	@echo ">>>>>>>>>>>>>>>>>>> $@ compiling >>>>>>>>>>>>>>>>>>>"
	@if [ ! -f ${ISCSI}/.nvt_finish ]; then \
		$(call check_exist, ${ISCSI}); \
		cd ${ISCSI}/utils/open-isns; ./configure --host=${NVT_HOST} --with-security=no; \
		cd ${EXTERNAL}/${ISCSI}; make CC=${CROSS_COMPILE}gcc; mkdir -p ${EXTERNAL}/${INSTALL_DIR}/etc/iscsi; \
	fi
	@cd ${ISCSI}; make install DESTDIR=${EXTERNAL}/${INSTALL_DIR};
	@cd ${EXTERNAL}/${INSTALL_DIR}/sbin; rm iscsi-iname; rm iscsi_discovery;
	@touch ${ISCSI}/.nvt_finish

tzcode: init
	@echo ">>>>>>>>>>>>>>>>>>> $@ compiling >>>>>>>>>>>>>>>>>>>"
	@cd ${TZCODE}; make CC=${CC} NM=${NM} STRIP=${STRIP} OBJCOPY=${OBJCOPY}; make install;

iperf: init
	@echo ">>>>>>>>>>>>>>>>>>> $@ compiling >>>>>>>>>>>>>>>>>>>"
	@if [ ! -f ${IPERF}/.nvt_finish ]; then \
		$(call check_exist, ${IPERF}); \
		cd ${IPERF}; ./configure  --host=${NVT_HOST} --build=i686-linux --prefix=${EXTERNAL}/${INSTALL_DIR}; make; \
	fi
	@cd ${IPERF}; make install;
	@touch ${IPERF}/.nvt_finish

iperf3: init
	@echo ">>>>>>>>>>>>>>>>>>> $@ compiling >>>>>>>>>>>>>>>>>>>"
	@if [ ! -f ${IPERF3}/.nvt_finish ]; then \
		$(call check_exist, ${IPERF3}); \
		cd ${IPERF3}; ./configure  --host=${NVT_HOST} --build=i686-linux --prefix=${EXTERNAL}/${INSTALL_DIR}; make; \
	fi
	@cd ${IPERF3}; make install;
	@touch ${IPERF3}/.nvt_finish

wireless_tool: init
	@echo ">>>>>>>>>>>>>>>>>>> $@ compiling >>>>>>>>>>>>>>>>>>>"
	@if [ ! -f ${WIRELESS_TOOL}/.nvt_finish ]; then \
		$(call check_exist, ${WIRELESS_TOOL}); \
		cd ${WIRELESS_TOOL}; make PREFIX=${EXTERNAL}/${INSTALL_DIR} CC=${CC} AR=${AR}; \
	fi
	@cd ${WIRELESS_TOOL}; make PREFIX=${EXTERNAL}/${INSTALL_DIR} CC=${CC} AR=${AR} install-bin install-dynamic;
	@touch ${WIRELESS_TOOL}/.nvt_finish

wpa_supplicant: init
	@echo ">>>>>>>>>>>>>>>>>>> $@ compiling >>>>>>>>>>>>>>>>>>>"
	@if [ ! -f ${WPA_SUPPLICANT}/.nvt_finish ]; then \
		$(call check_exist, ${WPA_SUPPLICANT}); \
		cd ${WPA_SUPPLICANT}/wpa_supplicant; make CC=${CC} LD=${LD} AR=${AR}; \
	fi
	rm -rf ${EXTERNAL}/${INSTALL_DIR}/tmp/
	cd ${WPA_SUPPLICANT}/wpa_supplicant; make DESTDIR=${EXTERNAL}/${INSTALL_DIR}/tmp BINDIR=/bin install;
	cp -arfv ${EXTERNAL}/${INSTALL_DIR}/tmp/* ${EXTERNAL}/${INSTALL_DIR}/
	@echo ${WPA_SUPPLICANT} > ${WPA_SUPPLICANT}/.nvt_finish

hostapd: init
	@echo ">>>>>>>>>>>>>>>>>>> $@ compiling >>>>>>>>>>>>>>>>>>>"
	@if [ ! -f ${HOSTAPD}/.nvt_finish ]; then \
		$(call check_exist, ${HOSTAPD}); \
		cd ${HOSTAPD}/hostapd; make CC=${CC} LD=${LD}; \
	fi
	rm -rf ${EXTERNAL}/${INSTALL_DIR}/tmp/
	cd ${HOSTAPD}/hostapd; make DESTDIR=${EXTERNAL}/${INSTALL_DIR}/tmp BINDIR=/usr/bin install;
	cp -arfv ${EXTERNAL}/${INSTALL_DIR}/tmp/* ${EXTERNAL}/${INSTALL_DIR}/
	@echo ${HOSTAPD} > ${HOSTAPD}/.nvt_finish

quectel-cm: init
	@echo ">>>>>>>>>>>>>>>>>>> $@ compiling >>>>>>>>>>>>>>>>>>>"
	$(call check_exist, ${QUECTEL_CM})
	@cd ${QUECTEL_CM}; make CC=${CC} NM=${NM} STRIP=${STRIP} OBJCOPY=${OBJCOPY}; make install;

e2fsprogs: init
	@echo ">>>>>>>>>>>>>>>>>>> $@ compiling >>>>>>>>>>>>>>>>>>>"
	@if [ ! -f ${E2FSPROGS}/.nvt_finish ]; then \
		$(call check_exist, ${E2FSPROGS}); \
		cd ${E2FSPROGS}; ./configure  --host=${NVT_HOST} --prefix=${EXTERNAL}/${INSTALL_DIR} \
			--disable-debugfs --disable-nls; make;\
	fi
	@mkdir -p ${EXTERNAL}/${INSTALL_DIR}/sbin/;
	@cd ${E2FSPROGS}; cp -f ./misc/mke2fs ${EXTERNAL}/${INSTALL_DIR}/sbin/mkfs.ext4;
	@${CROSS_COMPILE}strip ${EXTERNAL}/${INSTALL_DIR}/sbin/mkfs.ext4;
	@touch ${E2FSPROGS}/.nvt_finish

iptables: init
	@echo ">>>>>>>>>>>>>>>>>>> $@ compiling >>>>>>>>>>>>>>>>>>>"
	@if [ ! -f ${IPTABLES}/.nvt_finish ]; then \
		$(call check_exist, ${IPTABLES}); \
		cd ${IPTABLES}; mkdir output; ./configure  --disable-ipv6 --disable-nftables --disable-largefile --enable-libipq --disable-devel --host=${NVT_HOST} --with-xtlibdir=/lib/xtables; make; \
	fi
	@cd ${IPTABLES}; make install DESTDIR=${EXTERNAL}/${IPTABLES}/output; cd output/; cp -arfv usr ${EXTERNAL}/${INSTALL_DIR}; cp -arfv lib ${ROOTFS_DIR}/rootfs/;
	@touch ${IPTABLES}/.nvt_finish

exfat-utils: init
	@echo ">>>>>>>>>>>>>>>>>>> $@ compiling >>>>>>>>>>>>>>>>>>>"
	@if [ ! -f ${EXFAT_UTILS}/.nvt_finish ]; then \
		$(call check_exist, ${EXFAT_UTILS}); \
		cd ${EXFAT_UTILS}; ./configure  --host=${NVT_HOST} --prefix=${EXTERNAL}/${INSTALL_DIR}; make; \
	fi
	@mkdir -p ${EXTERNAL}/${INSTALL_DIR}/sbin/;
	@cd ${EXFAT_UTILS}; cp -f ./mkfs/mkexfatfs ${EXTERNAL}/${INSTALL_DIR}/sbin/;
	@${CROSS_COMPILE}strip ${EXTERNAL}/${INSTALL_DIR}/sbin/mkexfatfs;
	@touch ${EXFAT_UTILS}/.nvt_finish

dhd_priv: init
	@echo ">>>>>>>>>>>>>>>>>>> $@ compiling >>>>>>>>>>>>>>>>>>>"
	@if [ ! -f ${DHD_PRIV}/.nvt_finish ]; then \
		$(call check_exist, ${DHD_PRIV}); \
		cd ${DHD_PRIV}; make CC=${CC} LD=${LD}; \
	fi
	@mkdir -p ${EXTERNAL}/${INSTALL_DIR}/usr/local/bin;
	@cp -r ${DHD_PRIV}/dhd_priv ${EXTERNAL}/${INSTALL_DIR}/usr/local/bin;
	@touch ${DHD_PRIV}/.nvt_finish

nvt_remote_fw: init
	@echo ">>>>>>>>>>>>>>>>>>> $@ compiling >>>>>>>>>>>>>>>>>>>"
	@if [ ! -f ${NVT_REMOTE_FW}/.nvt_finish ]; then \
		$(call check_exist, ${NVT_REMOTE_FW}); \
		cd ${NVT_REMOTE_FW}; make CC=${CC} LD=${LD}; \
	fi
	@mkdir -p ${EXTERNAL}/${INSTALL_DIR}/usr/local/bin;
	@cp -r ${NVT_REMOTE_FW}/nvtfwcmd.out ${EXTERNAL}/${INSTALL_DIR}/usr/local/bin;
	@cp -r ${NVT_REMOTE_FW}/nvtfwlog_server.out ${EXTERNAL}/${INSTALL_DIR}/usr/local/bin;  
	@touch ${DHD_PRIV}/.nvt_finish

install:
	@echo ">>>>>>>>>>>>>>>>>>> $@ >>>>>>>>>>>>>>>>>>>"
	@$(call check_exist_cp_bin,bin)
	@$(call check_exist_cp_bin,sbin)
	@$(call check_exist_cp_bin,usr/bin)
	@$(call check_exist_cp_bin,usr/sbin)
	@$(call check_exist_cp_bin,usr/local/bin)
	@$(call check_exist_cp_bin,usr/local/sbin)
	@$(call check_exist_cp_lib,lib)
	@$(call check_exist_cp_lib,usr/local/lib)
	@$(call check_exist_cp_lib,include)

clean:
	@echo ">>>>>>>>>>>>>>>>>>> Remove >>>>>>>>>>>>>>>>>>>"
	@rm -rf ${EXTERNAL}/${INSTALL_DIR} ${CURL} ${NTP} ${ISCSI} ${IPERF} ${IPERF3} ${WIRELESS_TOOL} ${WPA_SUPPLICANT} ${HOSTAPD} ${QUECTEL_CM} ${ZTE_CONNECT} ${E2FSPROGS} ${IPTABLES} ${EXFAT_UTILS} ${DHD_PRIV} ${NVT_REMOTE_FW} 
