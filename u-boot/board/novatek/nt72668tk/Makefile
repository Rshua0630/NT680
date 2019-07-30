#
# (C) Copyright 2000, 2001, 2002
# Wolfgang Denk, DENX Software Engineering, wd@denx.de.
#
# SPDX-License-Identifier:	GPL-2.0+
#

obj-y := nt72668tk.o nt72668_stbc.o

# EMMC related
obj-$(CONFIG_NVT_EMMC)	+= nt72668_stbc.o nt72668_mpll.o
obj-$(CONFIG_NVT_EMMC)   += nt72668_emmc_fwupdate.o nt72668_emmc_partition.o nvtmbr.o nt72668_emmc_xbootdat.o
obj-$(CONFIG_NVT_EMMC) += nt72668_utils.o


#obj-$(CONFIG_NT72656_USB)	+= nvt_ehci.o
obj-$(CONFIG_NT72656_SPI)	+= nvt_spiflash.o nt72668_spi_fwupdate.o nt72668_spi_partition.o
obj-$(CONFIG_NT72656_NAND) += nand_flash.o

