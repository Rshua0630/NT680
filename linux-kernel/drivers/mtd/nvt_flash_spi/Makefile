#
# Makefile for the SPI NAND/NOR MTD
#

# Core functionality.
ifneq ($(CONFIG_MTD_SPINAND), y)
 ifeq ($(CONFIG_MTD_SPINOR), y) 
  obj-y		+= nvt_flash_spi.o nvt_flash_spi_cmd.o
 endif
else
  obj-y		+= nvt_flash_spi.o nvt_flash_spi_cmd.o
endif


