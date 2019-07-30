
DRV_NAME = nvtwifi

FW_STA_ORI = nt18211_sta_*.img
FW_AP_ORI = nt18211_ap_*.img
FW_HUT_ORI = nt18211_hut_*.img

FW_STA = dsw_18211_sta.img
FW_AP = dsw_18211_ap.img
FW_HUT = dsw_18211_hut.img

KERNEL_VER = $(shell uname -r)

#ifeq ($(origin KERNEL_SRC),undefined)
#KERNEL_SRC = /lib/modules/$(KERNEL_VER)/build
#endif


#DRV_DST_DIR = /lib/modules/$(KERNEL_VER)/kernel/drivers/net/wireless/nvt/
#FW_SRC_DIR = ./firmware
#FW_DST_DIR = /lib/firmware/nvt

#KERNEL_SRC = ~/novatek/linux/kernel_source/linux-3.12.6
#KERNEL_SRC = ~/kernel_source/from_ksw/linux-3.10

nvtwififlag += -I$(src)/include -DDEBUG

#20160127 nash:flag for bus utilization measurement
#nvtwififlag += -DBUS_UTILIZATION
#nvtwififlag += -DCONFIG_WIFI_TUNING_PHASE_I
#nvtwififlag += -DCONFIG_WIFI_TUNING_PHASE_II
EXTRA_CFLAGS = $(nvtwififlag)

obj-m += $(DRV_NAME).o
nvtwifi-objs += nvt_wlan_linux.o \
			nvt_wlan_priv.o \
			nvt_bus.o \
			nvt_cfg80211.o \
			nvt_util.o \
			nvt_util_dbg.o \
			nvt_fw.o \
			nvt_diag.o \
			nvt_icfg.o

#nvtwifi-objs += nvt_bus_usbif.o
nvtwifi-objs += nvt_bus_sdioif.o
nvtwifi-objs += nvt_bus_mailboxif.o
nvtwifi-objs += nvt_iw.o
