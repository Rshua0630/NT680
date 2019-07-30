SHELL=/bin/bash
checkdep = cd $(1); \
		target=`echo $$n | awk -F'nvt@lib@' '{print $$NF;}'`; \
		if [ -f $$target/Makefile ]; then \
			dep=`cat $$target/Makefile | grep DEP_LIBRARIES | awk -F'=' '{print $$NF;}'`; \
			if [ ! -z `echo $$dep | tr -d ' '` ]; then \
				dep_cond="TRUE"; \
				for dep_n in $$dep; do \
					if [ ! -z `echo $$dep_n | grep nvt@ext@lib@` ]; then \
						make_target=`echo $$dep_n | awk -F'nvt@ext@lib@' '{print $$NF;}'`; \
						cd $(LIBRARY_DIR)/external; if [ ! -f `find . -maxdepth 1 -type d -name "$$make_target*"`/.nvt_finish ]; then dep_cond="FALSE"; fi; \
					elif [ ! -z `echo $$dep_n | grep nvt@ext@app@` ]; then \
						make_target=`echo $$dep_n | awk -F'nvt@ext@app@' '{print $$NF;}'`; \
						cd $(APP_DIR)/external; if [ ! -f `find . -maxdepth 1 -type d -name "$$make_target*"`/.nvt_finish ]; then dep_cond="FALSE"; fi; \
					elif [ ! -z `echo $$dep_n | grep nvt@lib@` ]; then \
						make_target=`echo $$dep_n | awk -F'nvt@lib@' '{print $$NF;}'`; \
						cd $(LIBRARY_DIR); if [ ! -f `find . -maxdepth 1 -type d -name "$$make_target*"`/.nvt_finish ]; then dep_cond="FALSE"; fi; \
					else \
						if [ ! -f $(APP_DIR)/$$dep_n/.nvt_finish ]; then \
							dep_cond="FALSE"; \
						fi; \
					fi; \
				done; \
				echo ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> $(2) Dep = $$dep <<<<<<<<<<<<<<<<<<<<<<<<<<<"; \
				if [ $$dep_cond == "TRUE" ]; then \
					dirname=`echo $(2)| tr -d ' '`; \
					cd $(1); make -C $$target all || exit "$$?"; make -C $$target install; \
					touch $$target/.nvt_finish; \
					tmp=`echo $(2) | tr -d ' '`; \
					maked_list=`echo $$maked_list | sed -e 's/\<'$$tmp'\>//g'`; \
				fi; \
			else \
				echo ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> $(2) Non-dep <<<<<<<<<<<<<<<<<<<<<<<<<<<"; \
				dirname=`echo $(2)| tr -d ' '`; \
				cd $(1); make -C $$target all || exit "$$?"; make -C $$target install; \
				touch $$target/.nvt_finish; \
				tmp=`echo $(2) | tr -d ' '`; \
				maked_list=`echo $$maked_list | sed -e 's/\<'$$tmp'\>//g'`; \
			fi; \
		fi; \

makedep = make_cond="TRUE"; \
	maked_list="$(1)"; \
	while [ $$make_cond == "TRUE" ];\
	do \
		echo $$maked_list; \
		maked_list_old=`echo $$maked_list`; \
		for n in $$maked_list; do \
			if [ ! -z `echo $$n | grep nvt@ext@lib@` ]; then \
				echo "Check lib/ext $$n"; \
				make_target=`echo $$n | awk -F'nvt@ext@lib@' '{print $$NF;}'`; \
				cd $(LIBRARY_DIR)/external; \
				if [ ! -f $$make_target*/.nvt_finish ]; then \
					make $$make_target; make install; \
					if [ $$? == 0 ]; then \
						dirname=`find . -maxdepth 1 -type d -name "$$make_target*"`; \
						touch $$dirname/.nvt_finish; \
					fi; \
				fi; \
				maked_list=`echo $$maked_list | sed -e 's/\<'$$n'\>//g'`; \
			elif [ ! -z `echo $$n | grep nvt@ext@app@` ]; then \
				echo "Check app/ext $$n"; \
				make_target=`echo $$n | awk -F'nvt@ext@app@' '{print $$NF;}'`; \
				cd $(APP_DIR)/external; \
				dirname=`find . -maxdepth 1 -type d -name "$$make_target*" | head -n 1`; \
				chk_null=`if [ -f $$dirname/.nvt_finish ]; then cat $$dirname/.nvt_finish; fi`; \
				if [ ! -f $$dirname/.nvt_finish ] || [ "$$chk_null" != "" ]; then \
					make $$make_target; make install; \
					if [ $$? == 0 ]; then \
						dirname=`find . -maxdepth 1 -type d -name "$$make_target*"`; \
						touch $$dirname/.nvt_finish; \
					fi; \
				fi; \
				maked_list=`echo $$maked_list | sed -e 's/\<'$$n'\>//g'`; \
			elif [ ! -z `echo $$n | grep nvt@lib@` ]; then \
				echo "checkdep $$n"; \
				$(call checkdep, $(LIBRARY_DIR), $$n) \
			else \
				echo "checkdep $$n"; \
				$(call checkdep, $(APP_DIR), $$n) \
			fi; \
		done; \
		if [ -z "$$maked_list" ]; then \
			make_cond="FALSE"; \
		elif [ "`echo $$maked_list | tr -d ' '`" == "`echo $$maked_list_old | tr -d ' '`" ]; then \
			echo -e "\e[1;41mStop build, because $$maked_list can't pass build system check\e[0m"; \
			exit 1; \
			make_cond="FALSE"; \
		fi; \
	done

makelist = $(foreach n, $(1), make -C $(n) $(2) || exit "$$?"; if [ ! -z `echo $(2) | grep clean` ]; then rm -f $(n)/.nvt_finish; fi;)

update_modelcfg_to_build = $(shell cp $$NVT_PRJCFG_MODEL_CFG $$BUILD_DIR/.nvt_modelcfg)

list_subdir = $(shell cd $(1); find . -maxdepth 1 -type d -not -name ".*" -not -name "external" | sed -e 's/.\///g')
chk_src = $(shell cd $(1); for n in $(modelcfg_diff); do \
				stat=`grep -r $$n . | egrep 'defined|ifdef|ifeq|define'`; \
				if [ ! -z "$$stat" ]; then \
					stat="true"; \
					break; \
				fi; \
			done; \
			echo $$stat; \
		)
chk_src_makefile = $(shell cd $(1); for n in $(modelcfg_diff); do \
					stat=`grep -r $$n Makefile`; \
					if [ ! -z "$$stat" ]; then \
						stat="true"; \
						break; \
					fi; \
				done; \
				echo $$stat; \
			)
modelcfg_diff = $(shell if [ -f $(BUILD_DIR)/.nvt_modelcfg ]; then \
				ret_now=`sdiff $(NVT_PRJCFG_MODEL_CFG) $(BUILD_DIR)/.nvt_modelcfg | grep '[|]' | awk -F'|' '{print $$2}' | awk -F'=' '{print $$1}' | sed -e "/\#/d"`; \
				if [ -f $(BUILD_DIR)/.nvt_modelcfg_tmp ]; then \
					ret_prev=`sdiff $(BUILD_DIR)/.nvt_modelcfg_tmp $(BUILD_DIR)/.nvt_modelcfg | grep '[|]' | awk -F'|' '{print $$2}' | awk -F'=' '{print $$1}' | sed -e "/\#/d"`; \
				fi; \
				ret="$$ret_now $$ret_prev"; \
				ret_now=""; ret_prev=""; \
				for n in $$ret; do \
					ret_now="$$ret_now `grep -r $$n $(NVT_PRJCFG_MODEL_CFG) | awk -F'=' '{print $$1}' | sed -e "/\#/d" | sed -e "/$$n/d"`"; \
					if [ -f $(BUILD_DIR)/.nvt_modelcfg_tmp ]; then \
						ret_prev="$$ret_prev `grep -r $$n $(BUILD_DIR)/.nvt_modelcfg_tmp | awk -F'=' '{print $$1}' | sed -e "/\#/d" | sed -e "/$$n/d"`"; \
					fi; \
				done; \
				for n in $$ret_now $$ret_prev; \
				do \
					if [ ! -z $$n ]; then \
						if [ -z "`echo $$ret | grep $$n`" ]; then \
							ret="$$ret $$n"; \
						fi; \
					fi; \
				done; \
				echo $$ret; \
			else \
				exit 2; \
			fi;)

dep_build_list = $(shell dep_build_list=""; \
			for n in $(modelcfg_diff); do \
				cd $(1); \
				for src_dir in $(call list_subdir, $(1)); do \
					if [ -d $$src_dir ]; then \
						stat=`grep -r $$n $$src_dir | egrep 'defined|ifdef|ifeq'`; \
						if [ ! -z "$$stat" ]; then \
							dep_build_list="$$src_dir $$dep_build_list"; \
						fi; \
					fi; \
				done; \
			done; \
			echo $$dep_build_list; \
		)
str_remove_duplicate = $(shell result=""; \
				for n in $(1); \
				do \
					if [ ! -z $$n ]; then \
						if [ -z "`echo $$result | grep $$n`" ]; then \
							result="$$result $$n"; \
						fi; \
					fi; \
				done; \
			echo $$result;)

get_ext_list = $(shell cd $(1)/external; echo `make list`;)

get_ext_rebuild_list = $(shell result=""; \
				for n in $(call get_ext_list, $(1)); \
				do \
					for k in $(2); \
					do \
						if [ ! -z "`$(BUILD_DIR)/get_ext_target_content.sh "$$n:" "$(1)/external/Makefile" | grep "$$k"`" ]; then \
							result="$$result $$n"; \
						fi; \
					done; \
				done; \
				echo $$result;)

chk_for_kernel_rebuild = $(shell chk_str="NVT_CFG_KERNEL_CFG NVT_LINUX_SMP"; \
				for n in $$chk_str; \
				do \
					if [ ! -z "`echo $(modelcfg_diff) | grep $$n`" ]; then \
						echo "true"; \
						exit; \
					fi; \
				done; \
				)

chk_for_rootfs_rebuild = $(shell chk_str="NVT_ROOTFS_ETC NVT_BINARY_FILE_STRIP"; \
				for n in $$chk_str; \
				do \
					if [ ! -z "`echo $(modelcfg_diff) | grep $$n`" ]; then \
						echo "true"; \
						exit; \
					fi; \
				done; \
				)

chk_for_app_rebuild = $(shell chk_str="NVT_CFG_APP_EXTERNAL NVT_CFG_APP"; \
				for n in $$chk_str; \
				do \
					if [ ! -z "`echo $(modelcfg_diff) | grep $$n`" ]; then \
						echo "true"; \
						exit; \
					fi; \
				done; \
				)

make_dep_by_modelselect = \
			if [ ! -f $(BUILD_DIR)/.nvt_modelcfg ]; then echo "Please build all firstly"; exit 1; fi; \
			if [ ! -f $(BUILD_DIR)/.nvt_modelcfg_tmp ]; then cp $(NVT_PRJCFG_MODEL_CFG) $(BUILD_DIR)/.nvt_modelcfg_tmp; fi; \
			build_stat=""; \
			for n in $(call str_remove_duplicate, $(call dep_build_list, $(APP_DIR))); \
			do \
				if [ ! -z $$n ]; then cd $(APP_DIR)/$$n; make clean; build_stat="true"; fi; \
			done; \
			for n in $(call str_remove_duplicate, $(call dep_build_list, $(LIBRARY_DIR))); \
			do \
				if [ ! -z $$n ]; then cd $(LIBRARY_DIR)/$$n; make clean; build_stat="true"; fi; \
			done; \
			cd $(LIBRARY_DIR)/external; \
			for n in $(call get_ext_rebuild_list, $(LIBRARY_DIR), $(modelcfg_diff)); \
			do \
				dirname=`find . -maxdepth 1 -type d -not -name ".*"  | sed -e 's/.\///g' | grep $$n`; \
				rm -rf $$dirname; \
				build_stat="true"; \
			done; \
			for n in $(call get_ext_rebuild_list, $(APP_DIR), $(modelcfg_diff)); \
			do \
				dirname=`find . -maxdepth 1 -type d -not -name ".*"  | sed -e 's/.\///g' | grep $$n`; \
				rm -rf $$dirname; \
				build_stat="true"; \
			done; \
			if [ "$$build_stat" == "true" ] || [ "true" == "$(chk_for_app_rebuild)" ]; then cd $(APP_DIR); make all || exit "$$?"; fi; \
			build_stat=""; \
			if [ "true" == "$(call chk_src, $(UBOOT_DIR))" ]; then \
				cd $(LINUX_BUILD_TOP); make uboot || exit "$$?"; \
			fi; \
			if [ "true" == "$(chk_for_kernel_rebuild)" ]; then \
				cd $(LINUX_BUILD_TOP); make linux || exit "$$?"; make modules || exit "$$?"; make supplement || exit "$$?"; \
				build_stat="true"; \
			else \
				if [ "true" == "$(call chk_src_makefile, $(SUPPLEMENT_DIR))" ]; then \
					cd $(LINUX_BUILD_TOP); make supplement || exit "$$?"; \
					build_stat="true"; \
				fi; \
			fi; \
			if [ "true" == "$(chk_for_rootfs_rebuild)" ] || [ "true" == "$(call chk_src_makefile, $(ROOTFS_DIR))" ] || [ "true" == "$$build_stat" ]; then \
				cd $(LINUX_BUILD_TOP); make rootfs || exit "$$?"; \
			fi; \
			if [ ! -z "$(modelcfg_diff)" ]; then \
				cd $(LINUX_BUILD_TOP); make uitron_clean; make uitron || exit "$$?"; \
			fi; \
			if [ ! -z "$(modelcfg_diff)" ]; then echo -e "\e[1;33mChanged model config items >>> $(modelcfg_diff) \e[0m"; make pack; else echo -e "\e[1;33mModel config without any change\e[0m"; fi; \
			cp $(NVT_PRJCFG_MODEL_CFG) $(BUILD_DIR)/.nvt_modelcfg; rm $(BUILD_DIR)/.nvt_modelcfg_tmp;

gen_nvt_pack = \
		NVT_BIN_NAME=`cat $(NVT_PRJCFG_MODEL_CFG) | grep -E '(^|\s)BIN_NAME($|\s)' | grep = | awk -F'= ' '{print $$NF}' | tr -d ' '`; \
		if [ "FW\$$(CHIP)A" == "$$NVT_BIN_NAME" ]; then \
			NVT_BIN_NAME=FW`cat $(NVT_PRJCFG_MODEL_CFG) | grep -E '(^|\s)CHIP($|\s)' | grep -v FW | grep = | awk -F'= ' '{print $$NF}'`A; \
		fi; \
		DSP1_BIN=`cat $(NVT_PRJCFG_MODEL_CFG) | grep DSP1_TYPE | grep -v "\#" | grep DSP1_FREERTOS`; \
		DSP2_BIN=`cat $(NVT_PRJCFG_MODEL_CFG) | grep DSP2_TYPE | grep -v "\#" | grep DSP2_FREERTOS`; \
		if [ -f $(OUTPUT_DIR)/$$NVT_BIN_NAME.ini ]; then \
			echo -e "\e[1;33mnvt pack config is existing...\e[0m"; \
		else \
			echo -e "\e[1;33mnvt pack config generation...\e[0m"; \
			cd $(OUTPUT_DIR); $(BUILD_DIR)/nvt-tools/nvtpack -gen $$NVT_BIN_NAME.ext.bin; \
			if [ ! -z "$$DSP1_BIN" ]; then \
				sed -i 's/0 DSP1/1 DSP1/g' $(OUTPUT_DIR)/$$NVT_BIN_NAME.ini; \
			fi; \
			if [ ! -z "$$DSP2_BIN" ]; then \
				sed -i 's/0 DSP2/1 DSP2/g' $(OUTPUT_DIR)/$$NVT_BIN_NAME.ini; \
			fi; \
		fi; \
		echo -e "\e[1;33mnvt pack image is generated...\e[0m"; \
		cd $(OUTPUT_DIR); $(BUILD_DIR)/nvt-tools/nvtpack -fw $(OUTPUT_DIR)/$$NVT_BIN_NAME.ini

rm_nvt_pack = \
		NVT_BIN_NAME=`cat $(NVT_PRJCFG_MODEL_CFG) | grep -E '(^|\s)BIN_NAME($|\s)' | grep = | awk -F'= ' '{print $$NF}'`; \
		if [ "FW\$$(CHIP)A" == "$$NVT_BIN_NAME" ]; then \
			NVT_BIN_NAME=FW`cat $(NVT_PRJCFG_MODEL_CFG) | grep -E '(^|\s)CHIP($|\s)' | grep -v FW | grep = | awk -F'= ' '{print $$NF}'`A; \
		fi; \
		if [ -f $(OUTPUT_DIR)/$$NVT_BIN_NAME.ini ]; then rm $(OUTPUT_DIR)/$$NVT_BIN_NAME.ini; echo -e "\e[1;33mnvt pack config is removed...\e[0m"; fi; \
		rm -rf $(OUTPUT_DIR)/packed;

gen_mbr = \
		NVT_BIN_NAME=`cat $(NVT_PRJCFG_MODEL_CFG) | grep -E '(^|\s)BIN_NAME($|\s)' | grep = | awk -F'= ' '{print $$NF}'`; \
		if [ "FW\$$(CHIP)A" == "$$NVT_BIN_NAME" ]; then \
			NVT_BIN_NAME=FW`cat $(NVT_PRJCFG_MODEL_CFG) | grep -E '(^|\s)CHIP($|\s)' | grep -v FW | grep = | awk -F'= ' '{print $$NF}'`A; \
		fi; \
		EMBMEM=`cat $(NVT_PRJCFG_MODEL_CFG) | grep -E '(^|\s)EMBMEM($|\s)' | grep = | awk -F'= ' '{print $$NF}'`; \
		if [ "$$EMBMEM" == "EMBMEM_EMMC" ]; then \
			echo -e "\e[1;33mGenerate MBR\e[0m"; \
			cd $(OUTPUT_DIR); \
			$(BUILD_DIR)/nvt-tools/make_mbr.sh $(OUTPUT_DIR)/$$NVT_BIN_NAME.ext.bin; \
		fi
