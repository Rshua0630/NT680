FWDIR = $(ROOTFS_DIR)/rootfs/lib/firmware/ap6255

install:
	mkdir -p $(FWDIR)
	cp -af fw_bcm43455c0_ag_apsta.bin $(FWDIR)
	cp -af nvram_ap6255.txt $(FWDIR)
	cp -af config.txt $(FWDIR)

clean:
	rm -rf $(FWDIR)
