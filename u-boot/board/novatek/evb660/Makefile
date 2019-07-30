#
# (C) Copyright 2000, 2001, 2002
# Wolfgang Denk, DENX Software Engineering, wd@denx.de.
#
# SPDX-License-Identifier:	GPL-2.0+
#

obj-y := $(BOARD).o nvt_plat.o nt96660_utils.o BinInfo.o
obj-y	+= lowlevel_init.o CodeInfo.o
obj-y	+= spinand-660.o cache.o
obj-$(CONFIG_ETHNET)   += eth-660.o eth_dev-660.o
obj-y   += nand-660.o nand_cmd.o nand_host.o cc/cc.o
