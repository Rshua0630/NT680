# To build modules outside of the kernel tree, we run "make"
# in the kernel source tree; the Makefile these then includes this
# Makefile once again.
# This conditional selects whether we are being included from the
# kernel Makefile or not.
include $(NVT_PRJCFG_MODEL_CFG)
ifeq ($(KERNELRELEASE),)

	SUBDIRS := $(dir $(wildcard */*/Makefile */*/makefile))
	# The current directory is passed to sub-makes as argument
	PWD := $(shell pwd)

modules:
	$(MAKE) -C $(KERNELDIR)/OUTPUT M=$(PWD) modules;

modules_install:
	$(MAKE) -C $(KERNELDIR)/OUTPUT M=$(PWD) INSTALL_MOD_PATH=./_install_modules/ modules_install

clean:
	@for n in $(SUBDIRS); do \
		SRC="`find $$n -name  *.c ! -name *.mod.c`"; \
		if [ "$$SRC" ]; then \
			rm -rf .tmp_versions Module.symvers modules.order `find $$n -type f -name *.mod.c -o -name .*.cmd -o -name *.o -o -name *.ko -o -name modules.order -o -name *~`; \
			rm -f $(KBUILD_OUTPUT)/supplement.config; \
		else \
			echo ">>> Skip"; \
		fi \
	done

.PHONY: modules modules_install clean

else
# called from kernel build system: just declare what our modules are
obj-m += \
	misc/nvt_uctrl/ \
	misc/nvt_stream_snd/ \
	misc/nvt_status/ \
	misc/nvt_stream_rcv/ \
	misc/nvt_info/ \
	misc/nvt_pstore/ \
	misc/nvt_isp/ \
	crypto/cryptodev-linux/ \
	debug/nvt_data_breakpoint/ \
	fs/exfat/

ifeq ($(NVT_ETHERNET), NVT_ETHERNET_EQOS)
obj-m += net/synopsys/
endif

ifeq ($(NVT_SDIO_WIFI), NVT_SDIO_WIFI_RTK)
obj-m += net/rtl8189es/
obj-m += net/rtl8189fs/
endif

ifeq ($(NVT_SDIO_WIFI), NVT_SDIO_WIFI_BRCM)
obj-m += net/bcmdhd.1.363.59.144.x.cn.nvt/
endif

ifeq ($(NVT_SDIO_WIFI), NVT_SDIO_WIFI_NVT)
obj-m += net/18211_nvtfmac/
endif

ifeq ($(NVT_LINUX_SMP), NVT_LINUX_SMP_ON)
#obj-m += sys/nvt_vos/
endif

endif
