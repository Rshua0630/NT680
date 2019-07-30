include $(NVT_PRJCFG_MODEL_CFG)

define gen_model_cfg_link
	MODEL=`echo $(NVT_PRJCFG_MODEL_CFG) | awk -F'/' '{print $$NF}' | awk -F'.' '{print $$1}' | awk -F'ModelConfig_' '{print $$NF}'`; \
	MODEL_EXT=`cat $(NVT_PRJCFG_MODEL_CFG) | grep MODELEXT | grep -v "#" | awk -F'=' '{print $$NF}' | awk -F'MODELEXT_' '{print $$NF}'`; \
	cd $(CONFIG_DIR); rm -rf cfg_gen; ln -s cfg_$$MODEL cfg_gen; \
	rm -rf $(CONFIG_DIR)/cfg_$$MODEL/ModelConfig_"$$MODEL".txt; ln -s $(NVT_PRJCFG_MODEL_CFG) $(CONFIG_DIR)/cfg_$$MODEL/ModelConfig_"$$MODEL".txt; \
	rm -rf $(CONFIG_DIR)/cfg_$$MODEL/MODELEXT; ln -s $(UITRON_DIR)/DrvExt/DrvExt_src/ModelExt/"$$MODEL_EXT"/independant $(CONFIG_DIR)/cfg_$$MODEL/MODELEXT;
endef

define clean_model_cfg_link
	MODEL=`echo $(NVT_PRJCFG_MODEL_CFG) | awk -F'/' '{print $$NF}' | awk -F'.' '{print $$1}' | awk -F'ModelConfig_' '{print $$NF}'`; \
	rm -rf $(CONFIG_DIR)/cfg_gen/ModelConfig_*.txt; \
	rm -rf $(CONFIG_DIR)/cfg_$$MODEL/MODELEXT; \
	rm -rf $(CONFIG_DIR)/cfg_gen;
endef

all:
gen_cfg:
	@$(call gen_model_cfg_link)

clean:
	@$(call clean_model_cfg_link)
