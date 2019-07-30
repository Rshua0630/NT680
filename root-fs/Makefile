MTD_CFG = ${ROOTFS_DIR}/mtd_cfg.txt
COPYSO_CHK_BIN_PATH =bin sbin usr/bin usr/sbin usr/local/bin usr/local/sbin var/www var/www/onvif var/www/cgi-bin
COPYSO_SCRIPT = $(LINUX_BUILD_TOP)/build/copyso.sh
IMGNAME_RAMDISK="rootfs.ramdisk.bin"
IMGNAME_TARBZ2="rootfs.tar.bz2"
IMGNAME_UBIFS="rootfs.ubifs.bin"
IMGNAME_UBIFS_RW="rootfs_1.rw.ubifs.bin"
IMGNAME_SQUASHFS="rootfs.squash.bin"
IMGNAME_JFFS2="rootfs.jffs2.bin"
IMGNAME_JFFS2_RW="rootfs_1.rw.jffs2.bin"
IMGNAME_EXT4="rootfs.ext4.bin"
IMGNAME_FAT_CACHE="rootfs_1.cache.vfat.bin"
MODELCFG_PROFILE_PATH="${ROOTFS_DIR}/rootfs/etc/profile_prjcfg"

include $(NVT_PRJCFG_MODEL_CFG)
-include $(MTD_CFG)

ifeq ($(NVT_BINARY_FILE_STRIP), yes)
STRIPDIRS = ${ROOTFS_DIR}/rootfs/bin/* ${ROOTFS_DIR}/rootfs/sbin/* ${ROOTFS_DIR}/rootfs/user/bin/* ${ROOTFS_DIR}/rootfs/usr/sbin/*
STRIPLIBS = ${ROOTFS_DIR}/rootfs/lib/* ${ROOTFS_DIR}/rootfs/usr/lib/*
endif

define gen_rootfs_init
	echo -e "\e[1;44mRootfs are generating.....\e[0m"
	if [ -z $(LINUX_BUILD_TOP) ]; then \
		echo -e "\r\nERROR :Please source build/envsetup.sh in NA51000_Linux firstly to have auto copyso function\r\n"; \
		exit 1; \
	fi
	if [ ! -e $(ROOTFS_DIR)/bin ]; then \
		mkdir $(ROOTFS_DIR)/bin; \
	else \
		if [ -d $(ROOTFS_DIR)/bin/mnt ]; then sudo umount $(ROOTFS_DIR)/bin/mnt; fi; \
		rm -rf $(ROOTFS_DIR)/bin/*; \
        fi
	if [ ! -z $(LINUX_BUILD_TOP) ]; then \
		sh $(COPYSO_SCRIPT) "$(ROOTFS_DIR)/rootfs" $(COPYSO_CHK_BIN_PATH); \
	fi
endef

define gen_rootfs_etc_link
	if [ ! -z $(NVT_ROOTFS_ETC) ]; then \
		cd $(ROOTFS_DIR)/rootfs; rm -rf etc; ln -s etc_Model/etc_$(NVT_ROOTFS_ETC) etc; \
	else \
		MODEL=`echo $(NVT_PRJCFG_MODEL_CFG) | awk -F'/' '{print $$NF}' | awk -F'.' '{print $$1}' | awk -F'ModelConfig_' '{print $$NF}'`; \
		cd $(ROOTFS_DIR)/rootfs; rm -rf etc; ln -s etc_Model/etc_$$MODEL etc; \
		echo -e "\e[1;32mYour model is $$MODEL\e[0m"; \
	fi
endef

define gen_rootfs_ubinize_cfg
	echo -e "[ubifs]\nmode=ubi\nimage=bin/$(IMGNAME_UBIFS).raw\nvol_id=0" > bin/ubinize.cfg; \
	echo -e "vol_size=$(shell echo $(ROOTFS_UBI_MAX_LEB_COUNT)\*126976|bc)" >> bin/ubinize.cfg; \
	echo -e "vol_type=dynamic\nvol_name=rootfs" >> bin/ubinize.cfg; \
	echo -e "[ubifs]\nmode=ubi\nimage=bin/$(IMGNAME_UBIFS_RW).raw\nvol_id=0" > bin/ubinize_rw.cfg; \
	echo -e "vol_size=$(shell echo $(ROOTFS_UBI_RW_MAX_LEB_COUNT)\*126976|bc)" >> bin/ubinize_rw.cfg; \
	echo -e "vol_type=dynamic\nvol_name=rootfs" >> bin/ubinize_rw.cfg;
endef

define gen_rootfs_tmp
	rm -rf $(ROOTFS_DIR)/rootfs.tmp;
	mkdir $(ROOTFS_DIR)/rootfs.tmp; cd $(ROOTFS_DIR)/rootfs; cp -arf `ls | grep -v etc` $(ROOTFS_DIR)/rootfs.tmp;
	@# Generate etc folder
	@# List all etc folder files
	$(eval etc_files_list=`cd $(ROOTFS_DIR)/rootfs/etc; find .`)
	cd $(ROOTFS_DIR)/rootfs/etc/; \
	for file in $(etc_files_list); do \
		if [ -L $$file ]; then \
			mkdir -p $(ROOTFS_DIR)/rootfs.tmp/etc/`dirname $$file`; \
			cp -rf $$file $(ROOTFS_DIR)/rootfs.tmp/etc/$$file; \
		elif [ -d $$file ]; then \
			mkdir -p $(ROOTFS_DIR)/rootfs.tmp/etc/$$file; \
		else \
			mkdir -p $(ROOTFS_DIR)/rootfs.tmp/etc/`dirname $$file`; \
			cp -f $$file $(ROOTFS_DIR)/rootfs.tmp/etc/$$file; \
		fi \
	done
endef

define gen_selected_rootfs_ro
	echo -e "\e[1;32m$(NVT_ROOTFS_TYPE).....\e[0m"
	if [ "$(NVT_ROOTFS_TYPE)" == "NVT_ROOTFS_TYPE_NAND_UBI" ]; then \
		cd $(ROOTFS_DIR); mkfs.ubifs --squash-uids -x $(ROOTFS_UBI_COMPRESS_MODE) -m $(ROOTFS_UBI_PAGE_SIZE) -e $(ROOTFS_UBI_ERASE_BLK_SIZE) -c $(ROOTFS_UBI_MAX_LEB_COUNT) -o bin/$(IMGNAME_UBIFS).raw -r rootfs.tmp; \
		$(call gen_rootfs_ubinize_cfg) \
		ubinize -m $(ROOTFS_UBI_PAGE_SIZE) -p $(ROOTFS_UBI_BLK_SIZE) -s $(ROOTFS_UBI_PAGE_SIZE) -o bin/$(IMGNAME_UBIFS).ubi.raw bin/ubinize.cfg; \
		$(BUILD_DIR)/nvt-tools/nvt-ld-op --packsum-src=bin/$(IMGNAME_UBIFS).ubi.raw --packsum-dst=bin/$(IMGNAME_UBIFS) --packsum-type=0x9; \
	elif [ "$(NVT_ROOTFS_TYPE)" == "NVT_ROOTFS_TYPE_NAND_SQUASH" ] || [ "$(NVT_ROOTFS_TYPE)" == "NVT_ROOTFS_TYPE_NOR_SQUASH" ]; then \
		cd $(ROOTFS_DIR); mksquashfs rootfs.tmp bin/$(IMGNAME_SQUASHFS).raw -comp $(ROOTFS_SQ_COMPRESS_MODE) -b $(ROOTFS_SQ_BLK_SIZE) -root-owned; \
		$(BUILD_DIR)/nvt-tools/nvt-ld-op --packsum-src=bin/$(IMGNAME_SQUASHFS).raw --packsum-dst=bin/$(IMGNAME_SQUASHFS) --packsum-type=0x9; \
	elif [ "$(NVT_ROOTFS_TYPE)" == "NVT_ROOTFS_TYPE_NAND_JFFS2" ]; then \
		cd $(ROOTFS_DIR); mkfs.jffs2 -n -s $(ROOTFS_JFFS2_PAGE_SIZE) -e $(ROOTFS_JFFS2_BLK_SIZE) -r rootfs.tmp -U -X $(ROOTFS_JFFS2_COMPRESS_MODE) -p $(ROOTFS_JFFS2_SIZE) -o bin/$(IMGNAME_JFFS2).raw; \
		$(BUILD_DIR)/nvt-tools/nvt-ld-op --packsum-src=bin/$(IMGNAME_JFFS2).raw --packsum-dst=bin/$(IMGNAME_JFFS2) --packsum-type=0x9; \
	elif [ "$(NVT_ROOTFS_TYPE)" == "NVT_ROOTFS_TYPE_NOR_JFFS2" ]; then \
		cd $(ROOTFS_DIR); mkfs.jffs2 -n -s $(ROOTFS_JFFS2_PAGE_SIZE) -e $(ROOTFS_JFFS2_BLK_SIZE) -r rootfs.tmp -U -X $(ROOTFS_JFFS2_COMPRESS_MODE) -o bin/$(IMGNAME_JFFS2).raw; \
		$(BUILD_DIR)/nvt-tools/nvt-ld-op --packsum-src=bin/$(IMGNAME_JFFS2).raw --packsum-dst=bin/$(IMGNAME_JFFS2) --packsum-type=0x9; \
	elif [ "$(NVT_ROOTFS_TYPE)" == "NVT_ROOTFS_TYPE_EMMC" ]; then \
		cd $(ROOTFS_DIR); make_ext4fs -s -l $(ROOTFS_EXT4_SIZE) bin/$(IMGNAME_EXT4).raw rootfs.tmp; \
		$(BUILD_DIR)/nvt-tools/nvt-ld-op --packsum-src=bin/$(IMGNAME_EXT4).raw --packsum-dst=bin/$(IMGNAME_EXT4) --packsum-type=0x9; \
	else \
		cd $(ROOTFS_DIR)/rootfs.tmp; find . | cpio -o -H newc > $(ROOTFS_DIR)/bin/initramfs.cpio; \
		$(ROOTFS_DIR)/tools/lzop $(ROOTFS_DIR)/bin/initramfs.cpio; \
		$(ROOTFS_DIR)/tools/mkimage -A arm -T ramdisk -C none -n 'NVT initramfs' -d $(ROOTFS_DIR)/bin/initramfs.cpio.lzo $(ROOTFS_DIR)/bin/$(IMGNAME_RAMDISK); \
	fi

	@# tar.bz2 generate
	rm -rf $(ROOTFS_DIR)/bin/$(IMGNAME_TARBZ2); cd $(ROOTFS_DIR)/rootfs.tmp; tar -jcf $(ROOTFS_DIR)/bin/$(IMGNAME_TARBZ2) .
endef

define rm_rootfs_tmp_dir
	rm -rf $(ROOTFS_DIR)/rootfs.tmp
endef

define gen_selected_rootfs_rw
	if [ "$(NVT_ROOTFS_TYPE)" == "NVT_ROOTFS_TYPE_NAND_UBI" ]; then \
		cd $(ROOTFS_DIR); mkfs.ubifs --squash-uids -x $(ROOTFS_UBI_COMPRESS_MODE) -m $(ROOTFS_UBI_PAGE_SIZE) -e $(ROOTFS_UBI_ERASE_BLK_SIZE) -c $(ROOTFS_UBI_RW_MAX_LEB_COUNT) -o bin/$(IMGNAME_UBIFS_RW).raw -r rootfs.tmp; \
		$(call gen_rootfs_ubinize_cfg) \
		ubinize -m $(ROOTFS_UBI_PAGE_SIZE) -p $(ROOTFS_UBI_BLK_SIZE) -s $(ROOTFS_UBI_PAGE_SIZE) -o bin/$(IMGNAME_UBIFS_RW).ubi.raw bin/ubinize_rw.cfg; \
		$(BUILD_DIR)/nvt-tools/nvt-ld-op --packsum-src=bin/$(IMGNAME_UBIFS_RW).ubi.raw --packsum-dst=bin/$(IMGNAME_UBIFS_RW) --packsum-type=0x9; \
	elif [ "$(NVT_ROOTFS_TYPE)" == "NVT_ROOTFS_TYPE_NAND_SQUASH" ] || [ "$(NVT_ROOTFS_TYPE)" == "NVT_ROOTFS_TYPE_NAND_JFFS2" ]; then \
		cd $(ROOTFS_DIR); mkfs.jffs2 -n -s $(ROOTFS_JFFS2_PAGE_SIZE) -e $(ROOTFS_JFFS2_BLK_SIZE) -r rootfs.tmp -U -X $(ROOTFS_JFFS2_COMPRESS_MODE) -p $(ROOTFS_JFFS2_RW_SIZE) -o bin/$(IMGNAME_JFFS2_RW).raw; \
		$(BUILD_DIR)/nvt-tools/nvt-ld-op --packsum-src=bin/$(IMGNAME_JFFS2_RW).raw --packsum-dst=bin/$(IMGNAME_JFFS2_RW) --packsum-type=0x9; \
	elif [ "$(NVT_ROOTFS_TYPE)" == "NVT_ROOTFS_TYPE_EMMC" ]; then \
		cd $(ROOTFS_DIR); $(BUILD_DIR)/gen_vfat_image.sh bin/$(IMGNAME_FAT_CACHE).raw tools/rootfs_recovery `expr $(ROOTFS_FAT_CACHE_SIZE) / 1024 / 1024`; \
		$(BUILD_DIR)/nvt-tools/reduce_fat_bin.py bin/$(IMGNAME_FAT_CACHE).raw bin/$(IMGNAME_FAT_CACHE).raw.sparse; \
		$(BUILD_DIR)/nvt-tools/nvt-ld-op --packsum-src=bin/$(IMGNAME_FAT_CACHE).raw.sparse --packsum-dst=bin/$(IMGNAME_FAT_CACHE) --packsum-type=0x9; \
	else \
		continue; \
	fi
endef

define rm_rootfs_tmp_files
	rm -rf $(ROOTFS_DIR)/rootfs.tmp/*
endef

define strip_binary_files
	if [ "${STRIPDIRS}" != "" ]; then \
		STRIPFILES=`file ${STRIPDIRS} | grep "not stripped" | awk 'BEGIN {FS=":"}; {print $$1}'`; \
		for target_file in $$STRIPFILES; do \
			chmod 755 $$target_file; \
			${STRIP} $$target_file; \
		done; \
	fi
	if [ "${STRIPLIBS}" != "" ]; then \
		STRIPFILES=`file ${STRIPLIBS} | grep "not stripped" | awk 'BEGIN {FS=":"}; {print $$1}'`; \
		for target_file in $$STRIPFILES; do \
			chmod 755 $$target_file; \
			${STRIP} -g --strip-unneeded $$target_file; \
		done; \
	fi
endef

define install_rootfs
	echo -e "\e[1;44mRootfs are installing.....\e[0m"
	cd $(ROOTFS_DIR)/bin/; cp rootfs*.bin rootfs*.bin.raw $(LINUX_BUILD_TOP)/output/
endef

define gen_prjcfg_profile_init
	echo -e "\e[1;44m To do rootfs/etc/profile_prjcfg is generating .....\e[0m"
	if [ -z $(LINUX_BUILD_TOP) ]; then \
		echo -e "\r\nERROR :Please source build/envsetup.sh in NA51000_Linux firstly to have auto copyso function\r\n"; \
		exit 1; \
	fi
	echo "export MODEL=`echo ${NVT_PRJCFG_MODEL_CFG} | awk -F'ModelConfig_' '{print $$NF}' | awk -F'.txt' '{print $$1}'`" > $(MODELCFG_PROFILE_PATH);
	grep '^[^#][[:alnum:]_]*[[:blank:]]*=[[:blank:]]*[[:alnum:][:blank:]_]*' ${NVT_PRJCFG_MODEL_CFG}  | sed 's/\([[:alnum:]_]*\)[[:blank:]]*=[[:blank:]]*\([[:alnum:][:blank:]_\.$$\(\)]*\)/export \1="\2"/g' >> $(MODELCFG_PROFILE_PATH);
	sed -i '/BIN_NAME/d' $(MODELCFG_PROFILE_PATH);
	echo "export LD_PRELOAD='libnvtlibc.so'" >> $(MODELCFG_PROFILE_PATH);	
endef

define clean_rootfs
	rm -rf ${ROOTFS_DIR}/bin
	rm -rf ${ROOTFS_DIR}/rootfs/bin
	rm -rf ${ROOTFS_DIR}/rootfs/sbin
	rm -rf ${ROOTFS_DIR}/rootfs/lib/*
	rm -rf ${ROOTFS_DIR}/rootfs/usr/bin
	rm -rf ${ROOTFS_DIR}/rootfs/usr/local
	rm -rf ${ROOTFS_DIR}/rootfs/usr/sbin
	rm -rf ${ROOTFS_DIR}/rootfs/usr/lib/*
	rm -rf ${ROOTFS_DIR}/rootfs/var/www/cgi-bin/*
	rm -rf ${ROOTFS_DIR}/rootfs/etc
	rm -rf ${ROOTFS_DIR}/rootfs/man
	rm -rf ${ROOTFS_DIR}/rootfs/share
	if [ -d $(LINUX_BUILD_TOP)/output ]; then cd $(LINUX_BUILD_TOP)/output; rm -rf $(IMGNAME_RAMDISK) $(IMGNAME_UBIFS) $(IMGNAME_TARBZ2); fi
endef

all:
	@$(call gen_rootfs_init)

	@$(call gen_rootfs_etc_link)

	@$(call gen_prjcfg_profile_init)

	@$(call strip_binary_files)

	@$(call gen_rootfs_tmp)

	@$(call gen_selected_rootfs_ro)

	@$(call rm_rootfs_tmp_files)

	@$(call gen_selected_rootfs_rw)

	@$(call rm_rootfs_tmp_dir)

gen_etc:
	@$(call gen_rootfs_etc_link)

install:
	@$(call install_rootfs)

clean:
	@$(call clean_rootfs)
	@$(call rm_rootfs_tmp)
