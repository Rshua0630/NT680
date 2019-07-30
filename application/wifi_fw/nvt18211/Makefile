FWDIR = $(ROOTFS_DIR)/rootfs/lib/firmware/nvt

install:
	mkdir -p $(FWDIR)
	cp -af dsw_18211_ap.img $(FWDIR)
	cp -af dsw_18211_sta.img $(FWDIR)
	cp -af dsw_18211_hut.img $(FWDIR)
	cp -af cpf_linux.cpf $(FWDIR)

clean:
	rm -rf $(FWDIR)
