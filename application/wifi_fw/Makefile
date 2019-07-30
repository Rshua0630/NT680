include $(NVT_PRJCFG_MODEL_CFG)

ifeq ($(NVT_SDIO_WIFI), $(filter $(NVT_SDIO_WIFI),NVT_SDIO_WIFI_BRCM))
ifeq ($(WIFI_BRCM_MDL), $(filter $(WIFI_BRCM_MDL),WIFI_BRCM_MDL_43438a1_ampk6212axtal26))
SUBDIRS := ap6212a
else ifeq ($(WIFI_BRCM_MDL), $(filter $(WIFI_BRCM_MDL),WIFI_BRCM_MDL_43455c0_ampk6255c0))
SUBDIRS := ap6255
endif
endif

ifeq ($(NVT_SDIO_WIFI), $(filter $(NVT_SDIO_WIFI),NVT_SDIO_WIFI_NVT))
ifeq ($(WIFI_NVT_MDL), $(filter $(WIFI_NVT_MDL),WIFI_NVT_MDL_18211))
	SUBDIRS := nvt18211
else ifeq ($(WIFI_NVT_MDL), $(filter $(WIFI_NVT_MDL),WIFI_NVT_MDL_18202))
	SUBDIRS := nvt18202
endif
endif

all:
	@for n in $(SUBDIRS); do \
		echo ">>>>>>>>>> processing folder $$n"; \
		$(MAKE) -C $$n || exit "$$?"; \
		echo "Done!"; \
		echo " "; \
	done

install:
	@for n in $(SUBDIRS); do \
		echo ">>>>>>>>>> processing folder $$n install"; \
		$(MAKE) -C $$n install; \
		echo "Done!"; \
		echo " "; \
	done

clean:
	@for n in $(SUBDIRS); do \
		echo ">>>>>>>>>> processing folder $$n clean"; \
		$(MAKE) -C $$n clean; \
		echo "Done!"; \
		echo " "; \
	done
