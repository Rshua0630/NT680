#
# (C) Copyright 2000, 2001, 2002
# Wolfgang Denk, DENX Software Engineering, wd@denx.de.
#
# SPDX-License-Identifier:	GPL-2.0+
#

KBUILD_CFLAGS := -D__NVT_NAND_UBOOT__ -fno-builtin
obj-y	:= nt72656tk.o

# EMMC related
obj-$(CONFIG_NVT_EMMC)	+= nt72668_stbc.o nt72668_mpll.o
obj-$(CONFIG_NVT_EMMC)   += nt72668_emmc_fwupdate.o nt72668_emmc_partition.o nvtmbr.o
obj-$(CONFIG_NVT_EMMC) += nt72668_utils.o


#obj-$(CONFIG_NT72656_USB)	+= nvt_ehci.o
obj-$(CONFIG_NT72656_SPI)	+= nvt_spiflash.o
obj-$(CONFIG_NT72656_NAND)	+= nand/nvt_nand_uboot.o
obj-$(CONFIG_NT72656_NAND)	+= nand/nvt_nand.o
obj-$(CONFIG_NT72656_NAND)	+= nand/nvt_nand_flash_list.o
obj-$(CONFIG_NT72656_NAND)	+= nand/nvt_nand_nfc.o
obj-$(CONFIG_NT72656_NAND)	+= nand/cmd_nvtNand.o
obj-$(CONFIG_NT72656_NAND)	+= nand/cmd_nvtNandUpdateUtil.o
