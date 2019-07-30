#
# Makefile for the linux kernel.
#
ccflags-y += -I$(srctree)/drivers/soc/nvt/include
obj-y					:= clk.o nvt_stbc.o nvt_reset.o
obj-$(CONFIG_SUSPEND)			+= nvt_pm.o
obj-$(CONFIG_ARCH_NVT_V8)		+= aarch64/
obj-$(CONFIG_ARCH_NVT_V7)		+= aarch32/ ndp_hwmem.o
