FWDIR = $(ROOTFS_DIR)/rootfs/lib/firmware/ap6212a

install:
	mkdir -p $(FWDIR)
	cp -af fw_bcm43438a1_apsta.bin $(FWDIR)
	cp -af nvram_ap6212a.txt $(FWDIR)
	cp -af config.txt $(FWDIR)

clean:
	rm -rf $(FWDIR)
