#
# (C) Copyright 2000, 2001, 2002
# Wolfgang Denk, DENX Software Engineering, wd@denx.de.
#
# SPDX-License-Identifier:	GPL-2.0+
#
BUILD_DATE = 20`date +'%y%m%d'`
ccflags-y += -D_BUILD_DATE_=$(BUILD_DATE)

obj-y := na51000evb.o
obj-y += na51000_utils.o nvt_common.o nvtpack.o nvt_headinfo.o gxdsp.o drv/interrupt.o nvt_ipc.o
obj-y += drv/pinmux.o
obj-$(CONFIG_NVT_LINUX_AUTOLOAD) += nvt_fw_update.o

obj-y += cc/cc.o drv/pll.o drv/dma.o drv/pmc.o dsp/dsp.o drv/iocfg.o drv/rtc.o

obj-$(CONFIG_NVT_PWM) += drv/pwm.o
